//
// Created by George Alfaro on 11/24/25.
//

#ifndef RISC_V_EMULATOR_CPU_H
#define RISC_V_EMULATOR_CPU_H
#include <cstdint>
#include <array>
#include "instructions/r_type.h"

#ifdef DEBUG
    #define LOG(x) std::cout << x << std::endl
#else
    #define LOG(x)
#endif

class Cpu {
    static constexpr int NUM_OF_REGISTERS { 32 };
    uint32_t programCounter;
    std::array<uint32_t, NUM_OF_REGISTERS> registers;

   public:
    Cpu();
    ~Cpu();
    void reset();
    void execute(INSTRUCTION instruction);
};

#endif  // RISC_V_EMULATOR_CPU_H