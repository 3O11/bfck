#ifndef BFCK_COMPILER_HPP
#define BFCK_COMPILER_HPP

#include "Base.hpp"
#include "Machine.hpp"

#include <variant>

namespace BFCK
{
    namespace IR
    {
        struct Nop {};
        struct Increment { std::uint16_t Value; };
        struct Decrement { std::uint16_t Value; };
        struct IncrementPtr { std::uint32_t Value; };
        struct DecrementPtr { std::uint32_t Value; };
        struct Output {};
        struct Input {};
        struct Loop;
        struct SetValue { std::uint16_t Value; };
        struct Copy { std::uint32_t Offset; };
        struct Multiply { std::uint32_t Offset; std::uint16_t Value; };
        struct IncrementTo0 {};
        struct DecrementTo0 {};
        struct Scope;

        using Instruction = std::variant<
            Nop,
            Increment,
            Decrement,
            IncrementPtr,
            DecrementPtr,
            Output,
            Input,
            Loop,
            SetValue,
            Copy,
            Multiply,
            IncrementTo0,
            DecrementTo0,
            Scope
        >;

        struct Loop { std::vector<Instruction> Instructions; };
        struct Scope { std::vector<Instruction> Instructions; };
    }

    class Compiler
    {
    public:
    public:
        IR::Scope Parse(std::string_view source);
        void Optimize(IR::Scope& scope);
        Machine::Program EmitInstruction(const IR::Instruction& instruction);
        Machine::Program Emit(IR::Scope& scope);


        Machine::Program Compile(std::string_view source);
    private:

    };
}

#endif //BFCK_COMPILER_HPP
