#include "Line.hpp"
#include <map>

std::ostream& operator<<(std::ostream& stream, Line line) {
    static std::map<Line, std::string> registerNames {
        {Line::nop, "nop"},
        {Line::step_reset, "sr"},
        {Line::set_flags, "fs"},
        {Line::halt, "hlt"},
        {Line::counter_enable, "ce"},
        {Line::logic, "lgc"},
        {Line::clear_irq, "cirq"},
        {Line::carry, "cy"},

        {Line::inc, "ALU_inc"},
        {Line::add, "ALU_add"},
        {Line::sub, "ALU_sub"},
        {Line::passthrough, "ALU_pass"},
        {Line::dec, "ALU_dec"},
        {Line::sub_zero, "ALU_subz"},
        {Line::lsh, "ALU_lsh"},
        {Line::b_and, "ALU_and"},
        {Line::b_or, "ALU_or"},
        {Line::b_xor, "ALU_xor"},
        {Line::b_not, "ALU_not"},
        {Line::minus_one, "ALU_xFF"},
        {Line::zero, "ALU_00"},
        {Line::flip, "ALU_flip"},
    };
    return stream << registerNames[line];
}

