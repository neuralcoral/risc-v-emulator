#ifndef RISC_V_EMULATOR_J_TYPE_H
#define RISC_V_EMULATOR_J_TYPE_H
#include "base_instruction.h"

class JType : public BaseInstruction {
private:
    constexpr uint8_t getImmMSB() { return (instruction >> 31) & 0b1; }
    constexpr uint8_t getImmBit11() { return (instruction >> 20) & 0b1; }
    constexpr uint32_t getImmHi() {return (instruction >> 12) & 0xFF; }
    constexpr uint32_t getImmLo() { return (instruction >> 21) & 0x3FF; }
public:
    using BaseInstruction::BaseInstruction;
    constexpr uint32_t getImm() { return (
                (getImmMSB() << 20) |
                (getImmHi() << 12) |
                (getImmBit11() << 11) |
                (getImmLo() << 1)
            );
    }
    constexpr REGISTER_ADDRESS getRd() { return (instruction >> 7) & REGISTER_BITMASK; }
};

#endif //RISC_V_EMULATOR_J_TYPE_H
