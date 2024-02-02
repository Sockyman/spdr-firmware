#ifndef MODE_HPP
#define MODE_HPP

#include "Register.hpp"
#include <iostream>
#include <cstdint>

enum class Mode {
    Implied,
    Register,
    Direct,
    Immediate,
    ImmediateAddress,
    Indirect,
    Offset,
};

enum class Size {
    Unsized = 0,
    Byte = 1,
    Word = 2,
    Page,
};

int getSize(Size size);

std::ostream& operator<<(std::ostream& stream, Size size);

class ModeData {
public:
    static const ModeData& get(Mode mode);
    int dataSize;
    bool usesRegister;
    bool isDirect;

    Size defaultSize;

    ModeData(int dataSize, bool usesRegister, bool isDirect, Size defaultSize);
    ModeData();
};

class Address {
public:
    Mode mode;
    RID reg;

    Address(Mode mode);
    Address(RID reg);
    Address(Mode mode, RID reg);
    Address();

    std::ostream& print(std::ostream& stream, const std::string& argument) const;
};

std::ostream& operator<<(std::ostream& stream, const Address& address);

bool modeEqual(const Mode& mode0, const Mode& mode1);

std::strong_ordering operator<=>(const Address& address0, const Address& address1);
bool operator==(const Address& address0, const Address& address1);

class SizedAddress {
public:
    Address address;
    Size size;

    SizedAddress();
    SizedAddress(Mode mode);
    SizedAddress(RID reg);
    SizedAddress(Mode mode, RID reg);
    SizedAddress(Address address);
    SizedAddress(Address address, Size size);

    bool isImplied() const;

    bool operator==(const SizedAddress& other) const = default;
    auto operator<=>(const SizedAddress& other) const = default;

    std::ostream& print(std::ostream& stream, std::uint16_t argument) const;
    std::ostream& print(std::ostream& stream, const std::string& argument) const;
};

std::ostream& operator<<(std::ostream& stream, const SizedAddress& sizedAddress);

class AddressingMode {
private:
public:
    SizedAddress destination;
    SizedAddress source;

    AddressingMode();
    AddressingMode(SizedAddress destination, SizedAddress source);
    AddressingMode(SizedAddress destination);

    std::ostream& print(std::ostream& stream, std::uint16_t argument) const;
    std::ostream& print(std::ostream& stream, const std::string& argument) const;
};

std::ostream& operator<<(std::ostream& stream, const AddressingMode& mode);

bool operator==(const AddressingMode& mode0, const AddressingMode& mode1);
std::strong_ordering operator<=>(const AddressingMode& mode0, const AddressingMode& mode1);

#endif

