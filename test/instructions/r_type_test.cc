#include "gtest/gtest.h"
#include "src/instructions/r_type.h"
//
// Created by george on 1/31/26.
//

class RTypeTest : public ::testing::Test {
protected:
    RType r_type_inst;

    /* This is the machine code for: add x3, x1, x2
     * We are just making sure the masking works so we do not care much
     * accuracy of actual machine code
     * Funct7 | rs2 | rs1 | Funct3 | rd  | Opcode
     * 0000000|00010|00001| 000    |00011|0110011
     * Hex: 0x002081B3
     **/
    RTypeTest() : r_type_inst(0x002081B3) {}
};

TEST_F(RTypeTest, DecodeRdCorrectly) {
    EXPECT_EQ(r_type_inst.getRd(), 0b00011);
}

TEST_F(RTypeTest, DecodesRs1Correctly) {
    EXPECT_EQ(r_type_inst.getRs1(), 0b00001);
}

TEST_F(RTypeTest, DecodesRs2Correctly) {
    EXPECT_EQ(r_type_inst.getRs2(), 0b00010);
}

TEST_F(RTypeTest, DecodesFunct3Correctly) {
    EXPECT_EQ(r_type_inst.getFunct3(), 0b0);
}

TEST_F(RTypeTest, DecodesFunct7Correctly) {
    EXPECT_EQ(r_type_inst.getFunct7(), 0b0000000);
}

TEST_F(RTypeTest, DecodesOpCodeCorrectly) {
    EXPECT_EQ(r_type_inst.getOpCode(), 0b0110011);
}
