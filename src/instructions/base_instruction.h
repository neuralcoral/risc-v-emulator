#ifndef RISC_V_EMULATOR_OPERATIONS_H
#define RISC_V_EMULATOR_OPERATIONS_H
#include <cstdint>
typedef uint32_t INSTRUCTION ;
typedef uint8_t OPCODE;
typedef uint8_t FUNCT_7;
typedef uint8_t REGISTER_ADDRESS;
typedef uint8_t FUNCT_3;

class BaseInstruction {
protected:
    INSTRUCTION  instruction;
    static constexpr REGISTER_ADDRESS REGISTER_BITMASK = 0x1F;
    static constexpr FUNCT_7 FUNCT_7_BITMASK = 0x7F;
    static constexpr FUNCT_3 FUNCT_3_BITMASK = 0x7;
public:
    BaseInstruction(const INSTRUCTION&);
    constexpr OPCODE getOpCode() { return instruction & 0x7F; }
    constexpr INSTRUCTION  getInstruction() { return instruction; }
};


#endif //RISC_V_EMULATOR_OPERATIONS_H