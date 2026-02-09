#ifndef RISC_V_EMULATOR_UART_H
#define RISC_V_EMULATOR_UART_H


#include "src/devices/device.h"

class UART: public Device{
public:
    void write(const riscv::mem_addr_t &addr, const riscv::byte_t &value) override;
    riscv::byte_t read(const riscv::mem_addr_t &addr) const override;
};


#endif //RISC_V_EMULATOR_UART_H
