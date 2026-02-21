#include "imm_gen.h"

#include "src/instructions/b_type.h"

const riscv::word_t ImmGen::getBImm(const riscv::instruction_t& ins) const {
    const int32_t imm_msb = static_cast<int32_t> ((ins) & (1u << 31)) >> 19; // 19 = 31 (current bit) - 12 (goal bit)
    const uint16_t imm_eleventh_bit = (ins & (1u << 7))  << 4; // -4 = 7 (current bit) - 11 (goal bit), negative means shift left
    const uint32_t imm_hi = (ins & (0x3Fu << 25)) >> 20; // 20 = 25 (current bit) - 5 (goal bit)
    const uint8_t imm_lo = (ins & (0xF << 8)) >> 7; // 7 = 8 (current bit) - 1 (goal bit)
    return imm_msb |
           imm_eleventh_bit |
           imm_hi |
           imm_lo;
}

const riscv::word_t ImmGen::getIImm(const riscv::instruction_t& ins) const {
    return static_cast<int32_t> (ins) >> 20;
}

const riscv::word_t ImmGen::getJImm(const riscv::instruction_t& ins) const {
    // Shortcut to sign-extend the Most Significant Bit (MSB)
    const int32_t imm_msb = static_cast<int32_t> (ins & (1u << 31)) >> 11; // 11 = 31 (current bit) - 20 (goal bit)
    const uint32_t imm_bit_11 = (ins & (1u << 20)) >> 9; // 9 = 20 (current bit) - 11 (goal bit)
    const uint32_t imm_hi = (ins & (0xFFu << 12)); // 0 = 12 (current bit) - 12 (goal bit)
    const uint32_t imm_lo = (ins & (0x3FFu << 21)) >> 20; // 20 = 21 (current bit) - 1 (goal bit)

    return imm_msb |
           imm_hi |
           imm_bit_11 |
           imm_lo;
}


const riscv::word_t ImmGen::getSImm(const riscv::instruction_t& ins) const {
    const int32_t imm_hi = static_cast<int32_t> (ins & (0xFEu << 24)) >> 20;
    const uint8_t imm_lo = ins >> 7 & 0x1F;
    return imm_hi | imm_lo;
}

const riscv::word_t ImmGen::getUImm(const riscv::instruction_t& ins) const {
    return ins & 0xFFFFF000;
}

const riscv::word_t ImmGen::generate(const BaseInstruction& instruction) const {
    return generate(instruction.getInstruction());
}

const riscv::word_t ImmGen::generate(const riscv::instruction_t &inst) const {
    /**
     * bits [6:5] (rows) x bits [4:2] (columns)
     *
     *     | 000    | 001      | 010      | 011      | 100    | 101   | 110       | 111
     * ----+--------+----------+----------+----------+--------+-------+-----------+---------
     * 00  | LOAD   | LOAD-FP  | custom-0 | MISC-MEM | OP-IMM | AUIPC | OP-IMM-32 | 48b
     * 01  | STORE  | STORE-FP | custom-1 | AMO      | OP     | LUI   | OP-32     | 64b
     * 10  | MADD   | MSUB     | NMSUB    | NMADD    | OP-FP  | OP-V  | custom-2  | 48b
     * 11  | BRANCH | JALR     | reserved | JAL      | SYSTEM | OP-VE | custom-3  | >=80b
     *
     * Notes:
     * - For RV32I, you primarily care about columns 000, 011, 100, and 101.
     **/
     // The first two bits are always 11 in RISC-V
    const riscv::opcode_t opcode = (inst >> 2) & 0x1F;
    switch (opcode) {
        // I-Type: LOAD (0x00), OP-IMM (0x04), JALR (0x19)
        case 0x00: case 0x04: case 0x19:
            return getIImm(inst);
        // S-Type: STORE (0x08)
        case 0x08:
            return getSImm(inst);
        // B-Type: BRANCH (0x18)
        case 0x18:
            return getBImm(inst);
        // U-Type: AUIPC (0x05), LUI (0x0D)
        case 0x05:
        case 0x0D:
            return getUImm(inst);

            // J-Type: JAL (0x1B)
        case 0x1B:
            return getJImm(inst);

            // R-Type or Unimplemented
        default:
            return 0;
    }
}
