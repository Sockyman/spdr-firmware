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
        if (this->ioOperation) {
            throw std::runtime_error{"cannot merge 2 io operations"};
        }
        this->ioOperation = true;
        this->readFrom = other.readFrom;
        this->writeTo = other.writeTo;
    }

    for (auto element : other.lines) {
        this->lines.insert(element);
    }
}

std::ostream& operator<<(std::ostream& stream, const Micro& micro) {
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

int Instruction::getSize() const {
    return 1
        + ::getSize(this->mode.source.size)
        + ::getSize(this->mode.destination.size);
}

std::ostream& Instruction::print(std::ostream& stream, std::uint16_t argument) const {
    stream << this->name;
    if (!this->mode.destination.isImplied()) {
        stream << ' ';
    }
    this->mode.print(stream, argument);
    return stream;
}

std::ostream& Instruction::print(std::ostream& stream, const std::string& argument) const {
    stream << this->name;
    if (!this->mode.destination.isImplied()) {
        stream << ' ';
    }
    this->mode.print(stream, argument);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Instruction& instruction) {
    stream << instruction.name;
    if (!instruction.mode.destination.isImplied()) {
        stream << ' ';
    }
    return stream << instruction.mode;
}

std::strong_ordering operator<=>(
    const Instruction& ins0,
    const Instruction& ins1
) {
    if (ins0.name == ins1.name) {
        return ins0.mode <=> ins1.mode;
    }
    return ins0.name <=> ins1.name;
}

