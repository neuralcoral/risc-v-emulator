#ifndef RISC_V_EMULATOR_INVALID_ASSEMBLY_INSTRUCTION_H
#define RISC_V_EMULATOR_INVALID_ASSEMBLY_INSTRUCTION_H

#include <exception>

class InvalidAssemblyInstructionException: public std::exception {
};

#endif //RISC_V_EMULATOR_INVALID_ASSEMBLY_INSTRUCTION_H
