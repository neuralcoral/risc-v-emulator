#include "src/register_file.h"
#include "src/exceptions/register_file_exception.h"

RegisterFile::RegisterFile() {
    reset();
}

void RegisterFile::reset() {
    for(auto & i : registers) {
        i = 0;
    }
}

void RegisterFile::write(const riscv::address_t &registerAddress, const uint32_t &value) {
    if (registerAddress >= riscv::NUM_OF_REGISTERS or registerAddress < 0) {
        throw RegisterFileException {
            "RegisterFile::write failed. registerAddress=" + std::to_string(registerAddress) + " is out of bounds"
        };
    }
    if (registerAddress == 0) {
        return;
    }

    registers[registerAddress] = value;
}

uint32_t RegisterFile::read(const riscv::address_t &registerAddress) {
    if (registerAddress >= riscv::NUM_OF_REGISTERS or registerAddress < 0) {
        throw RegisterFileException {
                "RegisterFile::read failed. registerAddress=" + std::to_string(registerAddress) + " is out of bounds"
        };
    }
    return registers[registerAddress];
}

