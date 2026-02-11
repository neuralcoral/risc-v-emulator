#ifndef RISC_V_EMULATOR_SIMPLE_UART_H
#define RISC_V_EMULATOR_SIMPLE_UART_H


#include "src/devices/device.h"
#include "src/common/constants.h"
#include <queue>
#include <iosfwd>

class SimpleUART: public Device{
private:
    std::istream& in;
    std::ostream& out;
    std::queue<riscv::byte_t> rx;

public:
    explicit SimpleUART(std::istream& in_stream, std::ostream& out_stream) :in(in_stream), out(out_stream) {}
    riscv::byte_t read(const riscv::mem_addr_t &addr) override;
    void write(const riscv::mem_addr_t &addr, const riscv::byte_t &value) override;
    void update() override;
    bool is_active() override;

    riscv::mem_addr_t size() const override { return riscv::UART_SIZE; }

    static constexpr riscv::mem_addr_t DATA_ADDR { 0 },
            STATUS_ADDR { 5 };
    static constexpr riscv::byte_t LINE_READY { 0x21 },
            LINE_EMPTY { 0x20 };
    static constexpr riscv::double_word_t RX_BUFFER_SZ { 16 };
};


#endif //RISC_V_EMULATOR_SIMPLE_UART_H
