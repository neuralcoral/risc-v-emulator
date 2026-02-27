#include "assembler.h"

#include <functional>
#include <sstream>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, std::function<riscv::instruction_t(std::vector<std::string>&)>>
    Assembler::mnemonicParser = {
        {"add", parseRType},  {"sub", parseRType},   {"sll", parseRType},   {"slt", parseRType},
        {"sltu", parseRType}, {"xor", parseRType},   {"srl", parseRType},   {"sra", parseRType},
        {"or", parseRType},   {"and", parseRType},   {"mul", parseRType},   {"div", parseRType},
        {"rem", parseRType},

        {"addi", parseIType}, {"slti", parseIType},  {"sltiu", parseIType}, {"xori", parseIType},
        {"ori", parseIType},  {"andi", parseIType},  {"slli", parseIType},  {"srli", parseIType},
        {"srai", parseIType}, {"lb", parseIType},    {"lh", parseIType},    {"lw", parseIType},
        {"lbu", parseIType},  {"lhu", parseIType},   {"jalr", parseIType},

        {"sb", parseSType},   {"sh", parseSType},    {"sw", parseSType},

        {"beq", parseBType},  {"bne", parseBType},   {"blt", parseBType},   {"bge", parseBType},
        {"bltu", parseBType}, {"bgeu", parseBType},

        {"lui", parseUType},  {"auipc", parseUType},

        {"jal", parseJType}};

riscv::instruction_t Assembler::parseRType(std::vector<std::string>& tokens) {
    auto mnemonic = tokens[0];
    auto rd = tokens[1];
    auto rs1 = tokens[2];
    auto rs2 = tokens[3];

    return 0;
}

riscv::instruction_t Assembler::parseIType(std::vector<std::string>& tokens) {
    auto mnemonic = tokens[0];
    auto rd = tokens[1];
    auto rs1 = tokens[2];
    auto rs2 = tokens[3];
    return 0;
}

riscv::instruction_t Assembler::parseSType(std::vector<std::string>& tokens) {
    auto mnemonic = tokens[0];
    auto rd = tokens[1];
    auto rs1 = tokens[2];
    auto rs2 = tokens[3];
    return 0;
}

riscv::instruction_t Assembler::parseBType(std::vector<std::string>& tokens) {
    auto mnemonic = tokens[0];
    return 0;
}

riscv::instruction_t Assembler::parseUType(std::vector<std::string>& tokens) { return 0; }

riscv::instruction_t Assembler::parseJType(std::vector<std::string>& tokens) { return 0; }

void Assembler::assemble(const std::string& filename) {}

std::vector<std::string> parseLine(const std::string& asmLine) {
    std::stringstream ss(asmLine);
    std::string component;
    std::vector<std::string> result;
    while (ss >> component) {
        result.push_back(component);
    }
    return result;
}

riscv::instruction_t Assembler::translate(const std::string& asmLine) {
    riscv::instruction_t inst = 0;
    auto parsedLine = parseLine(asmLine);
    auto mnemonic = parsedLine[0];

    auto it = mnemonicParser.find(mnemonic);
    if (it != mnemonicParser.end()) {
        inst = it->second(parsedLine);
    }

    return inst;
}
