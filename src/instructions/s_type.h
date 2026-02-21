#ifndef RISC_V_EMULATOR_S_TYPE_H
#define RISC_V_EMULATOR_S_TYPE_H

#include "base_instruction.h"

class SType : public BaseInstruction {
public:
    using BaseInstruction::BaseInstruction;

    constexpr riscv::funct_t  getFunct3() { return (instruction >> 12) & FUNCT_3_BITMASK; }
    constexpr riscv::address_t getRs1() { return (instruction >> 15) & REGISTER_BITMASK; }
    constexpr riscv::address_t getRs2() { return (instruction >> 20) & REGISTER_BITMASK; }
};

#endif //RISC_V_EMULATOR_S_TYPE_H
