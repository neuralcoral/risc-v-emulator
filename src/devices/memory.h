#ifndef RISC_V_EMULATOR_MEMORY_H
#define RISC_V_EMULATOR_MEMORY_H
#include "device.h"
#include <array>
#include "src/common/constants.h"


class Memory : public Device
{
private:
    std::array<riscv::byte_t, riscv::RAM_SIZE> memory {};
    public:
        riscv::byte_t read(const riscv::mem_addr_t& addr) override;
        void write(const riscv::mem_addr_t& addr, const riscv::byte_t& value) override;
        riscv::mem_addr_t size() const override { return riscv::RAM_SIZE; };
};


#endif //RISC_V_EMULATOR_MEMORY_H