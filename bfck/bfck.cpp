#include "Interpreter.hpp"
#include "Utils.hpp"
#include "Compiler.hpp"
#include "Machine.hpp"

#include <print>

int main (int argc, const char *argv[])
{
    if (argc != 2) {
        std::println("Usage: bfck <file.bf>");
        return -1;
    }

    //BFCK::Interpreter interpreter;
    //interpreter.RunFile(argv[1]);

    auto source = BFCK::Utils::ReadFile(argv[1]);

    if (!source) {
        using BFCK::Utils::FileReadError;
        switch (source.error()) {
        break; case FileReadError::FileDoesNotExist:
            std::println("File does not exist.");
        break; case FileReadError::PathIsNotAFile:
            std::println("Path is not a file.");
        break; case FileReadError::CouldNotOpenFile:
            std::println("Could not open file.");
        }
        return -1;
    }

    BFCK::Utils::ScopedStopwatch sw(argv[1]);
    BFCK::Compiler compiler;
    BFCK::Machine::Program program = compiler.Compile(source.value());
    BFCK::Machine machine;
    machine.Execute(program);

    return 0;
}
