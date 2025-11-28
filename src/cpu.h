//
// Created by George Alfaro on 11/24/25.
//

#ifndef RISC_V_EMULATOR_CPU_H
#define RISC_V_EMULATOR_CPU_H
#include <cstdint>

class Cpu {
   private:
    static constexpr int NUM_OF_REGISTERS = 32;
    int programCounter;
    uint64_t* registers;

   public:
    Cpu();
    ~Cpu();
};

#endif  // RISC_V_EMULATOR_CPU_H