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
        bool swapA,
        bool keepResult,
        Line aluLine,
        FlagSet carrySet
    );

    MicroSequence& instructionAluBinaryImm(
        std::string name,
        bool swapA,
        bool keepResult,
        Line aluLine,
        FlagSet carrySet
    );

    void instructionAluBinary(
        std::string name,
        bool swapA,
        bool keepResult,
        Line aluLine,
        FlagSet carrySet
    );

    MicroSequence& instructionAluUnary(
        std::string name,
        RID reg,
        Line aluLine,
        FlagSet carrySet
    );

    MicroSequence& instructionAluUnaryMem(
        std::string name,
        Line aluLine,
        FlagSet carrySet
    );

    MicroSequence& instructionAluUnaryIndirect(
        std::string name,
        Line aluLine,
        FlagSet carrySet
    );

public:
    static const InstructionSet& instance();

    std::optional<const MicroSequence*> getInstruction(const Instruction& ins) const;
    std::optional<std::size_t> getOpcode(const Instruction& ins) const;

    std::span<const MicroSequence> getInstructions() const;

    InstructionSet();
};

#endif

