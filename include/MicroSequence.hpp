#ifndef MICROSEQUENCE_HPP
#define MICROSEQUENCE_HPP

#include "Flag.hpp"
#include "Register.hpp"
#include "Line.hpp"
#include "Instruction.hpp"
#include <cstdint>
#include <optional>

class MicroSequence {
private:
public:
    std::map<FlagSet, std::vector<Micro>> micros{};
    FlagSet conditionToAdd{};

    Instruction instruction;

    MicroSequence();

    MicroSequence& condition(FlagSet flags);
    MicroSequence& elseCondition();
    MicroSequence& endCondition();

    MicroSequence& add(Micro micro);
    MicroSequence& add(RID readFrom, RID writeTo, std::set<Line> lines);
    MicroSequence& add(RID readFrom, RID writeTo);
    MicroSequence& add(std::set<Line> lines);

    MicroSequence& with(FlagSet flags, Micro micro);

    MicroSequence& addFetch();
    MicroSequence& addLoadImm(RID into);
    MicroSequence& addLoadDirect(RID into);
    MicroSequence& addStore(RID from);

    MicroSequence& addPushStart();
    MicroSequence& addPush(RID reg);
    MicroSequence& addPushEnd(RID reg);
    MicroSequence& addPopStart();
    MicroSequence& addPop(RID reg);
    MicroSequence& addPopEnd(RID reg);
};

#endif

