#include "gtest/gtest.h"
#include "src/cpu/cpu.h"
#include "src/devices/memory.h"
#include "src/register_file.h"

class CpuTest : public ::testing::Test {
protected:
    Memory memory;
    RegisterFile registerFile;
    Cpu cpu;

    CpuTest() : memory(), registerFile(), cpu(memory, registerFile) {
        registerFile.reset();
    }

    void writeInstruction(riscv::instruction_t instr) {
        memory.write(0, instr & 0xFF);
        memory.write(1, (instr >> 8) & 0xFF);
        memory.write(2, (instr >> 16) & 0xFF);
        memory.write(3, (instr >> 24) & 0xFF);
    }
};

// IType
TEST_F(CpuTest, ADDIWithZeroImmProducesZero) {
    writeInstruction(0x00000513);  // addi x10, x0, 0
    cpu.step();
    riscv::uword_t expected = 0;
    riscv::uword_t actual = registerFile.read(10);
    EXPECT_EQ(actual, expected);
}

// SType
TEST_F(CpuTest, SWStoresByte0ToMemory) {
    writeInstruction(0x00A02023);  // sw x10, 0(x0)
    registerFile.write(10, 0xDEADBEEF);
    cpu.step();
    riscv::ubyte_t expected = 0xEF;
    riscv::ubyte_t actual = memory.read(0);
    EXPECT_EQ(actual, expected);
}

TEST_F(CpuTest, SWStoresByte1ToMemory) {
    writeInstruction(0x00A02023);
    registerFile.write(10, 0xDEADBEEF);
    cpu.step();
    riscv::ubyte_t expected = 0xBE;
    riscv::ubyte_t actual = memory.read(1);
    EXPECT_EQ(actual, expected);
}

TEST_F(CpuTest, SWStoresByte2ToMemory) {
    writeInstruction(0x00A02023);
    registerFile.write(10, 0xDEADBEEF);
    cpu.step();
    riscv::ubyte_t expected = 0xAD;
    riscv::ubyte_t actual = memory.read(2);
    EXPECT_EQ(actual, expected);
}

TEST_F(CpuTest, SWStoresByte3ToMemory) {
    writeInstruction(0x00A02023);
    registerFile.write(10, 0xDEADBEEF);
    cpu.step();
    riscv::ubyte_t expected = 0xDE;
    riscv::ubyte_t actual = memory.read(3);
    EXPECT_EQ(actual, expected);
}

// BType
TEST_F(CpuTest, BEQBranchesWhenEqual) {
    writeInstruction(0x00408163);
    registerFile.write(1, 10);
    registerFile.write(2, 10);
    cpu.step();
    riscv::reg_t expected = 4;
    riscv::reg_t actual = cpu.getPc();
    EXPECT_EQ(actual, expected);
}

TEST_F(CpuTest, BNEBranchesWhenNotEqual) {
    writeInstruction(0x00410163);
    registerFile.write(1, 10);
    registerFile.write(2, 10);
    cpu.step();
    riscv::reg_t expected = 4;
    riscv::reg_t actual = cpu.getPc();
    EXPECT_EQ(actual, expected);
}

// UType
TEST_F(CpuTest, LUILoadsUpperImmediate) {
    writeInstruction(0x00002137);  // lui x2, 2
    cpu.step();
    riscv::uword_t expected = 0x2000;
    riscv::uword_t actual = registerFile.read(2);
    EXPECT_EQ(actual, expected);
}

// JType
TEST_F(CpuTest, JALStoresReturnAddress) {
    writeInstruction(0x014000EF);  // jal x1, 20
    cpu.step();
    riscv::mem_addr_t expected = 8;  // pc is already original+4 at execute time, so pc+4 = 8
    riscv::mem_addr_t actual = registerFile.read(1);
    EXPECT_EQ(actual, expected);
}

TEST_F(CpuTest, JALUpdatesPC) {
    writeInstruction(0x014000EF);  // jal x1, 20
    cpu.step();
    riscv::mem_addr_t expected = 20;
    riscv::mem_addr_t actual = cpu.getPc();
    EXPECT_EQ(actual, expected);
}

// RType
TEST_F(CpuTest, ADDAddsTwoRegisters) {
    writeInstruction(0x002081B3);  // add x3, x1, x2
    registerFile.write(1, 10);
    registerFile.write(2, 20);
    cpu.step();
    riscv::uword_t expected = 30;
    riscv::uword_t actual = registerFile.read(3);
    EXPECT_EQ(actual, expected);
}
