#include "bus.h"


void Bus::write(const riscv::mem_addr_t& addr, const riscv::word_t& value) {
    const DeviceType device_type = routeToDevice(addr);
    if (device_type == UNDEFINED) {
        throw;
    }
    return device_map[device_type]->write(addr, value);
}

riscv::word_t Bus::read(const riscv::mem_addr_t& addr) {
    const DeviceType device_type = routeToDevice(addr);
    if (device_type == UNDEFINED) {
        throw;
    }
    return device_map[device_type]->read(addr);
}

DeviceType Bus::routeToDevice(const riscv::mem_addr_t &addr) const {
    if (addr >= riscv::RAM_BASE and addr <= riscv::RAM_UPPER_LIMIT) {
        return MEMORY;
    } else if (addr >= riscv::UART_BASE and addr <= (riscv::UART_UPPER_LIMIT)) {
        return UART;
    }
    return UNDEFINED;
}

Bus::Bus(Memory& memory, SimpleUART& uart) {
    device_map[MEMORY] = &memory;
    device_map[UART] = &uart;
}
