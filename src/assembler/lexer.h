#ifndef RISC_V_EMULATOR_LEXER_H
#define RISC_V_EMULATOR_LEXER_H

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/assembler/token.h"

class LexError : public std::runtime_error {
   public:
    explicit LexError(const std::string& message) : std::runtime_error(message) {}
};

class Lexer {
    std::string source;
    std::size_t pos;

    bool isAtEnd() const;
    char peek() const;
    char peekNext() const;
    char advance();

    void skipWhitespace();
    void skipLineComment();

    Token scanMnemonicOrRegisterOrLabel();
    Token scanImmediate();
    Token scanPunctuation();

   public:
    explicit Lexer(const std::string& source);

    std::vector<Token> tokenize();
};

#endif  // RISC_V_EMULATOR_LEXER_H
