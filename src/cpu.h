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
   public:
    Cpu();
    ~Cpu();
};

#endif  // RISC_V_EMULATOR_CPU_H