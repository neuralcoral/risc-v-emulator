#ifndef RISC_V_EMULATOR_TYPES_H
#define RISC_V_EMULATOR_TYPES_H
#include <cstdint>

namespace riscv {
    using byte_t = uint8_t;
    using word_t = uint32_t;
    using double_word_t = uint64_t;
    using address_t = uint8_t;
    using reg_t = uint32_t;
    using mem_addr_t = uint32_t;
}

#endif //RISC_V_EMULATOR_TYPES_H
