#include "gtest/gtest.h"
#include "src/instructions/i_type.h"

class ITypeTest : public ::testing::Test {
protected:
    IType i_type_inst;

    /* This is the machine code for: ADDI s0, s0, 8
     * We are just making sure the masking works so we do not care much
     * accuracy of actual machine code
     *  Imm | rs1 | Funct3 | rd  | Opcode
     * 0001000|00001| 000    |00001|001 0011
     * Hex: 0x808093
     **/
    ITypeTest() : i_type_inst(0x808093) {}
};

TEST_F(ITypeTest, DecodeRdCorrectly) {
    EXPECT_EQ(i_type_inst.getRd(), 0b00001);
}

TEST_F(ITypeTest, DecodesRs1Correctly) {
    EXPECT_EQ(i_type_inst.getRs1(), 0b00001);
}

TEST_F(ITypeTest, DecodesFunct3Correctly) {
    EXPECT_EQ(i_type_inst.getFunct3(), 0b0);
}

TEST_F(ITypeTest, DecodesImmCorrectly) {
    EXPECT_EQ(i_type_inst.getImm(), 8);
}

TEST_F(ITypeTest, DecodesOpCodeCorrectly) {
    EXPECT_EQ(i_type_inst.getOpCode(), 0b0010011);
}
