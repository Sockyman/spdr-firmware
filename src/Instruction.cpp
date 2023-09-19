#include "Instruction.hpp"

Micro::Micro(RID readFrom, RID writeTo, std::set<Line> lines) 
    :   readFrom{readFrom},
        writeTo{writeTo},
        ioOperation{true},
        lines{lines} {}

Micro::Micro(RID readFrom, RID writeTo)
    :   readFrom{readFrom},
        writeTo{writeTo},
        ioOperation{true},
        lines{} {}

Micro::Micro(std::set<Line> lines)
    :   ioOperation{false},
        lines{lines} {}

void Micro::merge(const Micro& other) {
    if (other.ioOperation) {
        if (this->ioOperation)
            throw; // TODO: Error stuff
        this->ioOperation = true;
        this->readFrom = other.readFrom;
        this->writeTo = other.writeTo;
    }

    for (auto element : other.lines) {
        this->lines.insert(element);
    }
}

std::ostream& operator<<(std::ostream& stream, Micro& micro) {
    stream << "{ ";
    if (micro.ioOperation) {
        stream << micro.readFrom << " -> " << micro.writeTo << ", ";
    }

    for (Line line : micro.lines) {
        stream << line << ", ";
    }
    return stream << "}";
}

Instruction::Instruction(std::string name, AddressingMode mode)
    :   name{name}, mode{mode} {}

Instruction::Instruction() : name{}, mode{} {}

std::ostream& operator<<(std::ostream& stream, const Instruction& instruction) {
    stream << instruction.name;
    if (instruction.mode.destination.mode != Mode::Implied)
        stream << ' ';
    return stream << instruction.mode;
}

bool operator<(const Instruction& ins0, const Instruction& ins1) {
    if (ins0.name == ins1.name)
        return ins0.mode < ins1.mode;
    return ins0.name < ins1.name;
}

