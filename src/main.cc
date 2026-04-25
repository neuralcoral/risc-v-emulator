#include <iostream>
#include "src/cpu/cpu.h"

int main() {
    std::cout << "Hello, Bazel!" << std::endl;
    auto cpu = Cpu();
    return 0;
}