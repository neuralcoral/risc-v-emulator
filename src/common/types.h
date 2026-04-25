#ifndef RISC_V_EMULATOR_TYPES_H
#define RISC_V_EMULATOR_TYPES_H
#include <cstdint>

namespace riscv {
    using ubyte_t = uint8_t;
    using byte_t = int8_t;
    using uword_t = uint32_t;
    using word_t = uint32_t;
    using udouble_word_t = uint64_t;
    using double_word_t = int64_t;
    using address_t = uint8_t;
    using reg_t = uint32_t;
    using mem_addr_t = uint32_t;
    using instruction_t = uint32_t;
    using opcode_t = uint8_t;
    using funct_t = uint8_t;
}

#endif //RISC_V_EMULATOR_TYPES_H
