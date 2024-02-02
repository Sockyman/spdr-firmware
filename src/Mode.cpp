#include "Mode.hpp"
#include <map>
#include <format>

int getSize(Size size) {
    switch (size) {
        case Size::Unsized:
            return 0;
        case Size::Byte:
        case Size::Page:
            return 1;
        case Size::Word:
            return 2;
    }
}

std::ostream& operator<<(std::ostream& stream, Size size) {
    switch (size) {
        case Size::Byte:
            return stream << "byte";
        case Size::Word:
            return stream << "word";
        case Size::Page:
            return stream << "page";
        case Size::Unsized:
            return stream << "unsized";
    }
    // TODO: Error
    throw;
}

ModeData::ModeData(int dataSize, bool usesRegister, bool isDirect, Size defaultSize)
    :   dataSize{dataSize},
        usesRegister{usesRegister},
        isDirect{isDirect},
        defaultSize{defaultSize} {}

const ModeData& ModeData::get(Mode mode) {
    static std::map<Mode, ModeData> modeDatas {
        {Mode::Implied, {0, false, false, Size::Unsized}},
        {Mode::Register, {0, true, false, Size::Unsized}},
        {Mode::Direct, {2, false, true, Size::Word}},
        {Mode::Immediate, {1, false, false, Size::Byte}},
        {Mode::ImmediateAddress, {2, false, false, Size::Word}},
        {Mode::Indirect, {0, true, true, Size::Unsized}},
        {Mode::Offset, {1, true, true, Size::Page}},
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

std::ostream& Address::print(std::ostream& stream, const std::string& argument) const {
    switch (this->mode) {
        case Mode::Implied:
            break;
        case Mode::Register:
            stream << this->reg;
            break;
        case Mode::Direct:
            stream << '[' << argument << ']';
            break;
        case Mode::ImmediateAddress:
        case Mode::Immediate:
            stream << argument;
            break;
        case Mode::Indirect:
            stream << '[' << this->reg << ']';
            break;
        case Mode::Offset:
            stream << '[' << this->reg << ", " << argument << ']';
            break;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Address& address) {
    ModeData modeData = ModeData::get(address.mode);
    return address.print(stream, (modeData.dataSize == 2) ? "addr" : "imm");

    if (address.mode == Mode::Implied)
        return stream;

    if (modeData.isDirect) {
        stream << "[";
    }

    if (modeData.usesRegister) {
        stream << address.reg;
        if (modeData.dataSize) {
            stream << ", ";
        }
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

    if (modeData.isDirect) {
        stream << "]";
    }
    return stream;
}

std::strong_ordering operator<=>(
    const Address& address0,
    const Address& address1
) {
    if (address0.mode == address1.mode) {
        return address0.reg <=> address1.reg;
    }
    return address0.mode <=> address1.mode;
}

bool operator==(const Address& address0, const Address& address1) {
    return address0.mode == address1.mode && address0.reg == address1.reg;
}

SizedAddress::SizedAddress()
    : SizedAddress{Address{}} {}

SizedAddress::SizedAddress(Mode mode)
    : SizedAddress{Address{mode}} {}

SizedAddress::SizedAddress(RID reg)
    : SizedAddress{Address{reg}} {}

SizedAddress::SizedAddress(Mode mode, RID reg)
    : SizedAddress{{mode, reg}} {}

SizedAddress::SizedAddress(Address address)
    : SizedAddress{address, ModeData::get(address.mode).defaultSize} {}

SizedAddress::SizedAddress(Address address, Size size)
: address{address}, size{size} {
    /*Size defaultSize = ModeData::get(address.mode).defaultSize;
    if ((defaultSize == Size::Unsized && size != Size::Unsized)
            || (defaultSize != Size::Unsized && size == Size::Unsized)
        ) {
        // TODO: Error stuff.
        throw std::runtime_error{"bad sizing"};
    }*/
}

std::ostream& SizedAddress::print(std::ostream& stream, std::uint16_t argument) const {
    std::string arg = "";
    switch (this->size) {
        case Size::Byte:
        case Size::Page:
            arg = std::format("{:02x}", argument & 0xff);
            break;
        case Size::Word:
            arg = std::format("{:04x}", argument);
            break;
        default:
            break;
    }
    return this->address.print(stream, arg);
}

std::ostream& SizedAddress::print(std::ostream& stream, const std::string& argument) const {
    return this->address.print(stream, argument);
}

std::ostream& operator<<(
    std::ostream& stream,
    const SizedAddress& sizedAddress
) {
    return stream << sizedAddress.address << " (" << sizedAddress.size << ")";
}

bool SizedAddress::isImplied() const {
    return this->address.mode == Mode::Implied;
}

AddressingMode::AddressingMode(SizedAddress destination, SizedAddress source)
    :   destination{destination},
        source{source} {
    if (destination.address.mode == Mode::Implied 
            && source.address.mode != Mode::Implied
        ) {
        throw; // TODO: Error stuff.
    }
}

AddressingMode::AddressingMode()
    :   AddressingMode{{Mode::Implied}, {Mode::Implied}} {}

AddressingMode::AddressingMode(SizedAddress destination) 
    :   AddressingMode{destination, {Mode::Implied}} {}


std::ostream& AddressingMode::print(std::ostream& stream, std::uint16_t argument) const {
    this->destination.print(stream, argument);
    if (!this->source.isImplied()) {
        stream << ", ";
        this->source.print(stream, argument);
    }
    return stream;
}

std::ostream& AddressingMode::print(std::ostream& stream, const std::string& argument) const {
    this->destination.address.print(stream, argument);
    if (!this->source.isImplied()) {
        stream << ", ";
        this->source.address.print(stream, argument);
    }
    return stream;
}


std::ostream& operator<<(std::ostream& stream, const AddressingMode& mode) {
    stream << mode.destination.address;
    if (!mode.source.isImplied()) {
        stream << ", " << mode.source.address;
    }
    return stream;
}

bool operator==(const AddressingMode& mode0, const AddressingMode& mode1) {
    return mode0.destination == mode1.destination
        && mode0.source == mode1.source
        && mode0.destination == mode1.destination;
}

std::strong_ordering operator<=>(
    const AddressingMode& mode0,
    const AddressingMode& mode1
) {
    if (mode0.destination == mode1.destination)
        return mode0.source <=> mode1.source;
    return mode0.destination <=> mode1.destination;
}

