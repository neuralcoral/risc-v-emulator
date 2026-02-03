#include <gtest/gtest.h>
#include "src/register_file.h"
#include "src/exceptions/register_file_exception.h"

class RegisterFileTest : public ::testing::Test {
protected:
    RegisterFile rf;

    void SetUp() override {
        rf.reset();
    }
};

TEST_F(RegisterFileTest, X0InitiallyZero) {
    EXPECT_EQ(rf.read(0), 0);
}

TEST_F(RegisterFileTest, X0IgnoresWrites) {
    rf.write(0, 0xFFFFFFFF);
    EXPECT_EQ(rf.read(0), 0);
}

TEST_F(RegisterFileTest, BasicWriteReadX1) {
    uint32_t test_val = 0xDEADBEEF;
    rf.write(1, test_val);
    EXPECT_EQ(rf.read(1), test_val);
}

TEST_F(RegisterFileTest, UpperBoundaryWriteReadX31) {
    uint32_t test_val = 0xCAFEBABE;
    rf.write(31, test_val);
    EXPECT_EQ(rf.read(31), test_val);
}

TEST_F(RegisterFileTest, WritingX5DoesNotAffectX6) {
    rf.write(5, 0x12345678);
    rf.write(6, 0x87654321);
    EXPECT_EQ(rf.read(5), 0x12345678);
}

TEST_F(RegisterFileTest, ResetClearsRegisters) {
    rf.write(10, 0x11111111);
    rf.reset();
    EXPECT_EQ(rf.read(10), 0);
}

TEST_F(RegisterFileTest, WritingOutOfBoundsThrowsAnException) {
    EXPECT_THROW(rf.write(32, 0x01), RegisterFileException);
}

TEST_F(RegisterFileTest, ReadingOutOfBoundsThrowsAnException) {
    EXPECT_THROW(rf.read(-1), RegisterFileException);
}
