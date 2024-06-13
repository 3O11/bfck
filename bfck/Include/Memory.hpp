#ifndef BFCK_STATE_HPP
#define BFCK_STATE_HPP

#include "Base.hpp"

#include <vector>

namespace BFCK
{
    class Memory
    {
    public:
        Memory(std::size_t memSize = 30000)
            : ProgramCounter{}, Index{}, mBuffer(memSize, 0)
        {}

        [[nodiscard]] constexpr
        std::uint8_t& Current()
        {
            return mBuffer[Index];
        }

        constexpr
        void Reset()
        {
            std::fill(mBuffer.begin(), mBuffer.end(), 0);
        }

        std::uint64_t ProgramCounter;
        std::uint64_t Index;
    private:
        std::vector<std::uint8_t> mBuffer;
    };
}

#endif //BFCK_STATE_HPP
