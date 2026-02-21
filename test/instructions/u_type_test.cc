#include "src/instructions/u_type.h"
#include "gtest/gtest.h"

class UTypeTest : public ::testing::Test {
protected:
    UType u_type_inst;


    /* This is the machine code for: LUI x31
     * We are just making sure the masking works so we do not care much
     * accuracy of actual machine code
     * Imm [31:12] | rd |  opcode
     * 0001 0010 0011 0100 0101 | 1111 1|011 0111
     * Hex: 0x12345FB7
     **/
    UTypeTest() : u_type_inst(0x12345FB7) {}
};

TEST_F(UTypeTest, DecodesRdCorrectly) {
    EXPECT_EQ(u_type_inst.getRd(), 31);
}

TEST_F(UTypeTest, DecodesOpcodeCorrectly) {
    EXPECT_EQ(u_type_inst.getOpCode(), 0b0110111);
}
