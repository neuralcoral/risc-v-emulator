#include "lexer.h"

Token* Lexer::handleMnemonic() {
    Token* mnemonic = Token::tokenMap(source);
    if (!mnemonic) {
        return mnemonic
    }
}