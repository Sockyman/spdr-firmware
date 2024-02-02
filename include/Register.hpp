#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <string>
#include <iostream>

enum class RID {
    A,
    C,
    D,
    F,
    Temp,
    Alu,
    Sp,
    PcL,
    PcH,
    PcLH,
    MarL,
    MarH,
    MarLH,
    Mem,
    Out,
    Lcd,
    Kbd,
    Ins,
    CD,
    Prog,
    None,
};

std::ostream& operator<<(std::ostream& stream, RID reg);

#endif

