#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "src/assembler/lexer.h"
#include "src/assembler/token.h"
#include "src/assembler/token_type.h"

static void printTokens(const std::vector<Token>& tokens) {
    for (const auto& t : tokens) {
        if (t.tokenType == END_OF_FILE) break;
        std::cout << "  [" << toString(t.tokenType) << "]";
        if (!t.tokenText.empty() && t.tokenText != "\n") {
            std::cout << "  \"" << t.tokenText << "\"";
        }
        std::cout << "\n";
    }
}

static void runOnSource(const std::string& source, const std::string& label) {
    if (!label.empty()) std::cout << "=== " << label << " ===\n";
    try {
        Lexer lexer(source);
        printTokens(lexer.tokenize());
    } catch (const LexError& e) {
        std::cerr << "LexError: " << e.what() << "\n";
    }
}

static void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "error: cannot open file '" << path << "'\n";
        return;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    runOnSource(ss.str(), path);
}

static void runRepl() {
    std::cout << "RISC-V Lexer REPL  (Ctrl-D to quit)\n";
    std::string line;
    while (true) {
        std::cout << "> ";
        std::cout.flush();
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        runOnSource(line, "");
    }
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        runRepl();
    } else {
        for (int i = 1; i < argc; ++i) {
            runFile(argv[i]);
        }
    }
    return 0;
}
