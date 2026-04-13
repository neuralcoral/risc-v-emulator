#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "src/assembler/assembler.h"
#include "src/common/types.h"

static void printUsage(const char* prog) {
    std::cerr << "Usage: " << prog << " <input.s> [-o <output.bin>]\n"
              << "  Reads RISC-V assembly from <input.s> and outputs machine code.\n"
              << "  Without -o, outputs hex to stdout.\n"
              << "  With -o, writes binary words to the specified file.\n";
}

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static void writeBinary(const std::string& path, const std::vector<riscv::instruction_t>& words) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Cannot open output file: " + path);
    }
    out.write(reinterpret_cast<const char*>(words.data()),
              static_cast<std::streamsize>(words.size() * sizeof(riscv::instruction_t)));
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string inputPath;
    std::string outputPath;
    bool binaryOutput = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            outputPath = argv[++i];
            binaryOutput = true;
        } else if (argv[i][0] != '-') {
            inputPath = argv[i];
        }
    }

    if (inputPath.empty()) {
        printUsage(argv[0]);
        return 1;
    }

    try {
        std::string source = readFile(inputPath);
        std::vector<riscv::instruction_t> program = Assembler::assemble(source);

        if (binaryOutput) {
            writeBinary(outputPath, program);
            std::cout << "Wrote " << program.size() << " instruction(s) to " << outputPath << "\n";
        } else {
            for (size_t i = 0; i < program.size(); ++i) {
                std::cout << program[i] << "\n";
            }
        }

    } catch (const ParseError& e) {
        std::cerr << "Parse error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}