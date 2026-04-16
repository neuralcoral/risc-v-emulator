#include "decoder.h"

#include <format>

#include "src/datapath/imm_gen.h"

ImmGen Decoder::immGen;

DecodedInstr Decoder::decode(const riscv::instruction_t &ins) {
    const riscv::opcode_t opcode = (ins >> 2) & 0x1F;

    switch (opcode) {
        case 0x00:
        case 0x04:
        case 0x19:
            return handleIType(ins);
        case 0x08:
            return handleSType(ins);
        case 0x18:
            return handleBType(ins);
        case 0x05:
        case 0x0D:
            return handleUType(ins);
        case 0x1B:
            return handleJType(ins);
        case 0x0C:
            return handleRType(ins);
        default:
            throw DecodeException(std::format("No matching instruction type found for {:#010x}", ins));
    }
}

DecodedInstr Decoder::handleIType(const riscv::instruction_t &ins) {
    const riscv::opcode_t opcode = ins & 0x7F;
    const riscv::reg_t rd = (ins >> 7) & REGISTER_BITMASK;
    const riscv::reg_t rs1 = (ins >> 15) & REGISTER_BITMASK;
    const riscv::reg_t rs2 = NO_REG;
    const riscv::word_t immediate = immGen.generate(ins);
    const riscv::funct_t funct3 = (ins >> 12) & FUNCT_3_BITMASK;
    const riscv::funct_t funct7 = NO_FUNCT;

    return {opcode, rd, rs1, rs2, immediate, funct3, funct7, IType};
}

DecodedInstr Decoder::handleSType(const riscv::instruction_t &ins) {
    const riscv::opcode_t opcode = ins & 0x7F;
    const riscv::reg_t rd = NO_REG;
    const riscv::reg_t rs1 = (ins >> 15) & REGISTER_BITMASK;
    const riscv::reg_t rs2 = (ins >> 20) & REGISTER_BITMASK;
    const riscv::word_t immediate = immGen.generate(ins);
    const riscv::funct_t funct3 = (ins >> 12) & FUNCT_3_BITMASK;
    const riscv::funct_t funct7 = NO_FUNCT;

    return {opcode, rd, rs1, rs2, immediate, funct3, funct7, SType};
}

DecodedInstr Decoder::handleBType(const riscv::instruction_t &ins) {
    const riscv::opcode_t opcode = ins & 0x7F;
    const riscv::reg_t rd = NO_REG;
    const riscv::reg_t rs1 = (ins >> 15) & REGISTER_BITMASK;
    const riscv::reg_t rs2 = (ins >> 20) & REGISTER_BITMASK;
    const riscv::word_t immediate = immGen.generate(ins);
    const riscv::funct_t funct3 = (ins >> 12) & FUNCT_3_BITMASK;
    const riscv::funct_t funct7 = NO_FUNCT;

    return {opcode, rd, rs1, rs2, immediate, funct3, funct7, BType};
}

DecodedInstr Decoder::handleUType(const riscv::instruction_t &ins) {
    const riscv::opcode_t opcode = ins & 0x7F;
    const riscv::reg_t rd = (ins >> 7) & REGISTER_BITMASK;
    const riscv::reg_t rs1 = NO_REG;
    const riscv::reg_t rs2 = NO_REG;
    const riscv::word_t immediate = immGen.generate(ins);
    const riscv::funct_t funct3 = NO_FUNCT;
    const riscv::funct_t funct7 = NO_FUNCT;

    return {opcode, rd, rs1, rs2, immediate, funct3, funct7, UType};
}

DecodedInstr Decoder::handleJType(const riscv::instruction_t &ins) {
    const riscv::opcode_t opcode = ins & 0x7F;
    const riscv::reg_t rd = (ins >> 7) & REGISTER_BITMASK;
    const riscv::reg_t rs1 = NO_REG;
    const riscv::reg_t rs2 = NO_REG;
    const riscv::word_t immediate = immGen.generate(ins);
    const riscv::funct_t funct3 = NO_FUNCT;
    const riscv::funct_t funct7 = NO_FUNCT;

    return {opcode, rd, rs1, rs2, immediate, funct3, funct7, JType};
}

DecodedInstr Decoder::handleRType(const riscv::instruction_t &ins) {
    const riscv::opcode_t opcode = ins & 0x7F;
    const riscv::reg_t rd = (ins >> 7) & REGISTER_BITMASK;
    const riscv::reg_t rs1 = (ins >> 15) & REGISTER_BITMASK;
    const riscv::reg_t rs2 = (ins >> 20) & REGISTER_BITMASK;
    const riscv::word_t immediate = NO_IMM;
    const riscv::funct_t funct3 = (ins >> 12) & FUNCT_3_BITMASK;
    const riscv::funct_t funct7 = (ins >> 25) & FUNCT_7_BITMASK;

    return {opcode, rd, rs1, rs2, immediate, funct3, funct7, RType};
}
