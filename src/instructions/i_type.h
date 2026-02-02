#ifndef RISC_V_EMULATOR_I_TYPE_H
#define RISC_V_EMULATOR_I_TYPE_H

#include "base_instruction.h"

class IType : public BaseInstruction {
public:
    using BaseInstruction::BaseInstruction;

    constexpr REGISTER_ADDRESS getRd() { return (instruction >> 7) & REGISTER_BITMASK; }
    constexpr REGISTER_ADDRESS getRs1() { return (instruction >> 15) & REGISTER_BITMASK; }
    constexpr FUNCT_3  getFunct3() { return (instruction >> 12) & FUNCT_3_BITMASK; }
    constexpr uint16_t getImm() {return (instruction >> 20) & 0x0FFF; }

};


#endif //RISC_V_EMULATOR_I_TYPE_H
