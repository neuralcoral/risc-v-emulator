
#include <gtest/gtest.h>
#include "src/devices/simple_uart.h"

class SimpleUARTTest : public ::testing::Test {
protected:
    std::stringstream fake_in, fake_out;
    SimpleUART uart;

    SimpleUARTTest() : uart(fake_in, fake_out) {}
};

TEST_F(SimpleUARTTest, WritesCorrectly) {
    uart.write(SimpleUART::DATA_ADDR, 0x48);
    EXPECT_EQ(fake_out.str(), "H");
}

TEST_F(SimpleUARTTest, ReadsCorrectly) {
    fake_in.put('A');
    uart.update();
    EXPECT_EQ(uart.read(SimpleUART::DATA_ADDR), 0x41);
}

TEST_F(SimpleUARTTest, EmptyReadsCorrectly) {
    EXPECT_EQ(uart.read(SimpleUART::DATA_ADDR), 0);
}

TEST_F(SimpleUARTTest, ReadsFilledBufferWithStatusCorrectly) {
    fake_in.put('B');
    uart.update();
    EXPECT_EQ(uart.read(SimpleUART::STATUS_ADDR), SimpleUART::LINE_READY);
}

TEST_F(SimpleUARTTest, ReadsEmptyBufferWithStatusCorrectly) {
    EXPECT_EQ(uart.read(SimpleUART::STATUS_ADDR), SimpleUART::LINE_EMPTY);
}

TEST_F(SimpleUARTTest, BufferCapsAtSixteenBytes) {
    for(auto i = 0u; i < SimpleUART::RX_BUFFER_SZ + 5; i++) {
        fake_in.put('A');
    }

    uart.update();

    for(int i = 0u; i < SimpleUART::RX_BUFFER_SZ; i++) {
        uart.read(SimpleUART::DATA_ADDR);
    }

    EXPECT_EQ(uart.read(SimpleUART::STATUS_ADDR), SimpleUART::LINE_EMPTY);
}

TEST_F(SimpleUARTTest, BufferPreservesFIFOOrderOnOverflow) {
    for(auto i = 0u; i < SimpleUART::RX_BUFFER_SZ + 5; i++) {
        fake_in.put(char('A' + i));
    }

    uart.update();

    for(int i = 0u; i < SimpleUART::RX_BUFFER_SZ - 1; i++) {
        uart.read(SimpleUART::DATA_ADDR);
    }

    EXPECT_EQ(uart.read(SimpleUART::DATA_ADDR), 'A' + (SimpleUART::RX_BUFFER_SZ) - 1);
}

TEST_F(SimpleUARTTest, UpdateDoesNotHangIfRxLessThanBuffer) {
    fake_in.put('C');
    uart.update();
    EXPECT_EQ(uart.read(SimpleUART::DATA_ADDR), 'C');
}