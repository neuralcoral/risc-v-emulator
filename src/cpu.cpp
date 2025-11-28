//
// Created by George Alfaro on 11/24/25.
//

#include "cpu.h"

Cpu::Cpu() {
    programCounter = 0;
    registers = new uint64_t(NUM_OF_REGISTERS);
}

Cpu::~Cpu() {
    delete registers;
}