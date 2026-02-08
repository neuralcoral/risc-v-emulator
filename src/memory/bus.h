#ifndef RISC_V_EMULATOR_BUS_H
#define RISC_V_EMULATOR_BUS_H

#include "src/common/types.h"
#include <array>

#include "src/common/constants.h"

class Bus {
private:
    std::array<riscv::word_t, riscv::MEMORY_SIZE> memory_m {};

public:
    explicit Bus();
    void write(const riscv::mem_addr_t& addr, const riscv::word_t& value);
    riscv::word_t read(const riscv::mem_addr_t& addr) const;
};

#endif //RISC_V_EMULATOR_BUS_H