#include <gtest/gtest.h>
#include "src/devices/simple_uart.h"
#include "src/devices/memory.h"
#include "src/bus.h"

class BusTest : public ::testing::Test {
protected:
    std::stringstream input, output;
    SimpleUART uart;
    Memory ram;
    Bus bus;

    BusTest() : uart(input, output), ram(), bus() {
        bus.register_device(&ram, riscv::RAM_OFFSET);
        bus.register_device(&uart, riscv::UART_OFFSET);
    }
};

TEST_F(BusTest, CorrectlyReadsFromRam) {
    ram.write(0xFFF, 0xAF);
    EXPECT_EQ(bus.read(riscv::RAM_UPPER_LIMIT - 1), 0xAF);
}

TEST_F(BusTest, CorrectlyWritesToRam) {
    bus.write(riscv::RAM_OFFSET, 0xCA);
    EXPECT_EQ(ram.read(0), 0xCA);
}

TEST_F(BusTest, CorrectlyReadsFromUart) {
    input.put('D');
    bus.update();
    EXPECT_EQ(bus.read(SimpleUART::DATA_ADDR + riscv::UART_OFFSET), 'D');
}

TEST_F(BusTest, CorrectlyWritesToUart) {
    bus.write(riscv::UART_OFFSET + SimpleUART::DATA_ADDR, 'Q');
    EXPECT_EQ(output.str(), "Q");
}

TEST_F(BusTest, ThrowsBusExceptionWhenReadingFromUnassingedAddr) {
    EXPECT_THROW(bus.read(0), BusException);
}

TEST_F(BusTest, ThrowsBusExceptionWhenWritingToUnassingedAddr) {
    EXPECT_THROW(bus.write(0, 'A'), BusException);
}

TEST_F(BusTest, ThrowsBusExceptionWhenRegisteringDeviceOverUsedAddress) {
    EXPECT_THROW(bus.register_device(new Memory(), riscv::UART_OFFSET), BusException);
}

