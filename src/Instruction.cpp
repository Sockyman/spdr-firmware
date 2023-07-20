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


/*MicroSequence::MicroSequence() {
    for (FlagSet flags : FlagSet::getAllCombinations()) {
        micros[flags] = std::vector<Micro>{};
    }
}

MicroSequence& MicroSequence::add(Micro micro) {
    for (auto& p : this->micros) {
        if (conditionToAdd.match(p.first))
            p.second.push_back(micro);
    }
    return *this;
}

MicroSequence& MicroSequence::add(RID readFrom, RID writeTo, std::set<Line> lines) {
    return this->add(Micro{readFrom, writeTo, lines});
}

MicroSequence& MicroSequence::add(RID readFrom, RID writeTo) {
    return this->add(Micro{readFrom, writeTo});
}

MicroSequence& MicroSequence::add(std::set<Line> lines) {
    return this->add(Micro{lines});
}

MicroSequence& MicroSequence::with(FlagSet flags, Micro micro) {
    for (auto& p : this->micros) {
        if (flags.match(p.first) && conditionToAdd.match(p.first)) {
            p.second.back().merge(micro);
        }
    }
    return *this;
}

MicroSequence& MicroSequence::condition(FlagSet flags) {
    this->conditionToAdd = flags;
    return *this;
}

MicroSequence& MicroSequence::elseCondition() {
    this->conditionToAdd = this->conditionToAdd.invert();
    return *this;
}

MicroSequence& MicroSequence::endCondition() {
    this->conditionToAdd = FlagSet{};
    return *this;
}

MicroSequence& MicroSequence::addFetch() {
    return this->addLoadImm(RID::Ins);
}

MicroSequence& MicroSequence::addLoadImm(RID into) {
    return this->add(RID::PcL, RID::MarL)
        .add(RID::PcH, RID::MarH)
        .add(RID::Mem, into, {Line::counter_enable});
}

MicroSequence& MicroSequence::addLoadDirect(RID into) {
    return this->addLoadImm(RID::Temp)
        .addLoadImm(RID::MarH)
        .add(RID::Temp, RID::MarL)
        .add(RID::Mem, into);
}

MicroSequence& MicroSequence::addStore(RID from) {
    return this->addLoadImm(RID::Temp)
        .addLoadImm(RID::MarH)
        .add(RID::Temp, RID::MarL)
        .add(from, RID::Mem);
}*/

Instruction::Instruction(std::string name, AddressingMode mode)
    :   name{name}, mode{mode} {}

Instruction::Instruction() : name{}, mode{} {}

std::ostream& operator<<(std::ostream& stream, Instruction& instruction) {
    std::cout << instruction.name;
    if (instruction.mode.destination.mode != Mode::Implied)
        std::cout << '\t';
    return std::cout << instruction.mode;
}

bool operator<(const Instruction& ins0, const Instruction& ins1) {
    if (ins0.name == ins1.name)
        return ins0.mode < ins1.mode;
    return ins0.name < ins1.name;
}

