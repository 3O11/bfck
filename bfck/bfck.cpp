#include "Interpreter.hpp"

int main (int argc, const char *argv[])
{
    if (argc < 2) {
        std::println("Usage: bfck <file.bf>");
        return -1;
    }

    //std::println("{}", BFCK::Utils::ReadFile(argv[1]).value_or("Error reading file."));

    BFCK::Interpreter interpreter;
    interpreter.RunFile(argv[1]);

    std::println("Hello World!");
    return 0;
}
