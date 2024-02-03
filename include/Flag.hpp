#ifndef FLAG_HPP
#define FLAG_HPP

#include <map>
#include <tuple>
#include <set>
#include <iostream>
#include <cstdint>

constexpr int flagCount{3};

enum class Flag {
    C = 0b1,
    Z = 0b10,
    S = 0b100,
    NC = 0b1000,
    NZ = 0b10000,
    NS = 0b100000,
};

Flag invertFlag(Flag flag);
std::ostream& operator<<(std::ostream& stream, Flag flag);

class FlagSet {
private:
public:
    static std::set<FlagSet> getAllCombinations();
    static FlagSet clear();
    static FlagSet set();

    std::set<Flag> flags{};
    bool noMatchOnEmpty;

    FlagSet();
    FlagSet(std::set<Flag> flags, bool noMatchOnEmpty);
    FlagSet(Flag flag);
    explicit FlagSet(bool value);
    //FlagSet(std::uint16_t value);

    FlagSet invert();
    bool match(const FlagSet flags);

    std::size_t intoInt();

    FlagSet operator|(const Flag flag);
    FlagSet operator|(const FlagSet flags);

    static FlagSet always();
    static FlagSet never();
};

std::uint32_t asBitFlags(const FlagSet flags);

bool operator<(const FlagSet flags0, const FlagSet flags1);

FlagSet operator|(const Flag flags0, const Flag flags1);
std::ostream& operator<<(std::ostream& stream, FlagSet flags);

#endif

