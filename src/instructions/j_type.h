#ifndef RISC_V_EMULATOR_J_TYPE_H
#define RISC_V_EMULATOR_J_TYPE_H
#include "base_instruction.h"

class JType : public BaseInstruction {
public:
    using BaseInstruction::BaseInstruction;
    constexpr riscv::address_t getRd() { return (instruction >> 7) & REGISTER_BITMASK; }
};

#endif //RISC_V_EMULATOR_J_TYPE_H
