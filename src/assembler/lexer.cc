#include "src/assembler/lexer.h"

#include <cctype>
#include <string>
#include <vector>

#include "src/assembler/token.h"
#include "src/assembler/token_type.h"


Lexer::Lexer(const std::string& source) : source(source), pos(0) {}


bool Lexer::isAtEnd() const { return pos >= source.size(); }

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[pos];
}

char Lexer::peekNext() const {
    if (pos + 1 >= source.size()) return '\0';
    return source[pos + 1];
}

char Lexer::advance() { return source[pos++]; }

void Lexer::skipWhitespace() {
    // Newlines are meaningful so we don't skip
    while (!isAtEnd() && (peek() == ' ' || peek() == '\t')) {
        advance();
    }
}

// Skip from '#' to end of line (does not consume the newline itself).
void Lexer::skipLineComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

Token Lexer::scanMnemonicOrRegisterOrLabel() {
    const std::size_t start = pos;
    while (!isAtEnd() && (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')) {
        advance();
    }
    const std::string word = source.substr(start, pos - start);

    // Label definition: "word:"
    if (!isAtEnd() && peek() == ':') {
        advance();  // consume ':'
        return Token(word, LABEL);
    }

    // Known mnemonic or register
    if (Token* mapped = Token::tokenMap(word)) {
        Token t = *mapped;
        delete mapped;
        return t;
    }

    // Unknown identifier — treat as a label reference (branch/jump target)
    return Token(word, LABEL);
}

// Scan a decimal or hexadecimal immediate value, including an optional
// leading '-' for negative numbers.
//
//   Decimal:     [-]digits
//   Hexadecimal: [-]0x[digits/a-fA-F]
Token Lexer::scanImmediate() {
    const std::size_t start = pos;

    // Optional leading minus
    if (peek() == '-') {
        advance();
        if (!std::isdigit(static_cast<unsigned char>(peek()))) {
            throw LexError("Expected digit after '-' at position " + std::to_string(pos));
        }
    }

    // Hexadecimal prefix
    if (peek() == '0' && (peekNext() == 'x' || peekNext() == 'X')) {
        advance();  // '0'
        advance();  // 'x'/'X'
        if (!std::isxdigit(static_cast<unsigned char>(peek()))) {
            throw LexError("Expected hex digit after '0x' at position " + std::to_string(pos));
        }
        while (!isAtEnd() && std::isxdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }
    } else {
        while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }
    }

    return Token(source.substr(start, pos - start), IMMEDIATE);
}

// Scan a single-character punctuation token.
Token Lexer::scanPunctuation() {
    const char c = advance();
    switch (c) {
        case ',':
            return Token(",", COMMA);
        case '(':
            return Token("(", L_PAREN);
        case ')':
            return Token(")", R_PAREN);
        case '\n':
            return Token("\n", NEWLINE);
        default: {
            throw LexError(std::string("Unexpected character '") + c + "' at position " +
                           std::to_string(pos - 1));
        }
    }
}


std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    pos = 0;

    while (true) {
        skipWhitespace();

        if (isAtEnd()) {
            tokens.emplace_back("", END_OF_FILE);
            break;
        }

        const char c = peek();

        if (c == '#') {
            skipLineComment();
            continue;
        }

        if (c == '\n') {
            tokens.push_back(scanPunctuation());
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            tokens.push_back(scanMnemonicOrRegisterOrLabel());
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c)) ||
            (c == '-' && std::isdigit(static_cast<unsigned char>(peekNext())))) {
            tokens.push_back(scanImmediate());
            continue;
        }

        if (c == ',' || c == '(' || c == ')') {
            tokens.push_back(scanPunctuation());
            continue;
        }

        throw LexError(std::string("Unexpected character '") + c + "' at position " +
                       std::to_string(pos));
    }

    return tokens;
}
