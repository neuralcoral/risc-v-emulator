#include "gtest/gtest.h"
#include "src/cpu.h"

TEST(CpuTest, SanityCheck) {
    Cpu cpu;
    // Since we haven't implemented anything yet, let's just assert true
    // to ensure the test infrastructure is working.
    EXPECT_TRUE(true);
}
