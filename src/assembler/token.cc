
#include "src/assembler/token.h"

#include <unordered_map>
#include <utility>

#include "src/assembler/token_type.h"

Token::Token() { tokenType = END_OF_FILE; }

Token::Token(std::string tokenText, const TokenType &tokenType)
    : tokenText(std::move(tokenText)), tokenType(tokenType) {}

Token *Token::tokenMap(const std::string &tokenText) {
    static const std::unordered_map<std::string, TokenType> tokenMapper = {
        {"\n", NEWLINE}, {",", COMMA}, {"(", L_PAREN},   {")", R_PAREN},

        {"x0", X0},      {"x1", X1},        {"x2", X2},       {"x3", X3},     {"x4", X4},
        {"x5", X5},      {"x6", X6},        {"x7", X7},       {"x8", X8},     {"x9", X9},
        {"x10", X10},    {"x11", X11},      {"x12", X12},     {"x13", X13},   {"x14", X14},
        {"x15", X15},    {"x16", X16},      {"x17", X17},     {"x18", X18},   {"x19", X19},
        {"x20", X20},    {"x21", X21},      {"x22", X22},     {"x23", X23},   {"x24", X24},
        {"x25", X25},    {"x26", X26},      {"x27", X27},     {"x28", X28},   {"x29", X29},
        {"x30", X30},    {"x31", X31},

        {"zero", ZERO},  {"ra", RA},        {"sp", SP},       {"tp", TP},     {"t0", T0},
        {"t1", T1},      {"t2", T2},        {"t3", T3},       {"t4", T4},     {"t5", T5},
        {"t6", T6},      {"s0", S0},        {"s1", S1},       {"s2", S2},     {"s3", S3},
        {"s4", S4},      {"s5", S5},        {"s6", S6},       {"s7", S7},     {"s8", S8},
        {"s9", S9},      {"s10", S10},      {"s11", S11},     {"a0", A0},     {"a1", A1},
        {"a2", A2},      {"a3", A3},        {"a4", A4},       {"a5", A5},     {"a6", A6},
        {"a7", A7},

        {"add", ADD},    {"sub", SUB},      {"sll", SLL},     {"slt", SLT},   {"sltu", SLTU},
        {"xor", XOR},    {"srl", SRL},      {"sra", SRA},     {"or", OR},     {"and", AND},
        {"mul", MUL},    {"div", DIV},      {"rem", REM},

        {"addi", ADDI},  {"slti", SLTI},    {"sltiu", SLTIU}, {"xori", XORI}, {"ori", ORI},
        {"andi", ANDI},  {"slli", SLLI},    {"srli", SRLI},   {"srai", SRAI}, {"lb", LB},
        {"lh", LH},      {"lw", LW},        {"lbu", LBU},     {"lhu", LHU},   {"jalr", JALR},

        {"sb", SB},      {"sh", SH},        {"sw", SW},

        {"beq", BEQ},    {"bne", BNE},      {"blt", BLT},     {"bge", BGE},   {"bltu", BLTU},
        {"bgeu", BGEU},

        {"lui", LUI},    {"auipc", AUIPC},

        {"jal", JAL},
    };

    auto typeIt = tokenMapper.find(tokenText);
    if (typeIt == tokenMapper.end()) {
        return nullptr;
    }
    return new Token(tokenText, typeIt->second);
}
