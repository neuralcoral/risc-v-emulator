#ifndef RISC_V_EMULATOR_REGISTER_FILE_H
#define RISC_V_EMULATOR_REGISTER_FILE_H

#include <array>
#include <cstdint>
#include "common/types.h"

class RegisterFile {
private:
    static constexpr uint8_t NUM_OF_REGISTERS { 32 };
    std::array<riscv::reg_t , NUM_OF_REGISTERS> registers{};
public:
    explicit RegisterFile();
    void reset();
    void write(const riscv::address_t& registerAddress, const riscv::word_t & value);
    uint32_t read(const riscv::address_t& registerAddress);

};

#endif //RISC_V_EMULATOR_REGISTER_FILE_H
