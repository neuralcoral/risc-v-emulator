#ifndef RISC_V_EMULATOR_TOKEN_H
#define RISC_V_EMULATOR_TOKEN_H

#include <string>
#include "src/assembler/token_type.h"

struct Token {
    std::string tokenText;
    TokenType tokenType;

    Token();
    Token(std::string  tokenText, const TokenType& tokenType);

    static Token* tokenMap(const std::string& tokenText);
};

#endif //RISC_V_EMULATOR_TOKEN_H
