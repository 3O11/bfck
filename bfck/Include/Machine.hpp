#ifndef BFCK_MACHINE_HPP
#define BFCK_MACHINE_HPP

#include "Base.hpp"

#include <vector>

namespace BFCK
{
    class Machine
    {
    public:
        struct Instruction
        {
            enum class Type : std::uint8_t
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
                Copy,
                Multiply,
                IncrementTo0,
                DecrementTo0,
            };

            Type          Identifier;
            std::uint8_t  Reserved;
            std::uint16_t Value;
            std::uint32_t Offset;
        };

        using Program = std::vector<Instruction>;

        struct State
        {
            using Tape = std::vector<std::uint8_t>;

            std::size_t ProgramCounter;
            std::size_t Pointer;
            Tape        Memory;
        };
    public:
        [[nodiscard]] constexpr
        Machine(std::size_t memSize = 30000)
            : mState{ 0, 0, State::Tape(memSize, 0) }
        {}

        void Execute(Instruction instr);
        void Execute(Program program);

        constexpr
        void Reset()
        {
            std::fill(mState.Memory.begin(), mState.Memory.end(), 0);
        }
    private:
        State mState;
    };
}

#endif //BFCK_MACHINE_HPP
