#include "src/assembler/assembler.h"

#include <gtest/gtest.h>

#include <vector>

#include "src/assembler/lexer.h"
#include "src/assembler/token.h"
#include "src/assembler/token_type.h"
#include "src/common/types.h"

// ---------------------------------------------------------------------------
// Helper: lex a single instruction line into tokens (strips NEWLINE / EOF)
// ---------------------------------------------------------------------------
static std::vector<Token> lex(const std::string& line) {
    Lexer lexer(line);
    std::vector<Token> all = lexer.tokenize();
    std::vector<Token> result;
    for (const auto& t : all) {
        if (t.tokenType != NEWLINE && t.tokenType != END_OF_FILE) {
            result.push_back(t);
        }
    }
    return result;
}

// ===========================================================================
// R-Type
// ===========================================================================

// add x3, x1, x2  →  0x002081B3
// funct7=0000000 rs2=00010 rs1=00001 funct3=000 rd=00011 opcode=0110011
TEST(AssemblerTest, RType_Add) {
    EXPECT_EQ(Assembler::translate(lex("add x3, x1, x2")), 0x002081B3u);
}

// sub x3, x1, x2  →  funct7=0100000, same fields otherwise → 0x402081B3
TEST(AssemblerTest, RType_Sub) {
    EXPECT_EQ(Assembler::translate(lex("sub x3, x1, x2")), 0x402081B3u);
}

// and x5, x6, x7
TEST(AssemblerTest, RType_And) {
    // funct7=0, rs2=7, rs1=6, funct3=111, rd=5, op=0110011
    uint32_t expected =
        (0b0000000 << 25) | (7 << 20) | (6 << 15) | (0b111 << 12) | (5 << 7) | 0b0110011;
    EXPECT_EQ(Assembler::translate(lex("and x5, x6, x7")), expected);
}

// xor x1, x2, x3
TEST(AssemblerTest, RType_Xor) {
    uint32_t expected =
        (0b0000000 << 25) | (3 << 20) | (2 << 15) | (0b100 << 12) | (1 << 7) | 0b0110011;
    EXPECT_EQ(Assembler::translate(lex("xor x1, x2, x3")), expected);
}

// ABI alias registers: add a0, s0, t0  → x10, x8, x5
TEST(AssemblerTest, RType_AbiAliases) {
    uint32_t expected =
        (0b0000000 << 25) | (5 << 20) | (8 << 15) | (0b000 << 12) | (10 << 7) | 0b0110011;
    EXPECT_EQ(Assembler::translate(lex("add a0, s0, t0")), expected);
}

// gp (x3) as a register operand
TEST(AssemblerTest, RType_GpAlias) {
    // add x1, gp, x0  →  gp=x3=3, x0=0
    uint32_t expected =
        (0b0000000 << 25) | (0 << 20) | (3 << 15) | (0b000 << 12) | (1 << 7) | 0b0110011;
    EXPECT_EQ(Assembler::translate(lex("add x1, gp, x0")), expected);
}

// ===========================================================================
// I-Type — arithmetic
// ===========================================================================

// addi x1, x1, 8  →  0x00808093
// imm=000000001000 rs1=00001 funct3=000 rd=00001 opcode=0010011
TEST(AssemblerTest, IType_Addi) {
    EXPECT_EQ(Assembler::translate(lex("addi x1, x1, 8")), 0x00808093u);
}

// addi x0, x0, 0  — NOP encoding
TEST(AssemblerTest, IType_AddiNop) {
    EXPECT_EQ(Assembler::translate(lex("addi x0, x0, 0")), 0x00000013u);
}

// addi with negative immediate: addi x1, x0, -1
// imm = -1 = 0xFFF (12-bit two's complement)
TEST(AssemblerTest, IType_AddiNegative) {
    uint32_t expected = (0xFFF << 20) | (0 << 15) | (0b000 << 12) | (1 << 7) | 0b0010011;
    EXPECT_EQ(Assembler::translate(lex("addi x1, x0, -1")), expected);
}

// addi with hex immediate: addi x1, x0, 0xFF
TEST(AssemblerTest, IType_AddiHex) {
    uint32_t expected = (0xFF << 20) | (0 << 15) | (0b000 << 12) | (1 << 7) | 0b0010011;
    EXPECT_EQ(Assembler::translate(lex("addi x1, x0, 0xFF")), expected);
}

// ori x2, x3, 7
TEST(AssemblerTest, IType_Ori) {
    uint32_t expected = (7 << 20) | (3 << 15) | (0b110 << 12) | (2 << 7) | 0b0010011;
    EXPECT_EQ(Assembler::translate(lex("ori x2, x3, 7")), expected);
}

// andi x4, x5, 15
TEST(AssemblerTest, IType_Andi) {
    uint32_t expected = (15 << 20) | (5 << 15) | (0b111 << 12) | (4 << 7) | 0b0010011;
    EXPECT_EQ(Assembler::translate(lex("andi x4, x5, 15")), expected);
}

// ===========================================================================
// I-Type — shifts
// ===========================================================================

// slli x1, x2, 3  → funct7=0000000 shamt=3
TEST(AssemblerTest, IType_Slli) {
    uint32_t expected =
        (0b0000000 << 25) | (3 << 20) | (2 << 15) | (0b001 << 12) | (1 << 7) | 0b0010011;
    EXPECT_EQ(Assembler::translate(lex("slli x1, x2, 3")), expected);
}

// srli x1, x2, 4  → funct7=0000000
TEST(AssemblerTest, IType_Srli) {
    uint32_t expected =
        (0b0000000 << 25) | (4 << 20) | (2 << 15) | (0b101 << 12) | (1 << 7) | 0b0010011;
    EXPECT_EQ(Assembler::translate(lex("srli x1, x2, 4")), expected);
}

// srai x1, x2, 4  → funct7=0100000
TEST(AssemblerTest, IType_Srai) {
    uint32_t expected =
        (0b0100000 << 25) | (4 << 20) | (2 << 15) | (0b101 << 12) | (1 << 7) | 0b0010011;
    EXPECT_EQ(Assembler::translate(lex("srai x1, x2, 4")), expected);
}

// ===========================================================================
// I-Type — loads
// ===========================================================================

// lw x1, 8(x2)
TEST(AssemblerTest, IType_Lw) {
    uint32_t expected = (8 << 20) | (2 << 15) | (0b010 << 12) | (1 << 7) | 0b0000011;
    EXPECT_EQ(Assembler::translate(lex("lw x1, 8(x2)")), expected);
}

// lw with negative offset: lw x1, -4(x2)
TEST(AssemblerTest, IType_LwNegativeOffset) {
    int32_t imm = -4;
    uint32_t expected = ((static_cast<uint32_t>(imm) & 0xFFF) << 20) | (2 << 15) | (0b010 << 12) |
                        (1 << 7) | 0b0000011;
    EXPECT_EQ(Assembler::translate(lex("lw x1, -4(x2)")), expected);
}

// lb x3, 0(x4)
TEST(AssemblerTest, IType_Lb) {
    uint32_t expected = (0 << 20) | (4 << 15) | (0b000 << 12) | (3 << 7) | 0b0000011;
    EXPECT_EQ(Assembler::translate(lex("lb x3, 0(x4)")), expected);
}

// lhu x5, 2(x6)
TEST(AssemblerTest, IType_Lhu) {
    uint32_t expected = (2 << 20) | (6 << 15) | (0b101 << 12) | (5 << 7) | 0b0000011;
    EXPECT_EQ(Assembler::translate(lex("lhu x5, 2(x6)")), expected);
}

// jalr x1, x2, 0
TEST(AssemblerTest, IType_Jalr) {
    uint32_t expected = (0 << 20) | (2 << 15) | (0b000 << 12) | (1 << 7) | 0b1100111;
    EXPECT_EQ(Assembler::translate(lex("jalr x1, x2, 0")), expected);
}

// ===========================================================================
// S-Type
// ===========================================================================

// sw x2, 4(x1)
// imm[11:5]=0000000 rs2=00010 rs1=00001 funct3=010 imm[4:0]=00100 op=0100011
// Note: the existing s_type_test.cc uses 0x208223 which has funct3=000 (sb),
// not funct3=010 (sw) — that constant tests the decoder bitmasks, not encoding.
TEST(AssemblerTest, SType_Sw) {
    uint32_t expected =
        (0b0000000 << 25) | (2 << 20) | (1 << 15) | (0b010 << 12) | (0b00100 << 7) | 0b0100011;
    EXPECT_EQ(Assembler::translate(lex("sw x2, 4(x1)")), expected);
}

// sb x3, 1(x4)
TEST(AssemblerTest, SType_Sb) {
    uint32_t expected =
        (0b0000000 << 25) | (3 << 20) | (4 << 15) | (0b000 << 12) | (0b00001 << 7) | 0b0100011;
    EXPECT_EQ(Assembler::translate(lex("sb x3, 1(x4)")), expected);
}

// sw with negative offset: sw x1, -8(sp)
TEST(AssemblerTest, SType_SwNegativeOffset) {
    int32_t imm = -8;
    uint32_t u = static_cast<uint32_t>(imm);
    uint32_t imm11_5 = (u >> 5) & 0x7F;
    uint32_t imm4_0 = u & 0x1F;
    uint32_t expected =
        (imm11_5 << 25) | (1 << 20) | (2 << 15) | (0b010 << 12) | (imm4_0 << 7) | 0b0100011;
    EXPECT_EQ(Assembler::translate(lex("sw x1, -8(sp)")), expected);
}

// ===========================================================================
// B-Type — with numeric immediate
// ===========================================================================

// beq x1, x2, 8  →  0x00208463 (from existing test)
// imm=8: bit12=0 bit11=0 bits10:5=000000 bits4:1=0100
TEST(AssemblerTest, BType_BeqNumericOffset) {
    EXPECT_EQ(Assembler::translate(lex("beq x1, x2, 8")), 0x00208463u);
}

// bne x1, x2, 4
TEST(AssemblerTest, BType_BneNumericOffset) {
    // imm=4: imm4_1=0b0010, imm11=0, imm12=0, imm10_5=0
    uint32_t expected = (0 << 31) | (0b000000 << 25) | (2 << 20) | (1 << 15) | (0b001 << 12) |
                        (0b0010 << 8) | (0 << 7) | 0b1100011;
    EXPECT_EQ(Assembler::translate(lex("bne x1, x2, 4")), expected);
}

// blt x3, x4, -4 (backwards branch)
TEST(AssemblerTest, BType_BltNegativeOffset) {
    int32_t imm = -4;
    uint32_t u = static_cast<uint32_t>(imm);
    uint32_t imm12 = (u >> 12) & 0x1;
    uint32_t imm11 = (u >> 11) & 0x1;
    uint32_t imm10_5 = (u >> 5) & 0x3F;
    uint32_t imm4_1 = (u >> 1) & 0xF;
    uint32_t expected = (imm12 << 31) | (imm10_5 << 25) | (4 << 20) | (3 << 15) | (0b100 << 12) |
                        (imm4_1 << 8) | (imm11 << 7) | 0b1100011;
    EXPECT_EQ(Assembler::translate(lex("blt x3, x4, -4")), expected);
}

// ===========================================================================
// U-Type
// ===========================================================================

// lui x31, 0x12345  →  0x12345FB7 (from existing test)
TEST(AssemblerTest, UType_Lui) {
    EXPECT_EQ(Assembler::translate(lex("lui x31, 0x12345")), 0x12345FB7u);
}

// lui x1, 1
TEST(AssemblerTest, UType_LuiSmall) {
    uint32_t expected = (1 << 12) | (1 << 7) | 0b0110111;
    EXPECT_EQ(Assembler::translate(lex("lui x1, 1")), expected);
}

// auipc x1, 0
TEST(AssemblerTest, UType_Auipc) {
    uint32_t expected = (0 << 12) | (1 << 7) | 0b0010111;
    EXPECT_EQ(Assembler::translate(lex("auipc x1, 0")), expected);
}

// ===========================================================================
// J-Type — with numeric immediate
// ===========================================================================

// jal x1, 20  →  0x014000EF (from existing test)
// imm=20: imm20=0 imm19_12=00000000 imm11=0 imm10_1=0000001010
TEST(AssemblerTest, JType_JalNumericOffset) {
    EXPECT_EQ(Assembler::translate(lex("jal x1, 20")), 0x014000EFu);
}

// jal x0, 0  — infinite loop (j 0 in pseudocode)
TEST(AssemblerTest, JType_JalZeroOffset) {
    // imm=0, rd=0, opcode=1101111
    uint32_t expected = 0b1101111u;
    EXPECT_EQ(Assembler::translate(lex("jal x0, 0")), expected);
}

// ===========================================================================
// Full program assembly with label resolution
// ===========================================================================

TEST(AssemblerTest, Assemble_SimpleProgram) {
    // addi x1, x0, 5   → instruction 0 (addr 0)
    // addi x2, x0, 10  → instruction 1 (addr 4)
    // add  x3, x1, x2  → instruction 2 (addr 8)
    const std::string src =
        "addi x1, x0, 5\n"
        "addi x2, x0, 10\n"
        "add  x3, x1, x2\n";

    auto prog = Assembler::assemble(src);
    ASSERT_EQ(prog.size(), 3u);

    EXPECT_EQ(prog[0], Assembler::translate(lex("addi x1, x0, 5")));
    EXPECT_EQ(prog[1], Assembler::translate(lex("addi x2, x0, 10")));
    EXPECT_EQ(prog[2], Assembler::translate(lex("add x3, x1, x2")));
}

TEST(AssemblerTest, Assemble_LabelOnOwnLine) {
    // loop:            → label at instruction 0
    // addi x1, x1, -1 → instruction 0 (addr 0)
    // bne  x1, x0, loop → instruction 1 (addr 4), offset = (0-1)*4 = -4
    const std::string src =
        "loop:\n"
        "addi x1, x1, -1\n"
        "bne x1, x0, loop\n";

    auto prog = Assembler::assemble(src);
    ASSERT_EQ(prog.size(), 2u);

    EXPECT_EQ(prog[0], Assembler::translate(lex("addi x1, x1, -1")));
    // bne with offset -4
    EXPECT_EQ(prog[1], Assembler::translate(lex("bne x1, x0, -4")));
}

TEST(AssemblerTest, Assemble_ForwardBranch) {
    // beq x1, x2, done → instruction 0 (addr 0), offset = (2-0)*4 = 8
    // addi x3, x0, 1   → instruction 1 (addr 4)
    // done:            → label at instruction 2
    // addi x4, x0, 2   → instruction 2 (addr 8)
    const std::string src =
        "beq x1, x2, done\n"
        "addi x3, x0, 1\n"
        "done:\n"
        "addi x4, x0, 2\n";

    auto prog = Assembler::assemble(src);
    ASSERT_EQ(prog.size(), 3u);

    // beq with offset +8
    EXPECT_EQ(prog[0], Assembler::translate(lex("beq x1, x2, 8")));
    EXPECT_EQ(prog[1], Assembler::translate(lex("addi x3, x0, 1")));
    EXPECT_EQ(prog[2], Assembler::translate(lex("addi x4, x0, 2")));
}

TEST(AssemblerTest, Assemble_JalWithLabel) {
    // jal x1, func     → instruction 0 (addr 0), offset = (2-0)*4 = 8
    // addi x2, x0, 0   → instruction 1 (addr 4)
    // func:            → label at instruction 2
    // add  x3, x1, x2  → instruction 2 (addr 8)
    const std::string src =
        "jal x1, func\n"
        "addi x2, x0, 0\n"
        "func:\n"
        "add x3, x1, x2\n";

    auto prog = Assembler::assemble(src);
    ASSERT_EQ(prog.size(), 3u);

    EXPECT_EQ(prog[0], Assembler::translate(lex("jal x1, 8")));
    EXPECT_EQ(prog[1], Assembler::translate(lex("addi x2, x0, 0")));
    EXPECT_EQ(prog[2], Assembler::translate(lex("add x3, x1, x2")));
}

TEST(AssemblerTest, Assemble_IgnoresComments) {
    const std::string src =
        "# initialise\n"
        "addi x1, x0, 1  # x1 = 1\n"
        "addi x2, x0, 2  # x2 = 2\n";

    auto prog = Assembler::assemble(src);
    ASSERT_EQ(prog.size(), 2u);
    EXPECT_EQ(prog[0], Assembler::translate(lex("addi x1, x0, 1")));
    EXPECT_EQ(prog[1], Assembler::translate(lex("addi x2, x0, 2")));
}

// ===========================================================================
// Error handling
// ===========================================================================

TEST(AssemblerTest, UndefinedLabelThrows) {
    EXPECT_THROW(Assembler::assemble("beq x1, x2, missing\n"), ParseError);
}
