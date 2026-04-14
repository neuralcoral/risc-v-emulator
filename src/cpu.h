#ifndef RISC_V_EMULATOR_CPU_H
#define RISC_V_EMULATOR_CPU_H
#include <cstdint>
#include <array>
#include "instructions/r_type.h"
#include "src/devices/memory.h"

#ifdef DEBUG
    #define LOG(x) std::cout << x << std::endl
#else
    #define LOG(x)
#endif

class Cpu {
    riscv::reg_t pc, rs1, rs2, rd, data;
    riscv::instruction_t  instruction;
public:
    Cpu();
    ~Cpu();

    void fetch(Memory instructions);
    BaseInstruction decode();
    void execute();
    void memoeryAccess();
    void writeBack();
};

#endif  // RISC_V_EMULATOR_CPU_H