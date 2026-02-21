#include <gtest/gtest.h>
#include "src/datapath/imm_gen.h"
#include "test/test_utils/custom_assertions.h"

class ImmGenTest : public ::testing::Test {
protected:
    ImmGen immGen;

    ImmGenTest() : immGen() { }
};

// --- I-Type Tests ---
TEST_F(ImmGenTest, ITypePositiveImmediate) {
    // addi x1, x0, 16
    EXPECT_IMM_EQ(immGen.generate(0x01000093), 16);
}

TEST_F(ImmGenTest, ITypeNegativeImmediate) {
    // addi x1, x0, -16
    EXPECT_IMM_EQ(immGen.generate(0xff000093), -16);
}

// --- S-Type Tests ---
TEST_F(ImmGenTest, STypePositiveImmediate) {
    // sb x2, 8(x1)
    EXPECT_IMM_EQ(immGen.generate(0x00208423), 8);
}

TEST_F(ImmGenTest, STypeNegativeImmediate) {
    // sw x2, -16(x1)
    EXPECT_IMM_EQ(immGen.generate(0xfe20a823), -16);
}

// --- B-Type Tests ---
TEST_F(ImmGenTest, BTypePositiveImmediate) {
    // beq x0, x0, 8
    EXPECT_IMM_EQ(immGen.generate(0x00000463), 8);
}

TEST_F(ImmGenTest, BTypeNegativeImmediate) {
    /*
     * Manual check:
     * 0xfe000ce3 = 11111110000000000000110011100011
     * imm_msb@[31] = 1 ->  (Maps to all bits up to and including 12)
     * imm_11_bit@[7] = 1 -> (Maps to 11th bit)
     * imm_hi@[30:25] = 111111-> (Maps to bits 10: 5)
     * imm_lo@[11:8] = 1100-> Maps to bits 4:1
     * Bit 0 is always 0
     *
     * 11111111111111111111 1 111111 1100 0
     * = -8 (2s complement)
     */
    // Remember, branches multiply the extracted value by 2 implicitly
    // beq x0, x0, -8
    EXPECT_IMM_EQ(immGen.generate(0xfe000ce3), -8);
}

// --- U-Type Tests ---
TEST_F(ImmGenTest, UTypePositiveImmediate) {
    // lui x10, 2
    // U-Types load into the upper 20 bits (2 << 12 = 8192)
    EXPECT_IMM_EQ(immGen.generate(0x00002537), 8192);
}

TEST_F(ImmGenTest, UTypeNegativeImmediate) {
    // lui x10, 0xFFFFF
    // 0xFFFFF << 12 = -4096 in 32-bit two's complement
    EXPECT_IMM_EQ(immGen.generate(0xfffff537), -4096);
}

// --- J-Type Tests ---
TEST_F(ImmGenTest, JTypePositiveImmediate) {
    // jal x1, 16
    EXPECT_IMM_EQ(immGen.generate(0x010000ef), 16);
}

TEST_F(ImmGenTest, JTypeNegativeImmediate) {
    /*
     * Manual check:
     *              [31:28] | [27:24] | [23:20] | [19:16] | [15:12] | [11:8] | [7:4] | [3:0]
     * 0xff5ff0ef = 1111    |   1111  | 0101    |  1111   |  1111   |  0000  | 1110  | 1111
     * imm_msb@[31] = 1 ->  (Maps to all bits up to and including 20)
     * imm_hi@[19:12] = 11111111-> (Maps to bits 19: 12)
     * imm_11_bit@[20] = 1 -> (Maps to bit 11)
     * imm_lo@[30:21] = 11 1111 1010 -> Maps to bits 10:1
     * Bit 0 is always 0
     * [31:28] | [27:24] | [23:20] | [19:16] | [15:12] | [11:8] | [7:4] | [3:0]
     *   1111  |  1111   |  1111   | 1111    |  1111 |  1111    | 1111  | 0100
     * = -12 (2s complement)
     */
    // jal x1, -12
    EXPECT_IMM_EQ(immGen.generate(0xff5ff0ef), -12);
}

// --- Edge Case / Default Fallback ---
TEST_F(ImmGenTest, RTypeReturnsZero) {
    // add x3, x1, x2
    // R-Types do not have immediates; the switch default should handle it
    EXPECT_IMM_EQ(immGen.generate(0x002081b3), 0);
}