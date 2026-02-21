#ifndef RISC_V_EMULATOR_OPERATIONS_H
#define RISC_V_EMULATOR_OPERATIONS_H
#include "src/common/types.h"

class BaseInstruction {
protected:
    riscv::instruction_t instruction;
    static constexpr riscv::address_t REGISTER_BITMASK = 0x1F;
    static constexpr riscv::funct_t FUNCT_7_BITMASK = 0x7F;
    static constexpr riscv::funct_t FUNCT_3_BITMASK = 0x7;
public:
    explicit BaseInstruction(const riscv::instruction_t& ins);
    // Bits [1:0] are always 11, so we shift right
    constexpr riscv::opcode_t getOpCode() const { return (instruction & 0x7F); }
    constexpr riscv::instruction_t getInstruction() const { return instruction; }
};


#endif //RISC_V_EMULATOR_OPERATIONS_H