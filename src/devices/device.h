#ifndef RISC_V_EMULATOR_DEVICE_H
#define RISC_V_EMULATOR_DEVICE_H

#include "src/common/types.h"


class Device
{
public:
    virtual ~Device() = default;
    virtual riscv::byte_t read(const riscv::mem_addr_t& addr) const = 0;
    virtual void write(const riscv::mem_addr_t& addr, const riscv::byte_t& value) = 0;
    // Size of the device's memory space in bytes
    virtual riscv::double_word_t  size() const = 0;
};

#endif //RISC_V_EMULATOR_DEVICE_H