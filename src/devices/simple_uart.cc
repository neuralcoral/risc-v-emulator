#include "simple_uart.h"

#include <iostream>

void SimpleUART::write(const riscv::mem_addr_t &addr, const riscv::ubyte_t &value) {
    if (addr == DATA_ADDR) {
        out.put((char) value);
        out.flush();
    }
}

riscv::ubyte_t SimpleUART::read(const riscv::mem_addr_t &addr) {
    if (addr == DATA_ADDR and not rx.empty()) {
        const riscv::ubyte_t val = rx.front();
        rx.pop();
        return val;
    }
    if (addr == STATUS_ADDR) {
        return rx.empty() ? LINE_EMPTY : LINE_READY;
    }
    return 0;
}

void SimpleUART::update() {
    while (in.peek() != EOF and rx.size() < RX_BUFFER_SZ) {
        rx.push(in.get());
    }
}

bool SimpleUART::is_active() {
    return true;
}
