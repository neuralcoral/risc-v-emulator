#ifndef RISC_V_EMULATOR_B_TYPE_H
#define RISC_V_EMULATOR_B_TYPE_H

#include "base_instruction.h"

class BType : public BaseInstruction {
    /**
     * Documentation:
     * 31   | 30 - 25   | 24 - 20 | 19 -15 | 14 - 12 | 11 - 8 | 7 | 6 - 0
     * [12] | imm[10:5] | rs2 | rs1 | funct3 | imm[4:1] | [11] | opcode
     */
private:
    constexpr uint8_t getImmLo() { return (instruction >> 8) & 0xF; }
    constexpr uint16_t getImmHi() { return (instruction >> 25) & 0x3F; }
public:
    using BaseInstruction::BaseInstruction;

    constexpr REGISTER_ADDRESS getRs1() { return (instruction >> 15) & REGISTER_BITMASK; }
    constexpr REGISTER_ADDRESS getRs2() { return (instruction >> 20) & REGISTER_BITMASK; }
    constexpr FUNCT_3  getFunct3() {return (instruction >> 12) & FUNCT_3_BITMASK; }
    constexpr uint16_t getImm() {
        return (
                ((instruction >> 31) & 0b1) << 13 |
                ((instruction >> 7) & 0b1) << 12 |
                (getImmHi() << 6) |
                (getImmLo() << 1)
        );
    }
};


#endif //RISC_V_EMULATOR_B_TYPE_H
