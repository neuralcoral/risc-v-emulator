//
// Created by George Alfaro on 11/24/25.
//

#include "cpu.h"
#include "src/devices/memory.h"
#include "src/instructions/i_type.h"
#include "src/instructions/s_type.h"
#include "src/instructions/b_type.h"
#include "src/instructions/u_type.h"
#include "src/instructions/j_type.h"
#include "src/exceptions/decode_exception.h"
#include <iostream>

Cpu::Cpu() {
    LOG("Initializing CPU...");
    pc = 0;
}


Cpu::~Cpu() { }

void Cpu::fetch(Memory instructions) {
    const riscv::instruction_t byte0 = instructions.read(pc);
    const riscv::instruction_t byte1 = instructions.read(pc + 1);
    const riscv::instruction_t byte2 = instructions.read(pc + 2);
    const riscv::instruction_t byte3 = instructions.read(pc + 3);
    instruction = byte3 << 24 | byte2 << 16 | byte1 << 8 | byte0;
    pc += 4;
}

BaseInstruction Cpu::decode() {
    const riscv::opcode_t opcode = (instruction >> 2) & 0x1F;
    BaseInstruction ins;
    switch (opcode) {
        case 0x00: case 0x04: case 0x19:
            ins = IType(instruction);
            break;
        case 0x08:
            ins = SType(instruction);
            break;
        case 0x18:
            ins = BType(instruction);
            break;
        case 0x05: case 0x0D:
            ins = UType(instruction);
            break;
        case 0x1B:
            ins = JType(instruction);
            break;
        case 0x0C:
            ins = RType(instruction);
            break;
        default:
            throw DecodeException(std::string("No matching instruction type found for ") + std::to_string(instruction));
    }
    return ins;
}