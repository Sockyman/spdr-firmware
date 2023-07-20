#include "InstructionSet.hpp"
#include "Mode.hpp"

InstructionSet::InstructionSet() {
    instruction("nop", {});

    instruction("hlt", {})
        .add({Line::halt});

    interupt()
        .addPushStart()
        .addPush(RID::PcL)
        .addPush(RID::PcH)
        .addPushEnd(RID::F)
        .add(RID::Alu, RID::PcL, {Line::zero})
        .add(RID::Alu, RID::Temp, {Line::zero})
        .add(RID::Alu, RID::PcH, {Line::inc})
        .add({Line::nop})
        .add({Line::clear_irq});

    instructionLoads(RID::A);
    instruction("mov", {{RID::A}, {Mode::Indirect, RID::CD}})
        .add(RID::C, RID::MarL)
        .add(RID::D, RID::MarH)
        .add(RID::Mem, RID::A);
    instruction("mov", {{RID::A}, {Mode::Offset, RID::C}})
        .addLoadImm(RID::MarH)
        .add(RID::C, RID::MarL)
        .add(RID::Mem, RID::A);

    instructionLoads(RID::C);
    instructionLoads(RID::D);
    instructionLoads(RID::F);

    instructionStores(RID::A);
    instruction("mov", {{Mode::Offset, RID::C}, {RID::A}})
        .addLoadImm(RID::MarH)
        .add(RID::C, RID::MarL)
        .add(RID::A, RID::Mem);
    instruction("mov", {{Mode::Indirect, RID::CD}, {RID::A}})
        .add(RID::C, RID::MarL)
        .add(RID::D, RID::MarH)
        .add(RID::A, RID::Mem);

    instructionStores(RID::C);
    instructionStores(RID::D);

    instructionTransfer(RID::C, RID::A);
    instructionTransfer(RID::A, RID::C);
    instructionTransfer(RID::D, RID::A);
    instructionTransfer(RID::A, RID::D);
    instructionTransfer(RID::D, RID::C);
    instructionTransfer(RID::C, RID::D);

    instructionTransfer(RID::F, RID::A);
    instructionTransfer(RID::A, RID::F);
    instructionTransfer(RID::Sp, RID::A);
    instructionTransfer(RID::A, RID::Sp);

    instructionXch(RID::A, RID::C);
    instructionXch(RID::A, RID::D);
    instructionXch(RID::C, RID::D);

    instructionXchMem(RID::A);
    instructionXchMem(RID::C);
    instructionXchMem(RID::D);


    instructionAluBinary("add", false, true, Line::add, false);
    instructionAluBinary("addc", false, true, Line::add, Flag::C);

    instructionAluBinary("sub", true, true, Line::sub, true);
    instructionAluBinary("subb", true, true, Line::sub, Flag::C);

    instructionAluBinaryDirect("cmp", true, false, Line::sub, true)
        .add(RID::Temp, RID::A);
    instructionAluBinaryImm("cmp", true, false, Line::sub, true)
        .add(RID::Temp, RID::A);

    instructionAluBinaryDirect("cmpb", true, false, Line::sub, Flag::C)
        .add(RID::Temp, RID::A);
    instructionAluBinaryImm("cmpb", true, false, Line::sub, Flag::C)
        .add(RID::Temp, RID::A);

    instructionAluUnary("not", RID::A, Line::b_not, false);

    instructionAluBinary("and", false, true, Line::b_and, false);
    instructionAluBinary("or", false, true, Line::b_or, false);
    instructionAluBinary("xor", false, true, Line::b_xor, false);
    instructionAluBinary("bit", false, false, Line::b_and, false);

    instructionAluUnary("inc", RID::A, Line::inc, false);
    instructionAluUnary("dec", RID::A, Line::dec, false);
    instructionAluUnary("inc", RID::C, Line::inc, false);
    instructionAluUnary("dec", RID::C, Line::dec, false);
    instructionAluUnary("inc", RID::D, Line::inc, false);
    instructionAluUnary("dec", RID::D, Line::dec, false);

    instructionAluUnaryMem("inc", Line::inc, false);
    instructionAluUnaryMem("dec", Line::dec, false);
    instructionAluUnaryIndirect("inc", Line::inc, false);
    instructionAluUnaryIndirect("dec", Line::dec, false);

    instructionAluUnary("incc", RID::A, Line::passthrough, Flag::C);
    instructionAluUnary("incc", RID::C, Line::passthrough, Flag::C);
    instructionAluUnary("incc", RID::D, Line::passthrough, Flag::C);

    instructionAluUnary("decb", RID::A, Line::dec, Flag::C);
    instructionAluUnary("decb", RID::C, Line::dec, Flag::C);
    instructionAluUnary("decb", RID::D, Line::dec, Flag::C);

    instructionAluUnaryMem("incc", Line::passthrough, Flag::C);
    instructionAluUnaryMem("decb", Line::dec, Flag::C);
    instructionAluUnaryIndirect("incc", Line::passthrough, Flag::C);
    instructionAluUnaryIndirect("decb", Line::dec, Flag::C);

    instructionAluUnary("shl", RID::A, Line::lsh, false);
    instructionAluUnary("rol", RID::A, Line::lsh, Flag::C);

    instruction("shr", {{RID::A}})
        .add(RID::A, RID::Temp)
        .add(RID::Alu, RID::Temp, {Line::lsh, Line::flip, Line::set_flags})
        .add(RID::Alu, RID::A, {Line::passthrough, Line::flip});

    instruction("ror", {{RID::A}})
        .add(RID::A, RID::Temp)
        .add(RID::Alu, RID::Temp, {Line::lsh, Line::flip, Line::set_flags})
            .with(Flag::C, {{Line::carry}})
        .add(RID::Alu, RID::A, {Line::passthrough, Line::flip});

    instructionAluUnaryMem("shl", Line::lsh, false);
    instructionAluUnaryMem("rol", Line::lsh, Flag::C);

    instruction("shr", {{Mode::Direct}})
        .addLoadDirect(RID::Temp)
        .add(RID::Alu, RID::Temp, {Line::lsh, Line::flip, Line::set_flags})
        .add(RID::Alu, RID::Temp, {Line::passthrough, Line::flip})
        .add(RID::Temp, RID::Mem);

    instruction("ror", {{RID::A}})
        .addLoadDirect(RID::Temp)
        .add(RID::Alu, RID::Temp, {Line::lsh, Line::flip, Line::set_flags})
            .with(Flag::C, {{Line::carry}})
        .add(RID::Alu, RID::Temp, {Line::passthrough, Line::flip})
        .add(RID::Temp, RID::Mem);

    instructionJump("jmp", true);

    instruction("jmp", {{Mode::ImmediateAddress}})
        .add(RID::C, RID::PcL)
        .add(RID::D, RID::PcH);

    instructionJump("jz", Flag::Z);
    instructionJump("jz", Flag::NZ);
    instructionJump("jc", Flag::C);
    instructionJump("jnc", Flag::NC);
    instructionJump("jn", Flag::S);
    instructionJump("jp", Flag::NS);

    instructionPush(RID::A);
    instructionPush(RID::C);
    instructionPush(RID::D);
    instructionPush(RID::F);

    instructionPop(RID::A);
    instructionPop(RID::C);
    instructionPop(RID::D);
    instructionPop(RID::F);

    instruction("phc", {})
        .addPushStart()
        .addPush(RID::PcL)
        .addPushEnd(RID::PcH);

    instruction("ret", {})
        .addPopStart()
        .addPop(RID::PcH)
        .addPopEnd(RID::PcL)
            .with(true, {{Line::counter_enable}})
        .add({Line::counter_enable})
        .add({Line::counter_enable});

    instruction("rti", {})
        .addPopStart()
        .addPop(RID::F)
        .addPop(RID::PcH)
        .addPopEnd(RID::PcL);

    instruction("out", {{RID::A}})
        .add(RID::A, RID::Out);

    instruction("in", {{RID::A}})
        .add(RID::Kbd, RID::A);

    instruction("lcd", {{RID::A}})
        .add(RID::A, RID::Lcd, {Line::logic});

    instruction("lcc", {{RID::A}})
        .add(RID::A, RID::Lcd);


    /*for (int i = 0; i < instructions.size(); ++i) {
        auto ins = instructions[i];
        std::cout << std::hex << std::setw(2) << std::setfill('0') << i << " " << ins.instruction;
        //for (auto j : ins.micros[Flag::NC | Flag::NZ | Flag::NS]) {
        //    std::cout << j << ", ";
        //}
        std::cout << '\n';
    }*/

}

