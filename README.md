# bfck

A quick and dirty brainfuck interpreter written as a simple exercise
in a few hours. Compiles into an executable that takes the path to a file with
code and executes it. It's not the fastest thing in the world, and there are
still many simple optimizations left on the table.

Implemented optimizations:
- Contraction (long chains of identical operations)
- Clear loops (setting the current cell to 0)

TODO:
- Error checking (currently checks almost nothing)
    - Non-matching `[]`
- Get rid of unsafe indexing
- Use a tree as IR
- Optimizations ([reference](http://calmerthanyouare.org/2015/01/07/optimizing-brainfuck.html))
- Configuration
    - Memory size
    - Memory wrapping
    - [Extensions](https://esolangs.org/wiki/Brainfuck%2B)?

Notes (for later):
- Type erasure sucks for this
- std::variant and its drawbacks
- Arbitrary limits (instruction size optimization)
- The cost of optimization
