#ifndef RISC_V_EMULATOR_ASSEMBLER_H
#define RISC_V_EMULATOR_ASSEMBLER_H

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "src/assembler/token.h"
#include "src/common/types.h"

class ParseError : public std::runtime_error {
   public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Assembler {
    static const std::unordered_map<TokenType, riscv::opcode_t> opcodeTable;
    static const std::unordered_map<TokenType, riscv::funct_t> funct3Table;
    static const std::unordered_map<TokenType, riscv::funct_t> funct7Table;

    static riscv::address_t resolveRegister(const Token& t);
    static int32_t resolveImmediate(const Token& t);

    static riscv::instruction_t encodeRType(const std::vector<Token>& tokens);
    static riscv::instruction_t encodeIType(const std::vector<Token>& tokens);
    static riscv::instruction_t encodeSType(const std::vector<Token>& tokens);
    static riscv::instruction_t encodeBType(const std::vector<Token>& tokens, int32_t offset);
    static riscv::instruction_t encodeUType(const std::vector<Token>& tokens);
    static riscv::instruction_t encodeJType(const std::vector<Token>& tokens, int32_t offset);

   public:
    // Translate a single pre-tokenised instruction line.
    // Branch/jump tokens that contain a label reference must already have
    // their PC-relative offset resolved and passed in via `labelOffset`.
    static riscv::instruction_t translate(const std::vector<Token>& tokens,
                                          int32_t labelOffset = 0);

    // Assemble a full source string; returns one word per instruction in
    // program order (labels and blank lines are skipped).
    static std::vector<riscv::instruction_t> assemble(const std::string& source);
};

#endif  // RISC_V_EMULATOR_ASSEMBLER_H
