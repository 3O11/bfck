#include "Interpreter.hpp"
#include "Utils.hpp"
#include "Instructions.hpp"

#include <print>
#include <vector>

namespace BFCK
{
    namespace
    {
        std::vector<Instruction> Parse(std::string_view code)
        {
            std::string source {code};

            // Remove ignored characters
            std::erase_if(source, [](char c) {
                return std::string("+-<>[],.").find(c) == std::string::npos;
            });

            std::vector<Instruction> instructions;

            // Parse
            if (!source.empty())
            {
                using IType = Instruction::Type;

                Instruction inProgress(IType::Nop);
                std::vector<std::uint64_t> loopBeginStack;

                auto pushPrevInstruction = [&instructions, &inProgress](IType type) {
                    if (inProgress.GetType() != IType::Nop) {
                        instructions.push_back(inProgress);
                    }
                    inProgress = Instruction(type);
                };

                std::uint64_t i = 0;
                while (i < source.size()) {
                    switch (source[i]) {
                        break; case '+': {
                            if (inProgress.GetType() == IType::Increment) {
                                ++inProgress.Value();
                            }
                            else {
                                pushPrevInstruction(IType::Increment);
                            }
                        }
                        break; case '-': {
                            if (inProgress.GetType() == IType::Decrement) {
                                ++inProgress.Value();
                            }
                            else {
                                pushPrevInstruction(IType::Decrement);
                            }
                        }
                        break; case '>': {
                            if (inProgress.GetType() == IType::IncrementPtr) {
                                ++inProgress.Value();
                            }
                            else {
                                pushPrevInstruction(IType::IncrementPtr);
                            }
                        }
                        break; case '<': {
                            if (inProgress.GetType() == IType::DecrementPtr) {
                                ++inProgress.Value();
                            }
                            else {
                                pushPrevInstruction(IType::DecrementPtr);
                            }
                        }
                        break; case '[': {
                            pushPrevInstruction(IType::Nop);
                            instructions.emplace_back(IType::LoopBegin);
                            loopBeginStack.push_back(instructions.size() - 1);
                        }
                        break; case ']': {
                            pushPrevInstruction(IType::Nop);
                            if (instructions.back().GetType() == IType::LoopBegin) {
                                // Empty loops
                                instructions.pop_back();
                                loopBeginStack.pop_back();
                            }
                            else if (instructions.back().GetType() == IType::Decrement
                                 &&  instructions.back().Value() == 1
                                 &&  instructions[instructions.size() - 2].GetType() == IType::LoopBegin) {
                                // Clear loops
                                instructions.pop_back();
                                instructions.pop_back();
                                loopBeginStack.pop_back();
                                instructions.emplace_back(IType::SetValue, 0);
                            }
                            else {
                                std::uint64_t loopLength = instructions.size() - loopBeginStack.back();
                                instructions.emplace_back(IType::LoopEnd, loopLength);
                                instructions[loopBeginStack.back()].Value() = loopLength;
                                loopBeginStack.pop_back();
                            }
                        }
                        break; case '.': {
                            pushPrevInstruction(IType::Nop);
                            instructions.emplace_back(IType::Output);
                        }
                        break; case ',': {
                            pushPrevInstruction(IType::Nop);
                            instructions.emplace_back(IType::Input);
                        }
                    }
                    ++i;
                }
            }

            return instructions;
        }
    }


    void Interpreter::Run(std::string_view code)
    {
        std::vector<Instruction> instructions = Parse(code);

        for (mMemory.ProgramCounter = 0; mMemory.ProgramCounter < instructions.size(); ++mMemory.ProgramCounter) {
            instructions[mMemory.ProgramCounter].Execute(mMemory);
        }
    }


    void Interpreter::RunFile(const fs::path& filepath)
    {
        auto code = Utils::ReadFile(filepath);

        if (!code) {
            switch (code.error()) {
                break; case Utils::FileReadError::FileDoesNotExist:
                    std::print("File does not exist.");
                break; case Utils::FileReadError::PathIsNotAFile:
                    std::print("Path is not a file.");
                break; case Utils::FileReadError::CouldNotOpenFile:
                    std::print("Could not open file.");
            }
            return;
        }

        Run(code.value());
    }


    void Interpreter::Reset()
    {
        mMemory.Reset();
    }
}
