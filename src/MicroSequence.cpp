#include "MicroSequence.hpp"
#include "Flag.hpp"
#include "Register.hpp"
#include "Line.hpp"
#include "Instruction.hpp"

MicroSequence::MicroSequence() {
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
}


MicroSequence& MicroSequence::addPushStart() {
    return this->add(RID::Alu, RID::MarH, {Line::minus_one})
        .add(RID::Sp, RID::Temp);
}

MicroSequence& MicroSequence::addPush(RID reg) {
    return this->add(RID::Alu, RID::MarL, {Line::dec})
        .add(RID::Alu, RID::Temp, {Line::dec})
        .add(reg, RID::Mem);
}

MicroSequence& MicroSequence::addPushEnd(RID reg) {
    return this->add(RID::Alu, RID::MarL, {Line::dec})
        .add(RID::Alu, RID::Sp, {Line::dec})
        .add(reg, RID::Mem);
}

MicroSequence& MicroSequence::addPopStart() {
    return this->add(RID::Alu, RID::MarH, {Line::minus_one})
        .add(RID::Sp, RID::MarL)
        .add(RID::Sp, RID::Temp);
}

MicroSequence& MicroSequence::addPop(RID reg) {
    return this->add(RID::Mem, reg)
        .add(RID::Alu, RID::MarL, {Line::inc})
        .add(RID::Alu, RID::Temp, {Line::inc});
}

MicroSequence& MicroSequence::addPopEnd(RID reg) {
    return this->add(RID::Mem, reg)
        .add(RID::Alu, RID::Sp, {Line::inc});
}

