#include "InstructionSet.hpp"
#include "Mode.hpp"
#include <iostream>

int main() {
    InstructionSet instructions{};
    for (auto& fs: instructions.getInstruction({"ret", {}})->micros) {
        //std::cout << fs.first << '\n';
        for (auto& micro : fs.second) {
            std::cout << micro << '\n';
        }
        break;
        std::cout << '\n';
    }

    return 0;
}

