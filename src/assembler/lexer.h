#ifndef RISC_V_EMULATOR_LEXER_H
#define RISC_V_EMULATOR_LEXER_H


#include <cstdint>
#include "src/assembler/token.h"

class Lexer {
    std::string source;
    std::uint64_t currentPosition, sourceLength;

    Token* handleMnemonic();
    Token* handleRegister();
    Token* handleImmediate();
    Token* handleLabel();
    Token* handleBrackets();

public:
    explicit Lexer(const std::string& source);

    void skipWhitespace();
    void skipComment();
    Token* getToken();
    static void abort(const std::string& message);

};


#endif //RISC_V_EMULATOR_LEXER_H
