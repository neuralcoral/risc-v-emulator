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
: memory(memory), registerFile(registerFile) {
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
    const riscv::instruction_t byte0 = memory.read(pc);
    const riscv::instruction_t byte1 = memory.read(pc + 1);
    const riscv::instruction_t byte2 = memory.read(pc + 2);
    const riscv::instruction_t byte3 = memory.read(pc + 3);
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
        case 0x00:
            handleLoads(decodedInstr);
            break;
        // OP-IMM
        case 0x04:
            handleOpImm(decodedInstr);
            break;
        // Jump and Link Register
        case 0x19:
            handleJumpAndLinkRegister(decodedInstr);
            break;
        default:
            throw ExecuteException("executeIType failed, unknown op-code. opcode=" + std::to_string(decodedInstr.opcode));
    }
}

void Cpu::handleLoads(const DecodedInstr &decodedInstr) {
    const riscv::mem_addr_t memAddr = registerFile.read(rs1) + decodedInstr.immediate;
    riscv::uword_t finalValue = 0;
    riscv::word_t  signedTemp = 0;
    riscv::ubyte_t significantByte;
    switch (decodedInstr.funct3) {
        // LB
        case 0b000:
            significantByte = (riscv::byte_t) memory.read(memAddr);

            signedTemp = static_cast<riscv::word_t>(significantByte);
            registerFile.write(rd, (signedTemp << 24) >> 24);
            break;
        // LH
        case 0b001:
            significantByte = (riscv::byte_t) memory.read(memAddr + 1);

            finalValue |= memory.read(memAddr);
            finalValue |= (significantByte << 8);
            signedTemp = static_cast<riscv::word_t>(finalValue);
            registerFile.write(rd, ((signedTemp << 16) >> 16));
            break;
        // LW
        case 0b010:
            finalValue |= memory.read(memAddr);
            finalValue |= (memory.read(memAddr + 1) << 8);
            finalValue |= (memory.read(memAddr + 2) << 16);
            finalValue |= (memory.read(memAddr + 3) << 24);
            registerFile.write(rd, finalValue);
            break;
        // LBU
        case 0b100:
            finalValue |= memory.read(memAddr);
            registerFile.write(rd, finalValue);
            break;
        // LHU
        case 0b101:
            finalValue |= memory.read(memAddr);
            finalValue |= memory.read(memAddr + 1) << 8;
            registerFile.write(rd, finalValue);
            break;
        default:
            throw ExecuteException("Load Operation failed, unknown load decoded from funct3="
            + std::to_string(decodedInstr.funct3));
    }
}

void Cpu::handleOpImm(const DecodedInstr &decodedInstr) {
    riscv::word_t rs1_val = registerFile.read(rs1);
    riscv::word_t imm = decodedInstr.immediate;
    switch(decodedInstr.funct3) {
        //ADDI
        case 0b000:
            registerFile.write(rd, imm + rs1_val);
            break;
        // SLTI
        case 0b010:
            registerFile.write(rd, (rs1_val < imm) ? 1 : 0);
            break;
        // SLTIU
        case 0b011:
            registerFile.write(rd,
                               (static_cast<riscv::uword_t>(rs1_val) < (static_cast<riscv::uword_t>(imm))
                               ? 1 : 0));
            break;
        // XORI
        case 0b100:
            registerFile.write(rd, imm ^ rs1_val);
            break;
        //ORI
        case 0b110:
            registerFile.write(rd, imm | rs1_val);
            break;
        // ANDI
        case 0b111:
            registerFile.write(rd, imm & rs1_val);
            break;
        // SLLI (Shift Left Logical Immediate)
        case 0b001: {
            riscv::uword_t shift_amount = imm & 0x1F;
            registerFile.write(rd, static_cast<riscv::uword_t>(rs1_val) << shift_amount);
            break;
        }
        // SRLI and SRAI (Shift Right Logical/Arithmetic Immediate)
        case 0b101: {
            riscv::uword_t shift_amount = imm & 0x1F;
            bool isArithmetic = (imm & 0x400) != 0;

            if (isArithmetic) {
                // SRAI: Arithmetic Shift (rs1_val is already a signed riscv::word_t)
                registerFile.write(decodedInstr.rd, rs1_val >> shift_amount);
            } else {
                // SRLI: Logical Shift
                // We cast to unsigned so C++ pulls in 0s from the left.
                registerFile.write(decodedInstr.rd, static_cast<riscv::uword_t>(rs1_val) >> shift_amount);
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
    riscv::word_t rs1_val = registerFile.read(rs1);
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
            registerFile.write(rd, return_pc);
            break;
        default:
            throw ExecuteException("JALR Operation failed, unknown JALR decoded from funct3="
                                   + std::to_string(decodedInstr.funct3));
    }
}

void Cpu::executeSType(const DecodedInstr &decodedInstr) {

}

void Cpu::executeBType(const DecodedInstr &decodedInstr) {

}

void Cpu::executeUType(const DecodedInstr &decodedInstr) {

}

void Cpu::executeJType(const DecodedInstr &decodedInstr) {

}

void Cpu::executeRType(const DecodedInstr &decodedInstr) {

}
