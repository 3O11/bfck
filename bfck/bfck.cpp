#include "Interpreter.hpp"

#include <print>

int main (int argc, const char *argv[])
{
    if (argc < 2) {
        std::println("Usage: bfck <file.bf>");
        return -1;
    }

    BFCK::Interpreter interpreter;
    interpreter.RunFile(argv[1]);
    return 0;
}
