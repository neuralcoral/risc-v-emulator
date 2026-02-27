#ifndef RISC_V_EMULATOR_ASSEMBLER_H
#define RISC_V_EMULATOR_ASSEMBLER_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "src/common/types.h"

class Assembler {
   public:
    static std::unordered_map<std::string, std::function<riscv::instruction_t(std::vector<std::string>&)>> mnemonicParser;

    static riscv::instruction_t parseRType(std::vector<std::string>& tokens);
    static riscv::instruction_t parseIType(std::vector<std::string>& tokens);
    static riscv::instruction_t parseSType(std::vector<std::string>& tokens);
    static riscv::instruction_t parseBType(std::vector<std::string>& tokens);
    static riscv::instruction_t parseUType(std::vector<std::string>& tokens);
    static riscv::instruction_t parseJType(std::vector<std::string>& tokens);

    void assemble(const std::string& filename);
    riscv::instruction_t translate(const std::string& asmLine);
};

#endif  // RISC_V_EMULATOR_ASSEMBLER_H
