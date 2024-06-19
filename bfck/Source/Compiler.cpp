#include "Compiler.hpp"

namespace BFCK
{
    namespace
    {
        template <typename... Ts>
        struct Overload : Ts...
        {
            using Ts::operator()...;
        };

        template <typename... Ts>
        Overload(Ts...) -> Overload<Ts...>;
    }

    IR::Scope Compiler::Parse(std::string_view source)
    {
        IR::Scope scope;

        if (source.empty())
        {
            return scope;
        }

        IR::Instruction inProgress{IR::Nop{}};
        std::vector<IR::Loop> inProgressLoops;

        auto pushPrevInstruction = [&scope, &inProgress, &inProgressLoops](IR::Instruction&& next) {
            if (inProgress.index() != 0) {
                if (inProgressLoops.size()) {
                    inProgressLoops.back().Instructions.push_back(inProgress);
                }
                else {
                    scope.Instructions.push_back(inProgress);
                }
            }

            inProgress = std::move(next);
        };

        std::size_t i = 0;
        while(i < source.size()) {
            switch(source[i]) {
                break; case '+': {
                    if (inProgress.index() == 1) {
                        std::get<1>(inProgress).Value++;
                    }
                    else {
                        pushPrevInstruction(IR::Increment{1});
                    }
                }
                break; case '-': {
                    if (inProgress.index() == 2) {
                        std::get<2>(inProgress).Value++;
                    }
                    else {
                        pushPrevInstruction(IR::Decrement{1});
                    }
                }
                break; case '>': {
                    if (inProgress.index() == 3) {
                        std::get<3>(inProgress).Value++;
                    }
                    else {
                        pushPrevInstruction(IR::IncrementPtr{1});
                    }
                }
                break; case '<': {
                    if (inProgress.index() == 4) {
                        std::get<4>(inProgress).Value++;
                    }
                    else {
                        pushPrevInstruction(IR::DecrementPtr{1});
                    }
                }
                break; case '.': {
                    pushPrevInstruction(IR::Output{});
                }
                break; case ',': {
                    pushPrevInstruction(IR::Input{});
                }
                break; case '[': {
                    pushPrevInstruction(IR::Nop{});
                    inProgressLoops.push_back(IR::Loop{});
                }
                break; case ']': {
                    // TODO: Report error if there are no loops to close
                    // If there's an instruction to push, we need to do it before we move the loop
                    pushPrevInstruction(IR::Nop{});
                    if (inProgressLoops.size()) {
                        pushPrevInstruction(std::move(inProgressLoops.back()));
                        inProgressLoops.pop_back();
                    }
                }
                break; default: {
                    // Ignore others
                }
            }
            ++i;
        }

        // Make sure to consume the last inProgress instruction.
        pushPrevInstruction(IR::Nop{});

        // TODO: Report error if there are unclosed loops

        return scope;
    }

    void Compiler::Optimize(IR::Scope& scope)
    {
        for (auto& instruction : scope.Instructions) {
            if (instruction.index() == 7) { // IR::Loop
                IR::Loop& loop = std::get<7>(instruction);
                if (loop.Instructions.size() == 1) {
                    if (loop.Instructions[0].index() == 1) { // IR::Increment
                        if (std::get<1>(loop.Instructions[0]).Value == 1) {
                            instruction = IR::SetValue{0};
                        }
                    }
                    else if (loop.Instructions[0].index() == 2) { // IR::Decrement
                        if (std::get<2>(loop.Instructions[0]).Value == 1) {
                            instruction = IR::SetValue{0};
                        }
                    }
                }
                else {
                    Optimize(loop);
                }
            }
        }
    }

    void Compiler::Optimize(IR::Loop& loop)
    {
        for (auto& instruction : loop.Instructions) {
            if (instruction.index() == 7) { // IR::Loop
                IR::Loop& innerLoop = std::get<7>(instruction);
                if (innerLoop.Instructions.size() == 1) {
                    if (innerLoop.Instructions[0].index() == 1) { // IR::Increment
                        if (std::get<1>(innerLoop.Instructions[0]).Value == 1) {
                            instruction = IR::SetValue{0};
                        }
                    }
                    else if (innerLoop.Instructions[0].index() == 2) { // IR::Decrement
                        if (std::get<2>(innerLoop.Instructions[0]).Value == 1) {
                            instruction = IR::SetValue{0};
                        }
                    }
                }
                else {
                    Optimize(innerLoop);
                }
            }
        }
    }

    Machine::Program Compiler::EmitInstruction(const IR::Instruction& instruction)
    {
        Machine::Program program;

        std::visit(Overload{
                [&program](IR::Nop) {
                    program.push_back({Machine::Instruction::Type::Nop, 0, 0, 0});
                },
                [&program](IR::Increment inc) {
                    program.push_back({Machine::Instruction::Type::Increment, 0, inc.Value, 0});
                },
                [&program](IR::Decrement dec) {
                    program.push_back({Machine::Instruction::Type::Decrement, 0, dec.Value, 0});
                },
                [&program](IR::IncrementPtr inc) {
                    program.push_back({Machine::Instruction::Type::IncrementPtr, 0, 0, inc.Value});
                },
                [&program](IR::DecrementPtr dec) {
                    program.push_back({Machine::Instruction::Type::DecrementPtr, 0, 0, dec.Value});
                },
                [&program](IR::Output) {
                    program.push_back({Machine::Instruction::Type::Output, 0, 0, 0});
                },
                [&program](IR::Input) {
                    program.push_back({Machine::Instruction::Type::Input, 0, 0, 0});
                },
                [&program, this](IR::Loop loop) {
                    program.push_back({Machine::Instruction::Type::LoopBegin, 0, 0, 0});
                    std::size_t loopBeginIndex = program.size() - 1;
                    std::uint32_t loopOffset = static_cast<std::uint32_t>(program.size());
                    for (const auto& instr : loop.Instructions)
                    {
                        program.append_range(EmitInstruction(instr));
                    }
                    loopOffset = static_cast<std::uint32_t>(program.size() - loopBeginIndex);
                    program[loopBeginIndex].Offset = loopOffset;
                    program.push_back({Machine::Instruction::Type::LoopEnd, 0, 0, loopOffset});
                },
                [&program](IR::SetValue set) {
                    program.push_back({Machine::Instruction::Type::SetValue, 0, set.Value, 0});
                },
                [&program](IR::Copy copy) {
                    program.push_back({Machine::Instruction::Type::Copy, 0, 0, copy.Offset});
                },
                [&program](IR::Multiply mul) {
                    program.push_back({Machine::Instruction::Type::Multiply, 0, mul.Value, mul.Offset});
                },
                [&program](IR::IncrementTo0) {
                    program.push_back({Machine::Instruction::Type::IncrementTo0, 0, 0, 0});
                },
                [&program](IR::DecrementTo0) {
                    program.push_back({Machine::Instruction::Type::DecrementTo0, 0, 0, 0});
                },
                [&program, this](IR::Scope scope) {
                    for (const auto& instr : scope.Instructions) {
                        program.append_range(EmitInstruction(instr));
                    }
                }
            }, instruction);

            return program;
    }

    Machine::Program Compiler::Emit(IR::Scope& scope)
    {
        return EmitInstruction(scope);
    }

    Machine::Program Compiler::Compile(std::string_view source)
    {
        IR::Scope scope = Parse(source);
        Optimize(scope);
        return Emit(scope);
    }
}
