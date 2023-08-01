#ifndef MODE_HPP
#define MODE_HPP

#include "Register.hpp"
#include <iostream>

enum class Mode {
    Implied,
    Register,
    Direct,
    Immediate,
    ImmediateAddress,
    Indirect,
    Offset,
};

class ModeData {
public:
    static const ModeData& get(Mode mode);
    int dataSize;
    bool usesRegister;
    bool isDirect;

    ModeData(int dataSize, bool usesRegister, bool isDirect);
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
};

std::ostream& operator<<(std::ostream& stream, const Address& address);

bool modeEqual(const Mode& mode0, const Mode& mode1);

bool operator<(const Address& address0, const Address& address1);
bool operator==(const Address& address0, const Address& address1);

class AddressingMode {
private:
public:
    Address destination;
    Address source;

    AddressingMode();
    AddressingMode(Address destination, Address source);
    AddressingMode(Address destination);
};

std::ostream& operator<<(std::ostream& stream, const AddressingMode& mode);

bool operator==(const AddressingMode& mode0, const AddressingMode& mode1);
bool operator<(const AddressingMode& mode0, const AddressingMode& mode1);

#endif

