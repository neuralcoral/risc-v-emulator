#ifndef RISC_V_EMULATOR_IMM_GEN_H
#define RISC_V_EMULATOR_IMM_GEN_H
#include "src/common/types.h"
#include "src/instructions/base_instruction.h"


class ImmGen
{
    const riscv::word_t getBImm(const riscv::instruction_t& ins) const;
    const riscv::word_t getIImm(const riscv::instruction_t& ins) const;
    const riscv::word_t getJImm(const riscv::instruction_t& ins) const;
    const riscv::word_t getSImm(const riscv::instruction_t& ins) const;
    const riscv::word_t getUImm(const riscv::instruction_t& ins) const;
public:
    const riscv::word_t generate(const BaseInstruction& instruction) const;
    const riscv::word_t generate(const riscv::instruction_t& instruction) const;
};


#endif //RISC_V_EMULATOR_IMM_GEN_H