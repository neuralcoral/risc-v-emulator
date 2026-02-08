#ifndef RISC_V_EMULATOR_DEVICE_H
#define RISC_V_EMULATOR_DEVICE_H
#include <cstdint>

#include "src/common/types.h"


class Device
{
    public:
    virtual ~Device() = default;
    virtual riscv::byte_t read(const riscv::mem_addr_t& addr) const;
    virtual void write(const riscv::mem_addr_t& addr, const riscv::byte_t& value);
};


#endif //RISC_V_EMULATOR_DEVICE_H