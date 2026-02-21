#ifndef RISC_V_EMULATOR_CUSTOM_ASSERTIONS_H
#define RISC_V_EMULATOR_CUSTOM_ASSERTIONS_H
#include <gtest/gtest.h>
#include <bitset>
#include <iomanip>

// Custom macro to print hex and binary on failure
#define EXPECT_IMM_EQ(actual, expected) \
    do { \
        auto _expected = static_cast<int32_t>(expected); \
        auto _actual = static_cast<int32_t>(actual); \
        EXPECT_EQ(_actual, _expected) \
            << "\n========================================" \
            << "\nActual Hex: 0x" << std::hex << std::setw(8) << std::setfill('0') << _actual \
            << "\nActual Bin: " << std::bitset<32>(_actual) \
            << "\n========================================\n" \
            << std::dec; \
    } while(0)

#endif //RISC_V_EMULATOR_CUSTOM_ASSERTIONS_H
