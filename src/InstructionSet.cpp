#include "InstructionSet.hpp"
#include "Mode.hpp"
#include <iomanip>
#include <stdexcept>

MicroSequence& InstructionSet::instruction(
    std::string name,
    AddressingMode mode
) {
    MicroSequence& micros = this->interupt();
    Instruction ins{name, mode};

    micros.instruction = ins;

    std::size_t opcode = this->instructions.size() - 1;
    instructionReferences[ins] = opcode;
    micros.opcode = opcode;

    return micros.addFetch();
}

MicroSequence& InstructionSet::interupt() {
    MicroSequence ins{};
    this->instructions.push_back(ins);
    return this->instructions.back();
}

void InstructionSet::instructionLoads(RID reg) {
    instruction("mov", {{reg}, {Mode::Direct}})
        .addLoadDirect(reg);
    instruction("mov", {{reg}, {Mode::Immediate}})
        .addLoadImm(reg);
}

void InstructionSet::instructionStores(RID reg) {
    instruction("mov", {{Mode::Direct}, {reg}})
        .addStore(reg);
}

MicroSequence& InstructionSet::instructionTransfer(RID dest, RID source) {
    return instruction("mov", {{dest}, {source}})
        .add(source, dest);
}

MicroSequence& InstructionSet::instructionXch(RID reg0, RID reg1) {
    return instruction("xch", {{reg0}, {reg1}})
        .add(reg0, RID::Temp)
        .add(reg1, reg0)
        .add(RID::Temp, reg1);
}

MicroSequence& InstructionSet::instructionXchMem(RID reg) {
    return instruction("xch", {{reg}, {Mode::Direct}})
        .addLoadDirect(RID::Temp)
        .add(reg, RID::Mem)
        .add(RID::Temp, reg);
}

MicroSequence& InstructionSet::instructionJump(std::string name, FlagSet when) {
    return instruction(name, {{Mode::ImmediateAddress}})
        .condition(when)
            .addLoadImm(RID::Temp)
            .addLoadImm(RID::PcH)
            .add(RID::Temp, RID::PcL)
        .elseCondition()
            .add({Line::counter_enable})
            .add({Line::counter_enable})
        .endCondition();
}

MicroSequence& InstructionSet::instructionPush(RID reg) {
    return instruction("push", {{reg}})
        .addPushStart()
        .addPushEnd(reg);
}

MicroSequence& InstructionSet::instructionPop(RID reg) {
    return instruction("pop", {{reg}})
        .addPopStart()
        .addPopEnd(reg);
}

MicroSequence& InstructionSet::instructionAluBinaryDirect(
    std::string name,
    bool swapA,
    bool keepResult,
    Line aluLine,
    FlagSet carrySet
) {
    return instruction(name, {{RID::A}, {Mode::Direct}})
        .condition(swapA)
            .add(RID::A, RID::Temp)
            .add(RID::PcL, RID::MarL)
            .add(RID::PcH, RID::MarH)
            .add(RID::Mem, RID::A, {Line::counter_enable})
            .add(RID::PcL, RID::MarL)
            .add(RID::PcH, RID::MarH)
            .add(RID::Mem, RID::MarH, {Line::counter_enable})
            .add(RID::A, RID::MarL)
            .add(RID::Mem, RID::A)
        .elseCondition()
            .addLoadDirect(RID::Temp)
        .endCondition()
        .add({aluLine, Line::set_flags})
            .with(carrySet, {{Line::carry}}).with(keepResult, {RID::Alu, RID::A});
}

MicroSequence& InstructionSet::instructionAluBinaryImm(std::string name, bool swapA, bool keepResult, Line aluLine, FlagSet carrySet) {
    return instruction(name, {{RID::A}, {Mode::Immediate}})
        .condition(swapA)
            .add(RID::A, RID::Temp)
            .addLoadImm(RID::A)
        .elseCondition()
            .addLoadImm(RID::Temp)
        .endCondition()
        .add({aluLine, Line::set_flags})
            .with(carrySet, {{Line::carry}}).with(keepResult, {RID::Alu, RID::A});
}

void InstructionSet::instructionAluBinary(std::string name, bool swapA, bool keepResult, Line aluLine, FlagSet carrySet) {
    instructionAluBinaryDirect(name, swapA, keepResult, aluLine, carrySet);
    instructionAluBinaryImm(name, swapA, keepResult, aluLine, carrySet);
}

MicroSequence& InstructionSet::instructionAluUnary(std::string name, RID reg, Line aluLine, FlagSet carrySet) {
    return instruction(name, {{reg}})
        .add(reg, RID::Temp)
        .add(RID::Alu, reg, {aluLine, Line::set_flags})
            .with(carrySet, {{Line::carry}});

}

MicroSequence& InstructionSet::instructionAluUnaryMem(std::string name, Line aluLine, FlagSet carrySet) {
    return instruction(name, {{Mode::Direct}})
        .addLoadDirect(RID::Temp)
        .add(RID::Alu, RID::Temp, {aluLine, Line::set_flags})
            .with(carrySet, {{Line::carry}})
        .add(RID::Temp, RID::Mem);

}

MicroSequence& InstructionSet::instructionAluUnaryIndirect(std::string name, Line aluLine, FlagSet carrySet) {
    return instruction(name, {{Mode::Indirect, RID::CD}})
        .add(RID::C, RID::MarL)
        .add(RID::D, RID::MarH)
        .add(RID::Mem, RID::Temp)
        .add(RID::Alu, RID::Temp, {aluLine, Line::set_flags})
            .with(carrySet, {{Line::carry}})
        .add(RID::Temp, RID::Mem);
}

MicroSequence* InstructionSet::getInstruction(Instruction ins) {
    if (this->instructionReferences.count(ins))
        return &instructions[instructionReferences[ins]];
    throw std::invalid_argument("instruction not found");
}

