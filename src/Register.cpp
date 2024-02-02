#include "Register.hpp"
#include <map>
#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& stream, RID reg) {
    static std::map<RID, std::string> registerNames {
        {RID::A, "a"},
        {RID::C, "c"},
        {RID::D, "d"},
        {RID::F, "f"},
        {RID::Temp, "temp"},
        {RID::Alu, "alu"},
        {RID::Sp, "sp"},
        {RID::PcL, "pcl"},
        {RID::PcH, "pch"},
        {RID::PcLH, "pc"},
        {RID::MarL, "marl"},
        {RID::MarH, "marh"},
        {RID::MarLH, "mar"},
        {RID::Mem, "r"},
        {RID::Out, "out"},
        {RID::Lcd, "lcd"},
        {RID::Kbd, "kbd"},
        {RID::Ins, "ir"},
        {RID::CD, "cd"},
        {RID::Prog, "prog"},
        {RID::None, "none"},
    };
    return stream << registerNames[reg];
}

