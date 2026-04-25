#include "src/cpu/cpu.h"
#include "src/devices/memory.h"
#include "src/instructions/i_type.h"
#include "src/instructions/s_type.h"
#include "src/instructions/b_type.h"
#include "src/instructions/u_type.h"
#include "src/instructions/j_type.h"
#include "src/exceptions/decode_exception.h"
#include "src/cpu/decoder.h"
#include "src/exceptions/execute_exception.h"
#include <iostream>

Cpu::Cpu(Memory& memory, RegisterFile& registerFile)
: memory(&memory), registerFile(&registerFile) {
    LOG("Initializing CPU...");
}


Cpu::~Cpu() { }

void Cpu::step() {
    fetch();
    const DecodedInstr decoded_inst = Decoder::decode(instruction);
    rd = decoded_inst.rd; rs1 = decoded_inst.rs1; rs2 = decoded_inst.rs2;
    pc += 4;
    execute(decoded_inst);
}

void Cpu::fetch() {
    const riscv::instruction_t byte0 = memory->read(pc);
    const riscv::instruction_t byte1 = memory->read(pc + 1);
    const riscv::instruction_t byte2 = memory->read(pc + 2);
    const riscv::instruction_t byte3 = memory->read(pc + 3);
    instruction = byte3 << 24 | byte2 << 16 | byte1 << 8 | byte0;
}

void Cpu::execute(const DecodedInstr& decodedInstr) {
    const InstructionType instructionType = decodedInstr.type;

    switch (instructionType) {
        case IType: executeIType(decodedInstr); break;
        case SType: executeSType(decodedInstr); break;
        case BType: executeBType(decodedInstr); break;
        case UType: executeUType(decodedInstr); break;
        case JType: executeJType(decodedInstr); break;
        case RType: executeRType(decodedInstr); break;
        default:
            throw ExecuteException("execute failed. Unknown instructionType=" + std::to_string(instructionType));
    }
}

void Cpu::executeIType(const DecodedInstr &decodedInstr) {
    switch (decodedInstr.opcode) {
        // LB, LH, LW, LBU, LHU
        case 0x03:
            handleLoads(decodedInstr);
            break;
        // OP-IMM
        case 0x13:
            handleOpImm(decodedInstr);
            break;
        // Jump and Link Register
        case 0x67:
            handleJumpAndLinkRegister(decodedInstr);
            break;
        default:
            throw ExecuteException("executeIType failed, unknown op-code. opcode=" + std::to_string(decodedInstr.opcode));
    }
}

void Cpu::handleLoads(const DecodedInstr &decodedInstr) {
    const riscv::mem_addr_t memAddr = registerFile->read(rs1) + decodedInstr.immediate;
    riscv::uword_t finalValue = 0;
    riscv::word_t  signedTemp = 0;
    riscv::ubyte_t significantByte;
    switch (decodedInstr.funct3) {
        // LB
        case 0b000:
            significantByte = (riscv::byte_t) memory->read(memAddr);

            signedTemp = static_cast<riscv::word_t>(significantByte);
            registerFile->write(rd, (signedTemp << 24) >> 24);
            break;
        // LH
        case 0b001:
            significantByte = (riscv::byte_t) memory->read(memAddr + 1);

            finalValue |= memory->read(memAddr);
            finalValue |= (significantByte << 8);
            signedTemp = static_cast<riscv::word_t>(finalValue);
            registerFile->write(rd, ((signedTemp << 16) >> 16));
            break;
        // LW
        case 0b010:
            finalValue |= memory->read(memAddr);
            finalValue |= (memory->read(memAddr + 1) << 8);
            finalValue |= (memory->read(memAddr + 2) << 16);
            finalValue |= (memory->read(memAddr + 3) << 24);
            registerFile->write(rd, finalValue);
            break;
        // LBU
        case 0b100:
            finalValue |= memory->read(memAddr);
            registerFile->write(rd, finalValue);
            break;
        // LHU
        case 0b101:
            finalValue |= memory->read(memAddr);
            finalValue |= memory->read(memAddr + 1) << 8;
            registerFile->write(rd, finalValue);
            break;
        default:
            throw ExecuteException("Load Operation failed, unknown load decoded from funct3="
            + std::to_string(decodedInstr.funct3));
    }
}

void Cpu::handleOpImm(const DecodedInstr &decodedInstr) {
    riscv::word_t rs1_val = registerFile->read(rs1);
    riscv::word_t imm = decodedInstr.immediate;
    switch(decodedInstr.funct3) {
        //ADDI
        case 0b000:
            registerFile->write(rd, imm + rs1_val);
            break;
        // SLTI
        case 0b010:
            registerFile->write(rd, (rs1_val < imm) ? 1 : 0);
            break;
        // SLTIU
        case 0b011:
            registerFile->write(rd,
                               (static_cast<riscv::uword_t>(rs1_val) < (static_cast<riscv::uword_t>(imm))
                               ? 1 : 0));
            break;
        // XORI
        case 0b100:
            registerFile->write(rd, imm ^ rs1_val);
            break;
        //ORI
        case 0b110:
            registerFile->write(rd, imm | rs1_val);
            break;
        // ANDI
        case 0b111:
            registerFile->write(rd, imm & rs1_val);
            break;
        // SLLI
        case 0b001: {
            riscv::uword_t shift_amount = imm & 0x1F;
            registerFile->write(rd, static_cast<riscv::uword_t>(rs1_val) << shift_amount);
            break;
        }
        // SRLI / SRAI
        case 0b101: {
            riscv::uword_t shift_amount = imm & 0x1F;
            bool isArithmetic = (imm & 0x400) != 0;
            if (isArithmetic) {
                // SRAI: rs1_val is signed so >> is arithmetic
                registerFile->write(decodedInstr.rd, rs1_val >> shift_amount);
            } else {
                // SRLI: cast to unsigned for logical (zero-fill) shift
                registerFile->write(decodedInstr.rd, static_cast<riscv::uword_t>(rs1_val) >> shift_amount);
            }
            break;
        }
        default:
            throw ExecuteException("OP-IMM Operation failed, unknown OP-IMM decoded from funct3="
            + std::to_string(decodedInstr.funct3));
    }
}

void Cpu::handleJumpAndLinkRegister(const DecodedInstr &decodedInstr) {
    riscv::mem_addr_t return_pc = pc;
    riscv::mem_addr_t target_address;
    riscv::word_t rs1_val = registerFile->read(rs1);
    riscv::word_t imm = decodedInstr.immediate;

    switch (decodedInstr.funct3) {
        case 0b000:
            target_address = rs1_val + imm;
            // According to RISC-V Unprivileged Architecture, we set the last bit to 0. (Page 31)
            target_address &= ~1;
            // TODO when moving to Privileged ISA we need to replace this with a hardware exception instead of purely C++
            if (target_address % 4 != 0) {
                throw ExecuteException("FATAL! target_address=" + std::to_string(target_address) + " is misaligned") ;
            }
            pc = target_address;
            registerFile->write(rd, return_pc);
            break;
        default:
            throw ExecuteException("JALR Operation failed, unknown JALR decoded from funct3="
                                   + std::to_string(decodedInstr.funct3));
    }
}

void Cpu::executeSType(const DecodedInstr &decodedInstr) {
    switch (decodedInstr.opcode) {
        // SB, SH, SW
        case 0x23:
            handleStores(decodedInstr);
            break;
        default:
            throw ExecuteException("executeSType failed, unknown op-code. opcode=" + std::to_string(decodedInstr.opcode));
    }
}

void Cpu::handleStores(const DecodedInstr &decodedInstr) {
    riscv::mem_addr_t memAddr = registerFile->read(rs1) + decodedInstr.immediate;
    riscv::uword_t rs2_val = registerFile->read(rs2);
    switch (decodedInstr.funct3) {
        // SB
        case 0b000:
            memory->write(memAddr, rs2_val & 0xFF);
            break;
        // SH
        case 0b001:
            memory->write(memAddr, rs2_val & 0xFF);
            memory->write(memAddr + 1, (rs2_val >> 8) & 0xFF);
            break;
        // SW
        case 0b010:
            memory->write(memAddr, rs2_val & 0xFF);
            memory->write(memAddr + 1, (rs2_val >> 8) & 0xFF);
            memory->write(memAddr + 2, (rs2_val >> 16) & 0xFF);
            memory->write(memAddr + 3, (rs2_val >> 24) & 0xFF);
            break;
        default:
            throw ExecuteException("Store Operation failed, unknown store decoded from funct3="
            + std::to_string(decodedInstr.funct3));
    }
}

void Cpu::executeBType(const DecodedInstr &decodedInstr) {
    switch (decodedInstr.funct3) {
        // BEQ
        case 0b000:
            if (registerFile->read(rs1) == registerFile->read(rs2)) {
                pc += decodedInstr.immediate - 4;
            }
            break;
        // BNE
        case 0b001:
            if (registerFile->read(rs1) != registerFile->read(rs2)) {
                pc += decodedInstr.immediate - 4;
            }
            break;
        // BLT
        case 0b100:
            if (static_cast<riscv::word_t>(registerFile->read(rs1)) < static_cast<riscv::word_t>(registerFile->read(rs2))) {
                pc += decodedInstr.immediate - 4;
            }
            break;
        // BGE
        case 0b101:
            if (static_cast<riscv::word_t>(registerFile->read(rs1)) >= static_cast<riscv::word_t>(registerFile->read(rs2))) {
                pc += decodedInstr.immediate - 4;
            }
            break;
        default:
            throw ExecuteException("executeBType failed, unknown branch decoded from funct3="
            + std::to_string(decodedInstr.funct3));
    }
}

void Cpu::executeUType(const DecodedInstr &decodedInstr) {
    switch (decodedInstr.opcode) {
        // LUI
        case 0x37:
            registerFile->write(rd, decodedInstr.immediate);
            break;
        // AUIPC
        case 0x17:
            registerFile->write(rd, pc + decodedInstr.immediate);
            break;
        default:
            throw ExecuteException("executeUType failed, unknown op-code. opcode=" + std::to_string(decodedInstr.opcode));
    }
}

void Cpu::executeJType(const DecodedInstr &decodedInstr) {
    switch (decodedInstr.opcode) {
        // JAL
        case 0x6F:
            registerFile->write(rd, pc + 4);
            pc += decodedInstr.immediate - 4;  // -4 because PC already incremented in step()
            break;
        default:
            throw ExecuteException("executeJType failed, unknown op-code. opcode=" + std::to_string(decodedInstr.opcode));
    }
}

void Cpu::executeRType(const DecodedInstr &decodedInstr) {
    riscv::word_t rs1_val = registerFile->read(rs1);
    riscv::word_t rs2_val = registerFile->read(rs2);
    riscv::word_t result = 0;
    switch (decodedInstr.funct3) {
        // ADD / SUB
        case 0b000:
            if (decodedInstr.funct7 == 0) {
                // ADD
                result = rs1_val + rs2_val;
            } else if (decodedInstr.funct7 == 0x20) {
                // SUB
                result = rs1_val - rs2_val;
            }
            break;
        // SLL
        case 0b001:
            result = rs1_val << (rs2_val & 0x1F);
            break;
        // SLT
        case 0b010:
            result = (static_cast<riscv::word_t>(rs1_val) < static_cast<riscv::word_t>(rs2_val)) ? 1 : 0;
            break;
        // SLTU
        case 0b011:
            result = (rs1_val < rs2_val) ? 1 : 0;
            break;
        // XOR
        case 0b100:
            result = rs1_val ^ rs2_val;
            break;
        // OR
        case 0b110:
            result = rs1_val | rs2_val;
            break;
        // AND
        case 0b111:
            result = rs1_val & rs2_val;
            break;
        default:
            throw ExecuteException("executeRType failed, unknown operation decoded from funct3="
            + std::to_string(decodedInstr.funct3));
    }
    registerFile->write(rd, result);
}
