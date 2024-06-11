#ifndef BFCK_INTERPRETER_HPP
#define BFCK_INTERPRETER_HPP

#include "Base.hpp"

#include <vector>

namespace BFCK
{
    class Interpreter
    {
    public:
        Interpreter();

        void Run(std::string_view code);
        void RunFile(const fs::path& filepath);

        void Reset();
    private:
        std::vector<std::uint8_t> mMemory;
    };
}

#endif //BFCK_INTERPRETER_HPP
