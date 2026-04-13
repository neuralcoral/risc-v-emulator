#include "src/assembler/assembler.h"

#include <string>
#include <unordered_map>
#include <vector>

#include "src/assembler/lexer.h"
#include "src/assembler/token.h"
#include "src/assembler/token_type.h"
#include "src/common/types.h"

// 7-bit opcodes
const std::unordered_map<TokenType, riscv::opcode_t> Assembler::opcodeTable = {
    // R-type
    {ADD, 0b0110011},
    {SUB, 0b0110011},
    {SLL, 0b0110011},
    {SLT, 0b0110011},
    {SLTU, 0b0110011},
    {XOR, 0b0110011},
    {SRL, 0b0110011},
    {SRA, 0b0110011},
    {OR, 0b0110011},
    {AND, 0b0110011},

    // I-type arithmetic
    {ADDI, 0b0010011},
    {SLTI, 0b0010011},
    {SLTIU, 0b0010011},
    {XORI, 0b0010011},
    {ORI, 0b0010011},
    {ANDI, 0b0010011},
    {SLLI, 0b0010011},
    {SRLI, 0b0010011},
    {SRAI, 0b0010011},

    // I-type load
    {LB, 0b0000011},
    {LH, 0b0000011},
    {LW, 0b0000011},
    {LBU, 0b0000011},
    {LHU, 0b0000011},

    // I-type jump
    {JALR, 0b1100111},

    // S-type
    {SB, 0b0100011},
    {SH, 0b0100011},
    {SW, 0b0100011},

    // B-type
    {BEQ, 0b1100011},
    {BNE, 0b1100011},
    {BLT, 0b1100011},
    {BGE, 0b1100011},
    {BLTU, 0b1100011},
    {BGEU, 0b1100011},

    // U-type
    {LUI, 0b0110111},
    {AUIPC, 0b0010111},

    // J-type
    {JAL, 0b1101111},
};

// 3-bit funct3
const std::unordered_map<TokenType, riscv::funct_t> Assembler::funct3Table = {
    // R-type
    {ADD, 0b000},
    {SUB, 0b000},
    {SLL, 0b001},
    {SLT, 0b010},
    {SLTU, 0b011},
    {XOR, 0b100},
    {SRL, 0b101},
    {SRA, 0b101},
    {OR, 0b110},
    {AND, 0b111},

    // I-type arithmetic
    {ADDI, 0b000},
    {SLTI, 0b010},
    {SLTIU, 0b011},
    {XORI, 0b100},
    {ORI, 0b110},
    {ANDI, 0b111},
    {SLLI, 0b001},
    {SRLI, 0b101},
    {SRAI, 0b101},

    // I-type load
    {LB, 0b000},
    {LH, 0b001},
    {LW, 0b010},
    {LBU, 0b100},
    {LHU, 0b101},

    // I-type jump
    {JALR, 0b000},

    // S-type
    {SB, 0b000},
    {SH, 0b001},
    {SW, 0b010},

    // B-type
    {BEQ, 0b000},
    {BNE, 0b001},
    {BLT, 0b100},
    {BGE, 0b101},
    {BLTU, 0b110},
    {BGEU, 0b111},
};

// 7-bit funct7 (R-type and I-type shifts that need a non-zero value)
const std::unordered_map<TokenType, riscv::funct_t> Assembler::funct7Table = {
    {ADD, 0b0000000},
    {SUB, 0b0100000},
    {SLL, 0b0000000},
    {SLT, 0b0000000},
    {SLTU, 0b0000000},
    {XOR, 0b0000000},
    {SRL, 0b0000000},
    {SRA, 0b0100000},
    {OR, 0b0000000},
    {AND, 0b0000000},
    // I-type shifts reuse funct7 to distinguish SRLI vs SRAI
    {SLLI, 0b0000000},
    {SRLI, 0b0000000},
    {SRAI, 0b0100000},
};

// Convert a register Token to its 5-bit register number (0–31).
riscv::address_t Assembler::resolveRegister(const Token& t) {
    const TokenType ty = t.tokenType;

    // Canonical x0–x31: X0=10, X1=11, …, X31=41  (see token_type.h)
    if (ty >= X0 && ty <= X31) {
        return static_cast<riscv::address_t>(ty - X0);
    }

    // ABI aliases
    static const std::unordered_map<TokenType, riscv::address_t> abiMap = {
        {ZERO, 0}, {RA, 1},  {SP, 2},   {GP, 3},   {TP, 4},  {T0, 5},  {T1, 6},  {T2, 7},
        {S0, 8},   {S1, 9},  {A0, 10},  {A1, 11},  {A2, 12}, {A3, 13}, {A4, 14}, {A5, 15},
        {A6, 16},  {A7, 17}, {S2, 18},  {S3, 19},  {S4, 20}, {S5, 21}, {S6, 22}, {S7, 23},
        {S8, 24},  {S9, 25}, {S10, 26}, {S11, 27}, {T3, 28}, {T4, 29}, {T5, 30}, {T6, 31},
    };

    auto it = abiMap.find(ty);
    if (it != abiMap.end()) return it->second;

    throw ParseError("Expected register, got '" + t.tokenText + "'");
}

// Parse a decimal or hex immediate string to int32.
int32_t Assembler::resolveImmediate(const Token& t) {
    if (t.tokenType != IMMEDIATE) {
        throw ParseError("Expected immediate, got '" + t.tokenText + "'");
    }
    try {
        return static_cast<int32_t>(std::stoi(t.tokenText, nullptr, 0));
    } catch (const std::exception&) {
        throw ParseError("Invalid immediate value: '" + t.tokenText + "'");
    }
}

// =============================================================================
// Format encoders
//
// Token layout after the lexer (no NEWLINE / EOF):
//
//   R-type:       [mnemonic, rd,  COMMA, rs1, COMMA, rs2]
//   I-arith/JALR: [mnemonic, rd,  COMMA, rs1, COMMA, imm]
//   I-load:       [mnemonic, rd,  COMMA, imm, L_PAREN, rs1, R_PAREN]
//   S-type:       [mnemonic, rs2, COMMA, imm, L_PAREN, rs1, R_PAREN]
//   B-type:       [mnemonic, rs1, COMMA, rs2, COMMA, label/imm]
//   U-type:       [mnemonic, rd,  COMMA, imm]
//   J-type:       [mnemonic, rd,  COMMA, label/imm]
// =============================================================================

riscv::instruction_t Assembler::encodeRType(const std::vector<Token>& tokens) {
    const TokenType mn = tokens[0].tokenType;
    const uint32_t rd = resolveRegister(tokens[1]);
    const uint32_t rs1 = resolveRegister(tokens[3]);
    const uint32_t rs2 = resolveRegister(tokens[5]);
    const uint32_t f3 = funct3Table.at(mn);
    const uint32_t f7 = funct7Table.at(mn);
    const uint32_t op = opcodeTable.at(mn);

    return (f7 << 25) | (rs2 << 20) | (rs1 << 15) | (f3 << 12) | (rd << 7) | op;
}

riscv::instruction_t Assembler::encodeIType(const std::vector<Token>& tokens) {
    const TokenType mn = tokens[0].tokenType;
    const uint32_t op = opcodeTable.at(mn);
    const uint32_t f3 = funct3Table.at(mn);
    const uint32_t rd = resolveRegister(tokens[1]);

    uint32_t rs1 = 0;
    int32_t imm = 0;

    // Detect load/JALR "imm(rs1)" vs arithmetic "rs1, imm" by looking for L_PAREN
    if (tokens.size() >= 7 && tokens[4].tokenType == L_PAREN) {
        imm = resolveImmediate(tokens[3]);
        rs1 = resolveRegister(tokens[5]);
    } else {
        rs1 = resolveRegister(tokens[3]);
        imm = resolveImmediate(tokens[5]);
    }

    // Shifts: funct7 occupies imm[11:5], shamt occupies imm[4:0]
    if (mn == SLLI || mn == SRLI || mn == SRAI) {
        const uint32_t shamt = static_cast<uint32_t>(imm) & 0x1F;
        const uint32_t f7 = funct7Table.at(mn);
        return (f7 << 25) | (shamt << 20) | (rs1 << 15) | (f3 << 12) | (rd << 7) | op;
    }

    const uint32_t imm12 = static_cast<uint32_t>(imm) & 0xFFF;
    return (imm12 << 20) | (rs1 << 15) | (f3 << 12) | (rd << 7) | op;
}

riscv::instruction_t Assembler::encodeSType(const std::vector<Token>& tokens) {
    const TokenType mn = tokens[0].tokenType;
    const uint32_t rs2 = resolveRegister(tokens[1]);
    const int32_t imm = resolveImmediate(tokens[3]);
    const uint32_t rs1 = resolveRegister(tokens[5]);
    const uint32_t f3 = funct3Table.at(mn);
    const uint32_t op = opcodeTable.at(mn);

    const uint32_t imm11_5 = (static_cast<uint32_t>(imm) >> 5) & 0x7F;
    const uint32_t imm4_0 = static_cast<uint32_t>(imm) & 0x1F;

    return (imm11_5 << 25) | (rs2 << 20) | (rs1 << 15) | (f3 << 12) | (imm4_0 << 7) | op;
}

riscv::instruction_t Assembler::encodeBType(const std::vector<Token>& tokens, int32_t offset) {
    const TokenType mn = tokens[0].tokenType;
    const uint32_t rs1 = resolveRegister(tokens[1]);
    const uint32_t rs2 = resolveRegister(tokens[3]);
    const uint32_t f3 = funct3Table.at(mn);
    const uint32_t op = opcodeTable.at(mn);

    const int32_t imm = (tokens[5].tokenType == IMMEDIATE) ? resolveImmediate(tokens[5]) : offset;

    const uint32_t u = static_cast<uint32_t>(imm);
    const uint32_t imm12 = (u >> 12) & 0x1;
    const uint32_t imm11 = (u >> 11) & 0x1;
    const uint32_t imm10_5 = (u >> 5) & 0x3F;
    const uint32_t imm4_1 = (u >> 1) & 0xF;

    return (imm12 << 31) | (imm10_5 << 25) | (rs2 << 20) | (rs1 << 15) | (f3 << 12) |
           (imm4_1 << 8) | (imm11 << 7) | op;
}

riscv::instruction_t Assembler::encodeUType(const std::vector<Token>& tokens) {
    const TokenType mn = tokens[0].tokenType;
    const uint32_t rd = resolveRegister(tokens[1]);
    const int32_t imm = resolveImmediate(tokens[3]);
    const uint32_t op = opcodeTable.at(mn);

    // The immediate encodes bits [31:12]; mask to 20 bits and place at [31:12]
    const uint32_t imm31_12 = static_cast<uint32_t>(imm) & 0xFFFFF;
    return (imm31_12 << 12) | (rd << 7) | op;
}

riscv::instruction_t Assembler::encodeJType(const std::vector<Token>& tokens, int32_t offset) {
    const TokenType mn = tokens[0].tokenType;
    const uint32_t rd = resolveRegister(tokens[1]);
    const uint32_t op = opcodeTable.at(mn);

    const int32_t imm = (tokens[3].tokenType == IMMEDIATE) ? resolveImmediate(tokens[3]) : offset;

    const uint32_t u = static_cast<uint32_t>(imm);
    const uint32_t imm20 = (u >> 20) & 0x1;
    const uint32_t imm19_12 = (u >> 12) & 0xFF;
    const uint32_t imm11 = (u >> 11) & 0x1;
    const uint32_t imm10_1 = (u >> 1) & 0x3FF;

    return (imm20 << 31) | (imm10_1 << 21) | (imm11 << 20) | (imm19_12 << 12) | (rd << 7) | op;
}

riscv::instruction_t Assembler::translate(const std::vector<Token>& tokens, int32_t labelOffset) {
    if (tokens.empty()) throw ParseError("Empty token list");

    const TokenType mn = tokens[0].tokenType;

    if (mn >= ADD && mn <= AND) return encodeRType(tokens);
    if (mn >= ADDI && mn <= JALR) return encodeIType(tokens);
    if (mn >= SB && mn <= SW) return encodeSType(tokens);
    if (mn >= BEQ && mn <= BGEU) return encodeBType(tokens, labelOffset);
    if (mn == LUI || mn == AUIPC) return encodeUType(tokens);
    if (mn == JAL) return encodeJType(tokens, labelOffset);

    throw ParseError("Unknown mnemonic: '" + tokens[0].tokenText + "'");
}

std::vector<riscv::instruction_t> Assembler::assemble(const std::string& source) {
    Lexer lexer(source);
    const std::vector<Token> allTokens = lexer.tokenize();

    std::vector<std::vector<Token>> lines;
    std::unordered_map<std::string, uint32_t> labelAddress;

    std::vector<Token> current;
    for (const Token& t : allTokens) {
        if (t.tokenType == END_OF_FILE || t.tokenType == NEWLINE) {
            if (!current.empty()) {
                if (current.size() == 1 && current[0].tokenType == LABEL) {
                    labelAddress[current[0].tokenText] = static_cast<uint32_t>(lines.size());
                } else {
                    if (current[0].tokenType == LABEL) {
                        labelAddress[current[0].tokenText] = static_cast<uint32_t>(lines.size());
                        current.erase(current.begin());
                    }
                    if (!current.empty()) {
                        lines.push_back(std::move(current));
                    }
                }
                current.clear();
            }
        } else {
            current.push_back(t);
        }
    }

    std::vector<riscv::instruction_t> program;
    program.reserve(lines.size());

    for (uint32_t i = 0; i < static_cast<uint32_t>(lines.size()); ++i) {
        const std::vector<Token>& line = lines[i];
        const TokenType mn = line[0].tokenType;

        int32_t labelOffset = 0;
        const bool isBranch = (mn >= BEQ && mn <= BGEU);
        const bool isJump = (mn == JAL);

        if (isBranch || isJump) {
            const Token& target = line.back();
            if (target.tokenType == LABEL) {
                auto it = labelAddress.find(target.tokenText);
                if (it == labelAddress.end()) {
                    throw ParseError("Undefined label: '" + target.tokenText + "'");
                }
                labelOffset = (static_cast<int32_t>(it->second) - static_cast<int32_t>(i)) * 4;
            }
        }

        program.push_back(translate(line, labelOffset));
    }

    return program;
}
