#ifndef RISC_V_EMULATOR_U_TYPE_H
#define RISC_V_EMULATOR_U_TYPE_H

#include "base_instruction.h"

class UType : public BaseInstruction {
public:
    using BaseInstruction::BaseInstruction;

    constexpr riscv::address_t getRd() { return (instruction >> 7) & 0x1F; }
};

#endif //RISC_V_EMULATOR_U_TYPE_H
