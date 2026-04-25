#ifndef RISC_V_EMULATOR_CPU_H
#define RISC_V_EMULATOR_CPU_H
#include <cstdint>
#include <array>
#include "src/instructions/r_type.h"
#include "src/devices/memory.h"
#include "src/cpu/decoder.h"
#include "src/register_file.h"

#ifdef DEBUG
    #define LOG(x) std::cout << x << std::endl
#else
    #define LOG(x)
#endif

class Cpu {
    static constexpr riscv::reg_t RESET_VECTOR = 0x00000000;
    riscv::address_t  rs1{}, rs2{}, rd{};
    riscv::reg_t pc{RESET_VECTOR}, data{};
    riscv::instruction_t  instruction{};
    Memory* memory;
    RegisterFile* registerFile;
    void fetch();
    void execute(const DecodedInstr& decodedInstr);
    void executeIType(const DecodedInstr& decodedInstr);
    void handleLoads(const DecodedInstr& decodedInstr);
    void handleOpImm(const DecodedInstr& decodedInstr);
    void handleJumpAndLinkRegister(const DecodedInstr& decodedInstr);
    void handleStores(const DecodedInstr& decodedInstr);
    void executeSType(const DecodedInstr& decodedInstr);
    void executeBType(const DecodedInstr& decodedInstr);
    void executeUType(const DecodedInstr& decodedInstr);
    void executeJType(const DecodedInstr& decodedInstr);
    void executeRType(const DecodedInstr& decodedInstr);

    void memoryAccess();
    void writeBack();
public:
    Cpu(Memory& memory, RegisterFile& registerFile);
    ~Cpu();

    void step();

    riscv::reg_t getPc() const { return pc; }
};

#endif  // RISC_V_EMULATOR_CPU_H