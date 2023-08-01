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

class InstructionSet {
private:
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
    std::vector<MicroSequence> instructions{};
    std::map<Instruction, std::size_t> instructionReferences{};

    MicroSequence* getInstruction(Instruction ins);

    InstructionSet();
};

#endif

