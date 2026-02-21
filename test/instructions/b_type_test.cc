#include "gtest/gtest.h"
#include "src/instructions/b_type.h"

class BTypeTest : public ::testing::Test {
protected:
    BType b_type_inst;
    /* This is the machine code for: BEQ x1, x2, 8
     * We are just making sure the masking works so we do not care much
     * accuracy of actual machine code
     * [12] | imm[10:5] | rs2 | rs1 | funct3 | imm[4:1] | [11] | opcode
     *  0|000 000|0 0010 | 0000 1|000 | 0100 | 0|110 0011
     * Hex: 0x00208463
     **/

    BTypeTest() : b_type_inst(0x00208463) { }
};

TEST_F(BTypeTest, DecodeRs2Correctly) {
    EXPECT_EQ(b_type_inst.getRs2(), 2);
}

TEST_F(BTypeTest, DecodeRs1Correctly) {
    EXPECT_EQ(b_type_inst.getRs1(), 1);
}

TEST_F(BTypeTest, DecodeFunct3Correctly) {
    EXPECT_EQ(b_type_inst.getFunct3(), 0);
}

TEST_F(BTypeTest, DecodeOpcodeCorrectly) {
    EXPECT_EQ(b_type_inst.getOpCode(), 0b1100011);
}