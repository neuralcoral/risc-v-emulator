#include "memory.h"
#include "src/exceptions/memory_exception.h"


riscv::byte_t Memory::read(const riscv::mem_addr_t& addr) {
    if (addr < 0 or addr >= size()) {
        throw MemoryException("Failed Memory::read. `addr` is out of bounds");
    }

    return memory[addr];
}

void Memory::write(const riscv::mem_addr_t& addr, const riscv::byte_t& value) {
    if (addr < 0 or addr >= size()) {
        throw MemoryException("Failed Memory::write. `addr` is out of bounds");
    }
    memory[addr] = value;
}

riscv::double_word_t Memory::size() const {
    return riscv::RAM_SIZE;
}
