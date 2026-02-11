#include "src/bus.h"

void Bus::write(const riscv::mem_addr_t& addr, const riscv::word_t& value) {
    const DeviceMapping* device_mapping = find_device(addr);
    if (!device_mapping) {
        throw BusException("Failed Bus::write. Bus fault, no device at address.");
    }
    return device_mapping->device->write(addr - device_mapping->offset, value);
}

riscv::word_t Bus::read(const riscv::mem_addr_t& addr) {
    const DeviceMapping* device_mapping = find_device(addr);
    if (!device_mapping) {
        throw BusException("Failed Bus::read. Bus fault, no device at address.");
    }
    return device_mapping->device->read(addr - device_mapping->offset);
}

void Bus::register_device(Device *device, const riscv::mem_addr_t &addr_base) {
    if (detect_collision(addr_base, device->size())) {
        throw BusException("Failed Bus::register_device. Cannot register device, address collision detected.");
    }
    devices.push_back({addr_base, device});
}

bool Bus::detect_collision(const riscv::mem_addr_t& offset, const riscv::mem_addr_t & size) {
    return std::any_of(devices.begin(), devices.end(), [&](const auto& mapping) {
        return (offset < (mapping.offset + mapping.device->size()) and mapping.offset < offset + size);
    });
}

const Bus::DeviceMapping* Bus::find_device(const riscv::mem_addr_t& addr) const {
    for (const auto& device_mapping : devices) {
        if(addr >= device_mapping.offset and addr < (device_mapping.offset + device_mapping.device->size())) {
            return &device_mapping;
        }
    }
    return nullptr;
}

void Bus::update() {
    for (const auto& device_mapping : devices) {
        if (device_mapping.device->is_active()) {
            device_mapping.device->update();
        }
    }
}

