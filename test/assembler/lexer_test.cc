#include "src/assembler/lexer.h"

#include <gtest/gtest.h>

#include <vector>

#include "src/assembler/token.h"
#include "src/assembler/token_type.h"

// ---------------------------------------------------------------------------
// Helper: extract token types from a tokenize() result
// ---------------------------------------------------------------------------
static std::vector<TokenType> types(const std::vector<Token>& tokens) {
    std::vector<TokenType> result;
    result.reserve(tokens.size());
    for (const auto& t : tokens) {
        result.push_back(t.tokenType);
    }
    return result;
}

// ---------------------------------------------------------------------------
// Empty / whitespace-only input
// ---------------------------------------------------------------------------

TEST(LexerTest, EmptySourceProducesOnlyEOF) {
    Lexer lexer("");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, END_OF_FILE);
}

TEST(LexerTest, WhitespaceOnlyProducesOnlyEOF) {
    Lexer lexer("   \t  ");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, END_OF_FILE);
}

// ---------------------------------------------------------------------------
// Comment handling
// ---------------------------------------------------------------------------

TEST(LexerTest, LineCommentAloneProducesOnlyEOF) {
    Lexer lexer("# this is a comment");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, END_OF_FILE);
}

TEST(LexerTest, InlineCommentIsSkipped) {
    // "add x1, x2, x3  # comment" should lex identically to "add x1, x2, x3"
    Lexer lexer("add x1, x2, x3  # comment");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{ADD, X1, COMMA, X2, COMMA, X3, END_OF_FILE}));
}

// ---------------------------------------------------------------------------
// Newlines
// ---------------------------------------------------------------------------

TEST(LexerTest, NewlineToken) {
    Lexer lexer("\n");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{NEWLINE, END_OF_FILE}));
}

TEST(LexerTest, MultipleNewlines) {
    Lexer lexer("\n\n");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{NEWLINE, NEWLINE, END_OF_FILE}));
}

// ---------------------------------------------------------------------------
// Mnemonics
// ---------------------------------------------------------------------------

TEST(LexerTest, RTypeMnemonics) {
    const std::vector<std::pair<std::string, TokenType>> cases = {
        {"add", ADD}, {"sub", SUB}, {"sll", SLL}, {"slt", SLT}, {"sltu", SLTU},
        {"xor", XOR}, {"srl", SRL}, {"sra", SRA}, {"or", OR},   {"and", AND},
        {"mul", MUL}, {"div", DIV}, {"rem", REM},
    };
    for (const auto& [mnemonic, expected] : cases) {
        Lexer lexer(mnemonic);
        auto tokens = lexer.tokenize();
        ASSERT_GE(tokens.size(), 1u) << "mnemonic: " << mnemonic;
        EXPECT_EQ(tokens[0].tokenType, expected) << "mnemonic: " << mnemonic;
        EXPECT_EQ(tokens[0].tokenText, mnemonic) << "mnemonic: " << mnemonic;
    }
}

TEST(LexerTest, ITypeMnemonics) {
    const std::vector<std::pair<std::string, TokenType>> cases = {
        {"addi", ADDI}, {"slti", SLTI}, {"sltiu", SLTIU}, {"xori", XORI}, {"ori", ORI},
        {"andi", ANDI}, {"slli", SLLI}, {"srli", SRLI},   {"srai", SRAI}, {"lb", LB},
        {"lh", LH},     {"lw", LW},     {"lbu", LBU},     {"lhu", LHU},   {"jalr", JALR},
    };
    for (const auto& [mnemonic, expected] : cases) {
        Lexer lexer(mnemonic);
        auto tokens = lexer.tokenize();
        ASSERT_GE(tokens.size(), 1u) << "mnemonic: " << mnemonic;
        EXPECT_EQ(tokens[0].tokenType, expected) << "mnemonic: " << mnemonic;
    }
}

TEST(LexerTest, STypeMnemonics) {
    for (const auto& [mnemonic, expected] :
         std::vector<std::pair<std::string, TokenType>>{{"sb", SB}, {"sh", SH}, {"sw", SW}}) {
        Lexer lexer(mnemonic);
        auto tokens = lexer.tokenize();
        ASSERT_GE(tokens.size(), 1u);
        EXPECT_EQ(tokens[0].tokenType, expected) << "mnemonic: " << mnemonic;
    }
}

TEST(LexerTest, BTypeMnemonics) {
    for (const auto& [mnemonic, expected] :
         std::vector<std::pair<std::string, TokenType>>{{"beq", BEQ},
                                                        {"bne", BNE},
                                                        {"blt", BLT},
                                                        {"bge", BGE},
                                                        {"bltu", BLTU},
                                                        {"bgeu", BGEU}}) {
        Lexer lexer(mnemonic);
        auto tokens = lexer.tokenize();
        ASSERT_GE(tokens.size(), 1u);
        EXPECT_EQ(tokens[0].tokenType, expected) << "mnemonic: " << mnemonic;
    }
}

TEST(LexerTest, UTypeMnemonics) {
    for (const auto& [mnemonic, expected] :
         std::vector<std::pair<std::string, TokenType>>{{"lui", LUI}, {"auipc", AUIPC}}) {
        Lexer lexer(mnemonic);
        auto tokens = lexer.tokenize();
        ASSERT_GE(tokens.size(), 1u);
        EXPECT_EQ(tokens[0].tokenType, expected) << "mnemonic: " << mnemonic;
    }
}

TEST(LexerTest, JTypeMnemonic) {
    Lexer lexer("jal");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, JAL);
}

// ---------------------------------------------------------------------------
// Registers — canonical names
// ---------------------------------------------------------------------------

TEST(LexerTest, CanonicalRegisters) {
    // Spot-check a few across the range
    const std::vector<std::pair<std::string, TokenType>> cases = {
        {"x0", X0}, {"x1", X1}, {"x10", X10}, {"x15", X15}, {"x31", X31},
    };
    for (const auto& [reg, expected] : cases) {
        Lexer lexer(reg);
        auto tokens = lexer.tokenize();
        ASSERT_GE(tokens.size(), 1u) << "register: " << reg;
        EXPECT_EQ(tokens[0].tokenType, expected) << "register: " << reg;
        EXPECT_EQ(tokens[0].tokenText, reg) << "register: " << reg;
    }
}

// ---------------------------------------------------------------------------
// Registers — ABI aliases
// ---------------------------------------------------------------------------

TEST(LexerTest, AbiRegisterAliases) {
    const std::vector<std::pair<std::string, TokenType>> cases = {
        {"zero", ZERO}, {"ra", RA}, {"sp", SP},   {"tp", TP}, {"t0", T0},
        {"t6", T6},     {"s0", S0}, {"s11", S11}, {"a0", A0}, {"a7", A7},
    };
    for (const auto& [reg, expected] : cases) {
        Lexer lexer(reg);
        auto tokens = lexer.tokenize();
        ASSERT_GE(tokens.size(), 1u) << "register: " << reg;
        EXPECT_EQ(tokens[0].tokenType, expected) << "register: " << reg;
    }
}

// ---------------------------------------------------------------------------
// Immediates
// ---------------------------------------------------------------------------

TEST(LexerTest, DecimalImmediate) {
    Lexer lexer("42");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, IMMEDIATE);
    EXPECT_EQ(tokens[0].tokenText, "42");
}

TEST(LexerTest, ZeroImmediate) {
    Lexer lexer("0");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, IMMEDIATE);
    EXPECT_EQ(tokens[0].tokenText, "0");
}

TEST(LexerTest, NegativeImmediate) {
    Lexer lexer("-16");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, IMMEDIATE);
    EXPECT_EQ(tokens[0].tokenText, "-16");
}

TEST(LexerTest, HexImmediate) {
    Lexer lexer("0xFF");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, IMMEDIATE);
    EXPECT_EQ(tokens[0].tokenText, "0xFF");
}

TEST(LexerTest, NegativeHexImmediate) {
    Lexer lexer("-0x10");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, IMMEDIATE);
    EXPECT_EQ(tokens[0].tokenText, "-0x10");
}

// ---------------------------------------------------------------------------
// Punctuation
// ---------------------------------------------------------------------------

TEST(LexerTest, Comma) {
    Lexer lexer(",");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{COMMA, END_OF_FILE}));
}

TEST(LexerTest, Parens) {
    Lexer lexer("()");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{L_PAREN, R_PAREN, END_OF_FILE}));
}

// ---------------------------------------------------------------------------
// Labels
// ---------------------------------------------------------------------------

TEST(LexerTest, LabelDefinition) {
    Lexer lexer("loop:");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, LABEL);
    EXPECT_EQ(tokens[0].tokenText, "loop");
}

TEST(LexerTest, LabelReference) {
    // An identifier with no ':' that isn't a keyword is a label reference
    Lexer lexer("loop");
    auto tokens = lexer.tokenize();
    ASSERT_GE(tokens.size(), 1u);
    EXPECT_EQ(tokens[0].tokenType, LABEL);
    EXPECT_EQ(tokens[0].tokenText, "loop");
}

// ---------------------------------------------------------------------------
// Full instruction lines
// ---------------------------------------------------------------------------

TEST(LexerTest, RTypeInstruction) {
    // add x1, x2, x3
    Lexer lexer("add x1, x2, x3");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{ADD, X1, COMMA, X2, COMMA, X3, END_OF_FILE}));
}

TEST(LexerTest, ITypeArithInstruction) {
    // addi x1, x2, 10
    Lexer lexer("addi x1, x2, 10");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens),
              (std::vector<TokenType>{ADDI, X1, COMMA, X2, COMMA, IMMEDIATE, END_OF_FILE}));
    EXPECT_EQ(tokens[5].tokenText, "10");
}

TEST(LexerTest, ITypeLoadInstruction) {
    // lw x1, 8(x2)
    Lexer lexer("lw x1, 8(x2)");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{LW, X1, COMMA, IMMEDIATE, L_PAREN, X2, R_PAREN,
                                                     END_OF_FILE}));
    EXPECT_EQ(tokens[3].tokenText, "8");
}

TEST(LexerTest, STypeInstruction) {
    // sw x1, -4(sp)
    Lexer lexer("sw x1, -4(sp)");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{SW, X1, COMMA, IMMEDIATE, L_PAREN, SP, R_PAREN,
                                                     END_OF_FILE}));
    EXPECT_EQ(tokens[3].tokenText, "-4");
}

TEST(LexerTest, BTypeInstruction) {
    // beq x1, x2, loop
    Lexer lexer("beq x1, x2, loop");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens),
              (std::vector<TokenType>{BEQ, X1, COMMA, X2, COMMA, LABEL, END_OF_FILE}));
    EXPECT_EQ(tokens[5].tokenText, "loop");
}

TEST(LexerTest, UTypeInstruction) {
    // lui x5, 0x12345
    Lexer lexer("lui x5, 0x12345");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{LUI, X5, COMMA, IMMEDIATE, END_OF_FILE}));
}

TEST(LexerTest, JTypeInstruction) {
    // jal ra, target
    Lexer lexer("jal ra, target");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{JAL, RA, COMMA, LABEL, END_OF_FILE}));
}

// ---------------------------------------------------------------------------
// Multi-line programs
// ---------------------------------------------------------------------------

TEST(LexerTest, MultiLineProgram) {
    const std::string src =
        "addi x1, x0, 5\n"
        "addi x2, x0, 10\n"
        "add  x3, x1, x2\n";

    Lexer lexer(src);
    auto tokens = lexer.tokenize();

    // Line 1: ADDI X1 COMMA X0 COMMA IMM NEWLINE
    // Line 2: ADDI X2 COMMA X0 COMMA IMM NEWLINE
    // Line 3: ADD  X3 COMMA X1 COMMA X2  NEWLINE
    // EOF
    EXPECT_EQ(types(tokens), (std::vector<TokenType>{
                                 ADDI,  X1,    COMMA, X0,    COMMA,     IMMEDIATE,  NEWLINE, ADDI,
                                 X2,    COMMA, X0,    COMMA, IMMEDIATE, NEWLINE,    ADD,     X3,
                                 COMMA, X1,    COMMA, X2,    NEWLINE,   END_OF_FILE}));
}

TEST(LexerTest, ProgramWithLabelAndComment) {
    const std::string src =
        "# simple loop\n"
        "loop:\n"
        "  addi x1, x1, -1\n"
        "  bne x1, x0, loop\n";

    Lexer lexer(src);
    auto tokens = lexer.tokenize();

    EXPECT_EQ(types(tokens),
              (std::vector<TokenType>{NEWLINE,         // after comment line
                                      LABEL, NEWLINE,  // loop:\n
                                      ADDI, X1, COMMA, X1, COMMA, IMMEDIATE, NEWLINE, BNE, X1,
                                      COMMA, X0, COMMA, LABEL, NEWLINE, END_OF_FILE}));
    // "loop" label def
    EXPECT_EQ(tokens[1].tokenText, "loop");
    // "-1" immediate
    EXPECT_EQ(tokens[8].tokenText, "-1");
    // "loop" label ref
    EXPECT_EQ(tokens[15].tokenText, "loop");
}

// ---------------------------------------------------------------------------
// Error cases
// ---------------------------------------------------------------------------

TEST(LexerTest, UnexpectedCharacterThrows) {
    Lexer lexer("@");
    EXPECT_THROW(lexer.tokenize(), LexError);
}

TEST(LexerTest, StandaloneMinusThrows) {
    // '-' not followed by a digit
    Lexer lexer("- ");
    EXPECT_THROW(lexer.tokenize(), LexError);
}

TEST(LexerTest, IncompleteHexThrows) {
    Lexer lexer("0x");
    EXPECT_THROW(lexer.tokenize(), LexError);
}
