#ifndef BFCK_INTERPRETER_HPP
#define BFCK_INTERPRETER_HPP

#include "Base.hpp"
#include "Memory.hpp"

namespace BFCK
{
    class Interpreter
    {
    public:
        Interpreter()
            : mMemory{}
        {}

        void Run(std::string_view code);
        void RunFile(const fs::path& filepath);

        void Reset();
    private:
        Memory mMemory;
    };
}

#endif //BFCK_INTERPRETER_HPP
