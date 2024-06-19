#include "Machine.hpp"

namespace BFCK
{
    void Machine::Execute(Instruction instr)
    {
        switch (instr.Identifier) {
            break; case Instruction::Type::Nop: {
            }
            break; case Instruction::Type::Increment: {
                mState.Memory[mState.Pointer + instr.Offset] += instr.Value;
            }
            break; case Instruction::Type::Decrement: {
                mState.Memory[mState.Pointer + instr.Offset] -= instr.Value;
            }
            break; case Instruction::Type::IncrementPtr: {
                mState.Pointer += instr.Offset;
            }
            break; case Instruction::Type::DecrementPtr: {
                mState.Pointer -= instr.Offset;
            }
            break; case Instruction::Type::Output: {
                std::putchar(mState.Memory[mState.Pointer + instr.Offset]);
            }
            break; case Instruction::Type::Input: {
                mState.Memory[mState.Pointer + instr.Offset] = std::getchar();
            }
            break; case Instruction::Type::LoopBegin: {
                if (!mState.Memory[mState.Pointer]) {
                    mState.ProgramCounter += instr.Offset;
                }
            }
            break; case Instruction::Type::LoopEnd: {
                if (mState.Memory[mState.Pointer]) {
                    mState.ProgramCounter -= instr.Offset;
                }
            }
            break; case Instruction::Type::SetValue: {
                mState.Memory[mState.Pointer] = instr.Value;
            }
            break; case Instruction::Type::Copy: {
                mState.Memory[mState.Pointer + instr.Offset] += mState.Memory[mState.Pointer];
            }
            break; case Instruction::Type::Multiply: {
                mState.Memory[mState.Pointer + instr.Offset] += mState.Memory[mState.Pointer] * instr.Value;
            }
            break; case Instruction::Type::IncrementTo0: {
                // Replace with a more efficient implementation
                // - preferably not strchr
                while (mState.Memory[mState.Pointer]) {
                    ++mState.Pointer;
                }
            }
            break; case Instruction::Type::DecrementTo0: {
                // Replace with a more efficient implementation
                // - preferably not strrchr
                while (mState.Memory[mState.Pointer]) {
                    --mState.Pointer;
                }
            }
        }
    }

    void Machine::Execute(Program program)
    {
        for (mState.ProgramCounter = 0; mState.ProgramCounter < program.size(); ++mState.ProgramCounter) {
            Execute(program[mState.ProgramCounter]);
        }
    }
}
