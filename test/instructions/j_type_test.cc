#include "gtest/gtest.h"
#include "src/instructions/j_type.h"

class JTypeTest : public ::testing::Test {
protected:
    JType j_type_inst;

    /* This is the machine code for: JAL x1, 20
    * We are just making sure the masking works so we do not care much
    * accuracy of actual machine code
    *  Imm[20]| Imm [10:1] | Imm [11] | Imm [19:12]| rd  | Opcode
    *  0|000 0001 010|0 | 0000 0000 | 0000 1|110 1111
    * Hex: 0x014000EF
    **/
    JTypeTest() : j_type_inst(0x014000EF) {}
};

TEST_F(JTypeTest, DecodesImmCorrectly) {
    EXPECT_EQ(j_type_inst.getImm(), 20);
}

TEST_F(JTypeTest, DecodesRdCorrectly) {
    EXPECT_EQ(j_type_inst.getRd(), 1);
}

TEST_F(JTypeTest, DecodesOpCodeCorrectly) {
   EXPECT_EQ(j_type_inst.getOpCode(), 0b1101111) ;
}