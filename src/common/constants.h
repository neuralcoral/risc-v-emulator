#ifndef RISC_V_EMULATOR_CONSTANTS_H
#define RISC_V_EMULATOR_CONSTANTS_H
#include "types.h"

namespace riscv {
    constexpr word_t MEMORY_SIZE { 1 << 12 };
    constexpr uint8_t NUM_OF_REGISTERS { 32 };
}

#endif //RISC_V_EMULATOR_CONSTANTS_H