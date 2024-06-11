#include "Interpreter.hpp"
#include "Utils.hpp"

#include <cstdio>

namespace BFCK
{
    Interpreter::Interpreter()
        : mMemory(30000, 0)
    {}

    void Interpreter::Run(std::string_view code)
    {
        std::size_t pc = 0;
        std::size_t mp = 0;

        while (pc < code.size()) {
            switch (code[pc]) {
                case '>':
                    ++mp;
                    break;
                case '<':
                    --mp;
                    break;
                case '+':
                    ++mMemory[mp];
                    break;
                case '-':
                    --mMemory[mp];
                    break;
                case '.':
                    std::putchar(mMemory[mp]);
                    break;
                case ',':
                    mMemory[mp] = std::getchar();
                    break;
                case '[':
                    if (mMemory[mp] == 0) {
                        std::size_t loop = 1;
                        while (loop != 0) {
                            ++pc;
                            if (code[pc] == '[') {
                                ++loop;
                            } else if (code[pc] == ']') {
                                --loop;
                            }
                        }
                    }
                    break;
                case ']':
                    if (mMemory[mp] != 0) {
                        std::size_t loop = 1;
                        while (loop != 0) {
                            --pc;
                            if (code[pc] == '[') {
                                --loop;
                            } else if (code[pc] == ']') {
                                ++loop;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
            ++pc;
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
        mMemory.assign(mMemory.size(), 0);
    }
}
