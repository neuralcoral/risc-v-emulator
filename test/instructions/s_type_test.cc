#include "gtest/gtest.h"
#include "src/instructions/s_type.h"

class STypeTest : public ::testing::Test {
protected:
    SType s_type_inst;

    /* This is the machine code for: SW x2, 4(x1)
     * We are just making sure the masking works so we do not care much
     * accuracy of actual machine code
     * Imm [11:5] | rs2 | rs1 | funct3 | imm[4:0]  | opcode
     * 0000 000|0 0010 | 0000 1|000 | 0010 0|010 0011
     * Hex: 0x208223
     **/

    STypeTest() :  s_type_inst(0x208223) {}
};

TEST_F(STypeTest, DecodeRs2Correctly) {
    EXPECT_EQ(s_type_inst.getRs2(), 0b0010);
}

TEST_F(STypeTest, DecodeRs1Correctly) {
    EXPECT_EQ(s_type_inst.getRs1(), 0b0001);
}

TEST_F(STypeTest, DecodeFunct3Correctly) {
    EXPECT_EQ(s_type_inst.getFunct3(), 0);
}

TEST_F(STypeTest, DecodeOpcodeCorrectly) {
    EXPECT_EQ(s_type_inst.getOpCode(), 0b00100011);
}

