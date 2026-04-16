#ifndef RISC_V_EMULATOR_DECODER_H
#define RISC_V_EMULATOR_DECODER_H

#include "src/common/types.h"
#include "src/datapath/imm_gen.h"
#include "src/exceptions/decode_exception.h"
#include <ostream>
#include <iomanip>

enum InstructionType { IType, SType, BType, UType, JType, RType };

constexpr riscv::reg_t NO_REG = 0xFF;
constexpr riscv::funct_t NO_FUNCT = 0xFF;

// This immediate is impossible to create with RISC-V hardware,
// Can still be used in assembly, but it'd be split up by a clever
// assembler
constexpr riscv::word_t NO_IMM = 0xDEADBEEF;

struct DecodedInstr {
    riscv::opcode_t opcode;
    riscv::reg_t rd, rs1, rs2;
    riscv::word_t immediate;
    riscv::funct_t funct3, funct7;
    InstructionType type;
    DecodedInstr(riscv::opcode_t opcode, riscv::reg_t rd, riscv::reg_t rs1, riscv::reg_t rs2,
                 riscv::word_t immediate, riscv::funct_t funct3, riscv::funct_t funct7,
                 InstructionType type)
        : opcode(opcode),
          rd(rd),
          rs1(rs1),
          rs2(rs2),
          immediate(immediate),
          funct3(funct3),
          funct7(funct7),
          type(type) {}
};

class Decoder {
   private:
    static constexpr riscv::address_t REGISTER_BITMASK = 0x1F;
    static constexpr riscv::funct_t FUNCT_7_BITMASK = 0x7F;
    static constexpr riscv::funct_t FUNCT_3_BITMASK = 0x7;
    static ImmGen immGen;
    static DecodedInstr handleIType(const riscv::instruction_t& ins);
    static DecodedInstr handleSType(const riscv::instruction_t& ins);
    static DecodedInstr handleBType(const riscv::instruction_t& ins);
    static DecodedInstr handleUType(const riscv::instruction_t& ins);
    static DecodedInstr handleJType(const riscv::instruction_t& ins);
    static DecodedInstr handleRType(const riscv::instruction_t& ins);

   public:
    Decoder() = default;
    DecodedInstr decode(const riscv::instruction_t& ins);
};

#endif  // RISC_V_EMULATOR_DECODER_H
