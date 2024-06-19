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

        auto pushPrevInstruction = [&scope, &inProgress, &inProgressLoops](IR::Instruction next) {
            if (inProgress.index() != 0) {
                if (inProgressLoops.size()) {
                    inProgressLoops.back().Instructions.push_back(inProgress);
                }
                else {
                    scope.Instructions.push_back(inProgress);
                }
            }

            inProgress = next;
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
                    pushPrevInstruction(IR::Nop{});
                    scope.Instructions.push_back(IR::Output{});
                }
                break; case ',': {
                    pushPrevInstruction(IR::Nop{});
                    scope.Instructions.push_back(IR::Input{});
                }
                break; case '[': {
                    pushPrevInstruction(IR::Nop{});
                    inProgressLoops.push_back(IR::Loop{});
                }
                break; case ']': {
                    // TODO: Report error if there are no loops to close
                    if (inProgressLoops.size()) {
                        pushPrevInstruction(IR::Nop{});
                        scope.Instructions.push_back(inProgressLoops.back());
                        inProgressLoops.pop_back();
                    }
                }
                break; default: {
                    // Ignore others
                }
            }
            ++i;
        }

        return scope;
    }

    void Compiler::Optimize(IR::Scope& scope)
    {
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
                    std::uint32_t loopOffset = static_cast<std::uint32_t>(loop.Instructions.size());
                    program.push_back({Machine::Instruction::Type::LoopBegin, 0, 0, loopOffset});
                    for (const auto& instr : loop.Instructions)
                    {
                        program.append_range(EmitInstruction(instr));
                    }
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
                    for (const auto& instr : scope.Instructions)
                    {
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
