#include "memory.h"


riscv::byte_t Memory::read(const riscv::mem_addr_t& addr) const {
    if (addr < 0 or addr >= memory.size()) {

    }

    return memory[addr];
}

void Memory::write(const riscv::mem_addr_t& addr, const riscv::byte_t& value) {
    if (addr < 0 or addr >= memory.size()) {
        // Throw exception
    }
    memory[addr] = value;
}

void Memory::write(const riscv::mem_addr_t& addr, const riscv::word_t& value) {
    if (addr < 0 or addr >= memory.size()) {
        // Throw exception
    }
    if (value % 8 != 0) {
        // Throw exception
    }
    memory[addr] = value;
}
