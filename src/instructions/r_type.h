//
// Created by george on 1/31/26.
//

#ifndef RISC_V_EMULATOR_R_TYPE_H
#define RISC_V_EMULATOR_R_TYPE_H

#include "base_instruction.h"
#include <cstdint>

class RType : public BaseInstruction {
public:
    RType(const INSTRUCTION& instruction);

    constexpr REGISTER_ADDRESS getRd() { return (instruction >> 7) & REGISTER_BITMASK; }
    constexpr REGISTER_ADDRESS getRs1() { return (instruction >> 15) & REGISTER_BITMASK; }
    constexpr REGISTER_ADDRESS getRs2() { return (instruction >> 20) & REGISTER_BITMASK; }
    constexpr FUNCT_3  getFunct3() { return (instruction >> 12) & FUNCT_3_BITMASK; }
    constexpr FUNCT_7  getFunct7() { return (instruction >> 25) & FUNCT_7_BITMASK; }
};


#endif //RISC_V_EMULATOR_R_TYPE_H
