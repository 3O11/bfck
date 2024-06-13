#ifndef BFCK_INSTRUCTIONS_HPP
#define BFCK_INSTRUCTIONS_HPP

#include "Base.hpp"
#include "Memory.hpp"

#include <cstdio>

namespace BFCK
{
    class Instruction
    {
    public:
        enum class Type : std::uint64_t
        {
            Nop,
            Increment,
            Decrement,
            IncrementPtr,
            DecrementPtr,
            Output,
            Input,
            LoopBegin,
            LoopEnd,
            SetValue,
        };
    public:
        Instruction(Type type, std::uint64_t value = 1)
            : mType(type), mValue(value)
        {}

        void Execute(Memory& memory) const
        {
            switch (mType) {
                break; case Type::Nop: {
                }
                break; case Type::Increment: {
                    memory.Current() += mValue;
                }
                break; case Type::Decrement: {
                    memory.Current() -= mValue;
                }
                break; case Type::IncrementPtr: {
                    memory.Index += mValue;
                }
                break; case Type::DecrementPtr: {
                    memory.Index -= mValue;
                }
                break; case Type::Output: {
                    std::putchar(memory.Current());
                }
                break; case Type::Input: {
                    memory.Current() = std::getchar();
                }
                break; case Type::LoopBegin: {
                    if (!memory.Current()) {
                        memory.ProgramCounter += mValue;
                    }
                }
                break; case Type::LoopEnd: {
                    if (memory.Current()) {
                        memory.ProgramCounter -= mValue;
                    }
                }
                break; case Type::SetValue: {
                    memory.Current() = mValue;
                }
            }
        }

        Type GetType() const
        {
            return mType;
        }

        std::uint64_t& Value()
        {
            return mValue;
        }
    private:
        Type          mType;
        std::uint64_t mValue;
    };
}

#endif //BFCK_INSTRUCTIONS_HPP
