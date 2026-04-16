#include "src/cpu/decoder.h"
#include "gtest/gtest.h"

class DecoderTest : public ::testing::Test {
protected:
    Decoder decoder;
};

void PrintTo(const DecodedInstr& instr, std::ostream* os) {
    *os << "DecodedInstr { "
        << "opcode: 0x" << std::hex << static_cast<int>(instr.opcode) << std::dec
        << ", rd: " << static_cast<int>(instr.rd)
        << ", rs1: " << static_cast<int>(instr.rs1)
        << ", rs2: " << static_cast<int>(instr.rs2)
        << ", imm: " << instr.immediate
        << ", funct3: " << static_cast<int>(instr.funct3)
        << ", funct7: " << static_cast<int>(instr.funct7)
        << ", type: " << instr.type
        << " }";
}

bool operator==(const DecodedInstr& lhs, const DecodedInstr& rhs) {
    return lhs.opcode == rhs.opcode &&
           lhs.rd == rhs.rd &&
           lhs.rs1 == rhs.rs1 &&
           lhs.rs2 == rhs.rs2 &&
           lhs.immediate == rhs.immediate &&
           lhs.funct3 == rhs.funct3 &&
           lhs.funct7 == rhs.funct7 &&
           lhs.type == rhs.type;
}
// --- Base Instruction Format Tests ---

TEST_F(DecoderTest, DecodesITypeAddi) {
    riscv::instruction_t ins = 0x00000513;
    // Actual hex decoding: rd=10, rs1=0, imm=0, funct3=0
    DecodedInstr expected(0x13, 10, 0, NO_REG, 0, 0, NO_FUNCT, IType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

TEST_F(DecoderTest, DecodesSTypeSw) {
    riscv::instruction_t ins = 0x00a02023;
    // Actual hex decoding: rs1=0, rs2=10, imm=0, funct3=2
    DecodedInstr expected(0x23, NO_REG, 0, 10, 0, 2, NO_FUNCT, SType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

TEST_F(DecoderTest, DecodesBTypeBeq) {
    riscv::instruction_t ins = 0x00608163;
    // Actual hex decoding: rs1=1, rs2=6, imm=2, funct3=0
    DecodedInstr expected(0x63, NO_REG, 1, 6, 2, 0, NO_FUNCT, BType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

TEST_F(DecoderTest, DecodesUTypeLui) {
    riscv::instruction_t ins = 0x000120b7;
    // Actual hex decoding: rd=1, imm=73728 (0x12000)
    DecodedInstr expected(0x37, 1, NO_REG, NO_REG, 73728, NO_FUNCT, NO_FUNCT, UType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

TEST_F(DecoderTest, DecodesJTypeJal) {
    riscv::instruction_t ins = 0x0040006f;
    // Actual hex decoding: rd=0, imm=4
    DecodedInstr expected(0x6f, 0, NO_REG, NO_REG, 4, NO_FUNCT, NO_FUNCT, JType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

TEST_F(DecoderTest, DecodesRTypeAdd) {
    riscv::instruction_t ins = 0x00210133;
    // Actual hex decoding: rd=2, rs1=2, rs2=2, funct3=0, funct7=0
    DecodedInstr expected(0x33, 2, 2, 2, NO_IMM, 0, 0, RType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

// --- Immediate Edge Case Tests ---
// Note: Casting negative expected values to unsigned riscv::word_t to match the struct

TEST_F(DecoderTest, DecodesITypeNegativeImmediate) {
    riscv::instruction_t ins = 0xFFF00513;
    // Actual hex decoding: rd=10, rs1=0, imm=-1, funct3=0
    DecodedInstr expected(0x13, 10, 0, NO_REG, static_cast<riscv::word_t>(-1), 0, NO_FUNCT, IType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

TEST_F(DecoderTest, DecodesITypePositiveImmediate) {
    riscv::instruction_t ins = 0x06400513;
    // Actual hex decoding: rd=10, rs1=0, imm=100, funct3=0
    DecodedInstr expected(0x13, 10, 0, NO_REG, 100, 0, NO_FUNCT, IType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

TEST_F(DecoderTest, DecodesITypeMaxPositiveImmediate) {
    riscv::instruction_t ins = 0x7FF00513;
    // Actual hex decoding: rd=10, rs1=0, imm=2047, funct3=0
    DecodedInstr expected(0x13, 10, 0, NO_REG, 2047, 0, NO_FUNCT, IType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}

TEST_F(DecoderTest, DecodesITypeMaxNegativeImmediate) {
    riscv::instruction_t ins = 0x80000513;
    // Actual hex decoding: rd=10, rs1=0, imm=-2048, funct3=0
    DecodedInstr expected(0x13, 10, 0, NO_REG, static_cast<riscv::word_t>(-2048), 0, NO_FUNCT, IType);
    DecodedInstr actual = decoder.decode(ins);
    EXPECT_EQ(expected, actual);
}