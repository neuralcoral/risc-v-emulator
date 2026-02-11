#ifndef RISC_V_EMULATOR_BUS_H
#define RISC_V_EMULATOR_BUS_H

#include "src/common/types.h"
#include <array>

#include "src/common/constants.h"
#include "src/devices/memory.h"
#include "src/devices/simple_uart.h"
#include <unordered_map>
#include <functional>

namespace {
    enum DeviceType {
        MEMORY,
        UART,
        UNDEFINED
    };
}

class Bus {
private:
    std::unordered_map<DeviceType, Device*> device_map;


    DeviceType routeToDevice(const riscv::mem_addr_t& addr) const;

public:
    explicit Bus(Memory& memory, SimpleUART& uart);
    void write(const riscv::mem_addr_t& addr, const riscv::word_t& value);
    riscv::word_t read(const riscv::mem_addr_t& addr);
};

#endif //RISC_V_EMULATOR_BUS_H