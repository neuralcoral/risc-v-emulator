#include <iostream>
#include "src/cpu/cpu.h"

int main() {
    std::cout << "Hello, Bazel!" << std::endl;
    Memory memory = Memory();
    RegisterFile register_file = RegisterFile();
    auto cpu = Cpu(memory, register_file);
    return 0;
}