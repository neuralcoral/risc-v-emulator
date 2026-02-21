#ifndef RISC_V_EMULATOR_I_TYPE_H
#define RISC_V_EMULATOR_I_TYPE_H

#include "base_instruction.h"

class IType : public BaseInstruction {
public:
    using BaseInstruction::BaseInstruction;

    constexpr riscv::address_t getRd() { return (instruction >> 7) & REGISTER_BITMASK; }
    constexpr riscv::address_t getRs1() { return (instruction >> 15) & REGISTER_BITMASK; }
    constexpr riscv::funct_t  getFunct3() { return (instruction >> 12) & FUNCT_3_BITMASK; }
};

#endif //RISC_V_EMULATOR_I_TYPE_H
