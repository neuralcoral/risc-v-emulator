#ifndef RISC_V_EMULATOR_B_TYPE_H
#define RISC_V_EMULATOR_B_TYPE_H

#include "base_instruction.h"

class BType : public BaseInstruction {
    /**
     * Documentation:
     * 31   | 30 - 25   | 24 - 20 | 19 -15 | 14 - 12 | 11 - 8 | 7 | 6 - 0
     * [12] | imm[10:5] | rs2 | rs1 | funct3 | imm[4:1] | [11] | opcode
     */
public:
    using BaseInstruction::BaseInstruction;

    constexpr riscv::address_t getRs1() { return (instruction >> 15) & REGISTER_BITMASK; }
    constexpr riscv::address_t getRs2() { return (instruction >> 20) & REGISTER_BITMASK; }
    constexpr riscv::funct_t  getFunct3() {return (instruction >> 12) & FUNCT_3_BITMASK; }
};


#endif //RISC_V_EMULATOR_B_TYPE_H
