#ifndef RISC_V_EMULATOR_MEMORY_H
#define RISC_V_EMULATOR_MEMORY_H
#include "device.h"
#include <array>
#include "src/common/constants.h"


class Memory : public Device
{
private:
    std::array<riscv::byte_t, riscv::MEMORY_SIZE> memory {};
    public:
        riscv::byte_t read(const riscv::mem_addr_t& addr) const override;
        void write(const riscv::mem_addr_t& addr, const riscv::byte_t& value) override;
        void write(const riscv::mem_addr_t& addr, const riscv::word_t& value);
};


#endif //RISC_V_EMULATOR_MEMORY_H