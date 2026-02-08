#include "bus.h"

void Bus::write(const riscv::mem_addr_t& addr, const riscv::word_t& value) {

}

riscv::word_t Bus::read(const riscv::mem_addr_t& addr) const {
    return memory_m[addr];
}