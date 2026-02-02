#ifndef RISC_V_EMULATOR_S_TYPE_H
#define RISC_V_EMULATOR_S_TYPE_H

#include "base_instruction.h"

class SType : public BaseInstruction {
private:
    constexpr uint8_t getImmLo() { return (instruction >> 7) & 0x1F; }
    constexpr uint16_t getImmHi() { return (instruction >> 25) & 0x3F; }

public:
    using BaseInstruction::BaseInstruction;

    constexpr FUNCT_3  getFunct3() { return (instruction >> 12) & FUNCT_3_BITMASK; }
    constexpr REGISTER_ADDRESS getRs1() { return (instruction >> 15) & REGISTER_BITMASK; }
    constexpr REGISTER_ADDRESS getRs2() { return (instruction >> 20) & REGISTER_BITMASK; }
    // IMM_HI (16 bit) = ____ ____ _111 1001
    // IMM_LO (8 bit) = ___0 1010
    // IMM = 1111 0010 1010
    // (IMM_HI) << 9 | IMM_LO
    constexpr uint16_t getImm() { return (getImmHi() << 5) | getImmLo(); }
};

#endif //RISC_V_EMULATOR_S_TYPE_H
