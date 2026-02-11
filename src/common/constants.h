#ifndef RISC_V_EMULATOR_CONSTANTS_H
#define RISC_V_EMULATOR_CONSTANTS_H
#include "types.h"

namespace riscv {
    constexpr word_t RAM_SIZE {1 << 12 };
    constexpr uint8_t NUM_OF_REGISTERS { 32 };
    constexpr word_t RAM_BASE { 0x80000000 };
    constexpr word_t RAM_UPPER_LIMIT { RAM_BASE + RAM_SIZE };
    constexpr word_t UART_SIZE { 0x100 };
    constexpr word_t UART_BASE { 0x10000000 };
    constexpr word_t UART_UPPER_LIMIT { UART_BASE + UART_SIZE };
}

#endif //RISC_V_EMULATOR_CONSTANTS_H