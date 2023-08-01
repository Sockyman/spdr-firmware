#include "Mode.hpp"
#include <map>

ModeData::ModeData(int dataSize, bool usesRegister, bool isDirect)
    :   dataSize{dataSize},
        usesRegister{usesRegister},
        isDirect{isDirect} {}

const ModeData& ModeData::get(Mode mode) {
    static std::map<Mode, ModeData> modeDatas {
        {Mode::Implied, {0, false, false}},
        {Mode::Register, {0, true, false}},
        {Mode::Direct, {2, false, true}},
        {Mode::Immediate, {1, false, false}},
        {Mode::ImmediateAddress, {2, false, false}},
        {Mode::Indirect, {0, true, true}},
        {Mode::Offset, {1, true, true}},
    };
    return modeDatas[mode];
}

ModeData::ModeData() {}

Address::Address(Mode mode) : mode{mode}, reg{RID::A} {
    if (ModeData::get(mode).usesRegister) {
        throw; // TODO: Error stuff.
    }
}

Address::Address(RID reg) : Address{Mode::Register, reg} {}

Address::Address(Mode mode, RID reg) : mode{mode}, reg{reg} {
    if (!ModeData::get(mode).usesRegister) {
        throw; // TODO: Error stuff.
    }
}

Address::Address() : Address{Mode::Implied} {}

std::ostream& operator<<(std::ostream& stream, const Address& address) {
    if (address.mode == Mode::Implied)
        return stream;

    ModeData modeData = ModeData::get(address.mode);
    if (modeData.isDirect)
        stream << "[";

    if (modeData.usesRegister) {
        stream << address.reg;
        if (modeData.dataSize)
            stream << ", ";
    }

    switch (modeData.dataSize) {
        case 1:
            stream << "imm";
            break;
        case 2:
            stream << "addr";
            break;
        default:
            break;
    }

    if (modeData.isDirect)
        stream << "]";
    return stream;
}

bool operator<(const Address& address0, const Address& address1) {
    if (address0.mode == address1.mode)
        return address0.reg < address1.reg;
    return address0.mode < address1.mode;
}

bool operator==(const Address& address0, const Address& address1) {
    return address0.mode == address1.mode && address0.reg == address1.reg;
}

AddressingMode::AddressingMode(Address destination, Address source)
    :   destination{destination},
        source{source} {
    if (destination.mode == Mode::Implied && source.mode != Mode::Implied) {
        throw; // TODO: Error stuff.
    }
}

AddressingMode::AddressingMode()
    :   AddressingMode{{Mode::Implied}, {Mode::Implied}} {}

AddressingMode::AddressingMode(Address destination) 
    :   AddressingMode{destination, {Mode::Implied}} {}

std::ostream& operator<<(std::ostream& stream, const AddressingMode& mode) {
    stream << mode.destination;
    if (mode.source.mode != Mode::Implied) {
        stream << ", " << mode.source;
    }
    return stream;
}

bool operator==(const AddressingMode& mode0, const AddressingMode& mode1) {
    return mode0.destination == mode1.destination
        && mode0.source == mode1.source
        && mode0.destination == mode1.destination;
}

bool operator<(const AddressingMode& mode0, const AddressingMode& mode1) {
    if (mode0.destination == mode1.destination)
        return mode0.source < mode1.source;
    return mode0.destination < mode1.destination;
}

