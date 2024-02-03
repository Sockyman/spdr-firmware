#ifndef INSTRUCTION_SET_HPP
#define INSTRUCTION_SET_HPP

#include "Register.hpp"
#include "Line.hpp"
#include "Mode.hpp"
#include "Instruction.hpp"
#include "MicroSequence.hpp"
#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <optional>
#include <span>
#include <memory>

class InstructionSet {
private:
    static std::unique_ptr<InstructionSet> singleInstance;

    std::vector<MicroSequence> instructions{};
    std::map<Instruction, std::size_t> instructionReferences{};

    MicroSequence& instruction(std::string name, AddressingMode mode);
    MicroSequence& interupt();

    void instructionLoads(RID reg);
    void instructionStores(RID reg);
    MicroSequence& instructionTransfer(RID dest, RID source);
    MicroSequence& instructionXch(RID reg0, RID reg1);
    MicroSequence& instructionXchMem(RID reg);

    MicroSequence& instructionJump(std::string name, FlagSet when);

    MicroSequence& instructionPush(RID reg);
    MicroSequence& instructionPop(RID reg);

    MicroSequence& instructionAluBinaryDirect(
        std::string name,
        Line aluLine,
        FlagSet carrySet = FlagSet::never(),
        bool swapA = false,
        bool keepResult = true
    );

    MicroSequence& instructionAluBinaryImm(
        std::string name,
        Line aluLine,
        FlagSet carrySet = FlagSet::never(),
        bool swapA = false,
        bool keepResult = true
    );

    void instructionAluBinary(
        std::string name,
        Line aluLine,
        FlagSet carrySet = FlagSet::never(),
        bool swapA = false,
        bool keepResult = true
    );

    MicroSequence& instructionAluUnary(
        std::string name,
        RID reg,
        Line aluLine,
        FlagSet carrySet = FlagSet::never()
    );

    MicroSequence& instructionAluUnaryMem(
        std::string name,
        Line aluLine,
        FlagSet carrySet = FlagSet::never()
    );

    MicroSequence& instructionAluUnaryIndirect(
        std::string name,
        Line aluLine,
        FlagSet carrySet = FlagSet::never()
    );

public:
    static const InstructionSet& instance();

    std::optional<const MicroSequence*> getInstruction(const Instruction& ins) const;
    std::optional<std::size_t> getOpcode(const Instruction& ins) const;

    std::span<const MicroSequence> getInstructions() const;

    InstructionSet();
};

#endif

