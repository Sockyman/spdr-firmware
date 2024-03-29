#include "Flag.hpp"
#include <stdexcept>
#include <string>
#include <tuple>

const std::map<Flag, Flag> flagPairs {
    {Flag::C, Flag::NC},
    {Flag::Z, Flag::NZ},
    {Flag::S, Flag::NS},
};


std::set<FlagSet> getCombinations(
    FlagSet start,
    std::map<Flag, Flag>::const_iterator it
);

std::set<FlagSet> getCombinations(
    FlagSet start,
    std::map<Flag, Flag>::const_iterator it,
    Flag flag
) {
    start.flags.insert(flag);
    if (++it == flagPairs.cend()) {
        return {start};
    }
    return getCombinations(start, it);
}

std::set<FlagSet> getCombinations(
    FlagSet start,
    std::map<Flag, Flag>::const_iterator it
) {
    auto s = getCombinations(start, it, it->second);
    s.merge(getCombinations(start, it, it->first));
    return s;
}

std::set<FlagSet> FlagSet::getAllCombinations() {
    static auto combinations = getCombinations(FlagSet{}, flagPairs.cbegin());
    return combinations;
}


Flag invertFlag(Flag flag) {
    for (auto pair : flagPairs) {
        if (pair.first == flag)
            return pair.second;
        if (pair.second == flag)
            return pair.first;
    }
    throw std::invalid_argument{"invertFlag: invalid flag"};
}

std::ostream& operator<<(std::ostream& stream, Flag flag) {
    static std::map<Flag, std::string> flagNames {
        {Flag::C, "cf"},
        {Flag::Z, "zf"},
        {Flag::S, "sf"},
        {Flag::NC, "nc"},
        {Flag::NZ, "nz"},
        {Flag::NS, "ns"},
    };
    return stream << flagNames[flag];
}

FlagSet FlagSet::clear() {
    return FlagSet::set().invert();
}

FlagSet FlagSet::set() {
    FlagSet flags{};
    for (auto flag : flagPairs) {
        flags.flags.insert(flag.first);
    }
    return flags;
}

FlagSet::FlagSet() : FlagSet{{}, false} {}

FlagSet::FlagSet(std::set<Flag> flags, bool noMatchOnEmpty)
: flags{flags}, noMatchOnEmpty{noMatchOnEmpty} {

}

FlagSet::FlagSet(Flag flag) : FlagSet{{flag}, false} {}

FlagSet::FlagSet(bool value) : FlagSet{{}, !value} {}

FlagSet FlagSet::always() {
    return FlagSet{true};
}

FlagSet FlagSet::never() {
    return FlagSet{false};
}

/*FlagSet::FlagSet(std::uint16_t value) : flags{}, noMatchOnEmpty{false} {
    for (auto& flag : {
        std::pair{0b100, Flag::Z},
        std::pair{0b1000, Flag::C},
        std::pair{0b10, Flag::S}
    }) {
        if (value & flag.first) {
            this->flags.insert(flag.second);
        } else {
            this->flags.insert(invertFlag(flag.second));
        }
    }
}*/

FlagSet FlagSet::invert() {
    FlagSet flags{};
    flags.noMatchOnEmpty = !this->noMatchOnEmpty;
    for (Flag flag : this->flags) {
        flags = flags | invertFlag(flag);
    }
    return flags;
}

bool FlagSet::match(const FlagSet flags) {
    if ((this->flags.empty() && this->noMatchOnEmpty) 
        || (flags.flags.empty() && flags.noMatchOnEmpty)) {
        return false;
    }

    for (Flag flag : flags.flags) {
        if (this->flags.count(invertFlag(flag))) {
            return false;
        }
    }
    return true;
}

FlagSet FlagSet::operator|(const Flag flag) {
    return *this | FlagSet(flag);
}

FlagSet FlagSet::operator|(const FlagSet flags) {
    FlagSet newFlags = *this;
    for (Flag flag : flags.flags) {
        if (newFlags.flags.count(flag) == 0) {
            newFlags.flags.insert(flag);
        }
    }
    return newFlags;
}

std::size_t FlagSet::intoInt() {
    std::size_t value = 0;
    for (auto flag : this->flags) {
        value |= static_cast<std::size_t>(flag);
    }
    return (value) & 0b111;
}

FlagSet operator|(const Flag flags0, const Flag flags1) {
    return FlagSet(flags0) | FlagSet(flags1);
}

std::uint32_t asBitFlags(const FlagSet flags) {
    std::uint32_t bitFlags = 0;
    for (auto flag : flags.flags) {
        bitFlags |= static_cast<std::uint32_t>(flag);
    }
    return bitFlags;
}

bool operator<(const FlagSet flags0, const FlagSet flags1) {
    return asBitFlags(flags0) < asBitFlags(flags1);
}

std::ostream& operator<<(std::ostream& stream, FlagSet flags) {
    stream << "{ ";
    for (Flag flag : flags.flags) {
        stream << flag << ' ';
    }
    return stream << '}';
}

