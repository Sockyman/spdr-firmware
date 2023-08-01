#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Line.hpp"
#include "Register.hpp"
#include "Flag.hpp"
#include "Mode.hpp"
#include <string>
#include <set>
#include <vector>
#include <iostream>

class Micro {
public:
    RID readFrom;
    RID writeTo;
    bool ioOperation;
    std::set<Line> lines;

    Micro(RID readFrom, RID writeTo, std::set<Line> lines);
    Micro(RID readFrom, RID writeTo);
    Micro(std::set<Line> lines);

    void merge(const Micro& other);
};

std::ostream& operator<<(std::ostream& stream, Micro& micro);

class Instruction {
public:
    std::string name;
    AddressingMode mode;

    Instruction(std::string name, AddressingMode mode);
    Instruction();
};

std::ostream& operator<<(std::ostream& stream, const Instruction& instruction);

bool operator<(const Instruction& ins0, const Instruction& ins1);

#endif

