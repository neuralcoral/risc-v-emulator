#ifndef RISC_V_EMULATOR_BUS_H
#define RISC_V_EMULATOR_BUS_H

#include "src/common/types.h"
#include "src/exceptions/bus_exception.h"
#include <array>

#include "src/common/constants.h"
#include "src/devices/memory.h"
#include "src/devices/simple_uart.h"
#include <unordered_map>
#include <functional>

class Bus {
private:
    struct DeviceMapping {
        riscv::mem_addr_t offset;
        Device* device;
    };
    std::vector<DeviceMapping> devices;

    bool detect_collision(const riscv::mem_addr_t& offset, const riscv::mem_addr_t& size);
    const DeviceMapping* find_device(const riscv::mem_addr_t& addr) const;

public:
    explicit Bus() = default;
    void write(const riscv::mem_addr_t& addr, const riscv::word_t& value);
    riscv::word_t read(const riscv::mem_addr_t& addr);
    void register_device(Device *device, const riscv::mem_addr_t &addr_base);
    void update();
};

#endif //RISC_V_EMULATOR_BUS_H