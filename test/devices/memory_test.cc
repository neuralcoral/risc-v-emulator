#include <gtest/gtest.h>
#include "src/devices/memory.h"
#include "src/exceptions/memory_exception.h"

class MemoryTest : public ::testing::Test {
protected:
    Memory memory;

    MemoryTest() : memory() {}
};

TEST_F(MemoryTest, WritesCorreclty) {
    riscv::byte_t written_byte;
    for (uint16_t i = 0; i < memory.size(); i++) {
        written_byte = 1 << (i % 8);
        memory.write(i, written_byte);
        EXPECT_EQ(memory.read(i), written_byte);
    }
}

TEST_F(MemoryTest, ThrowsExeceptionWhenWriteOverbounds) {
    EXPECT_THROW(memory.write(4096, 0x42), MemoryException);
}

TEST_F(MemoryTest, ThrowsExeceptionWhenWriteUnderbounds) {
    EXPECT_THROW(memory.write(-1, 0xBA), MemoryException);
}

TEST_F(MemoryTest, ThrowsExeceptionWhenReadOverbounds) {
    EXPECT_THROW(memory.read(4096), MemoryException);
}

TEST_F(MemoryTest, ThrowsExeceptionWhenReadUnderbounds) {
    EXPECT_THROW(memory.read(-1), MemoryException);
}

