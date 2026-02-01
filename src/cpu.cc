//
// Created by George Alfaro on 11/24/25.
//

#include "cpu.h"
#include <iostream>

//#include "InstructionSetArchitecture.h"

Cpu::Cpu() {
    LOG("Initializing CPU...");
    reset();
}

void Cpu::reset() {
    LOG("Resetting CPU...");
    programCounter = 0;

    for (int i = 0; i < NUM_OF_REGISTERS; i++) {
        registers[i] = 0x0;
    }
}

Cpu::~Cpu() { }

void Cpu::execute(INSTRUCTION instruction) {
    //if (OP_CODES.find(instruction) == OP_CODES.end()) {
    //    throw std::runtime_error("BaseInstruction not found");
    //}

    //const InstructionSet opcode = OP_CODES.at(instruction);

    //switch (opcode) {
    //    case ADDIW:
    //        break;
    //    case ADDW:
    //        break;
    //    default:
    //        throw std::runtime_error("Invalid opcode");
    //}
}
