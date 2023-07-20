#ifndef LINE_HPP
#define LINE_HPP

#include <string>
#include <iostream>

enum class Line {
    nop,
    step_reset,
    set_flags,
    halt,
    counter_enable,
    logic,
    clear_irq,
    carry,

    inc,
    add,
    sub,
    passthrough,
    dec,
    sub_zero,
    lsh,
    b_and,
    b_or,
    b_xor,
    b_not,
    minus_one,
    zero,
    flip,
};

std::ostream& operator<<(std::ostream& stream, Line line);

#endif

