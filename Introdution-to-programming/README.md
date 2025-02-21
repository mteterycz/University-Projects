# Introduction to Programming

This folder contains introductory programming exercises and supporting materials. It covers fundamental concepts, basic algorithms, and practical examples to help build confidence with essential programming skills. All exercises are conducted in the C language.

## Reversi Game

The program reversi.c implements a two-player Reversi game. It:
- Reads player commands continuously, using text-based prompts that indicate:
    - Which player’s turn it is (Black or White).
    - The list of legal moves available for that player (sorted lexicographically).
- Processes three types of commands:
    - A move command, which specifies a legal move.
    - A resignation command ("-"), allowing a player to skip their move.
    - A termination command ("="), which ends the game.
- Updates the board by placing pieces and flipping opponent pieces according to the rules.
- Continues executing until the termination command is received, regardless of available moves.

## L-System Interpreter

The program l_system.c implements a deterministic context-free L-system (D0L system). It reads the derivation length, a formal description of the L-system (axiom and rewriting rules), and the associated interpretation rules. The program then computes the derived word from the axiom by applying the rewriting rules repeatedly for the specified number of derivations. Finally, it constructs a textual output by concatenating a prolog, invoking the interpretation for each symbol in the derived word (omitting any without a rule), and appending an epilog.

## Petlik Interpreter and Compiler

The program petlik.c implements a solution for the Pętlik programming language. It reads commands line by line where each command is either:
- A print command, starting with '=' followed by a variable name. The program outputs the current value of that variable.
- A Pętlik program source code conforming to the defined grammar.

The language consists of two types of instructions:
- An increment instruction (a single variable), equivalent in C to ++variable.
- A loop instruction enclosed in parentheses, which performs as a C-style while loop that decrements the controlling variable on each iteration.

The compiler optimizes loops when possible. If a loop contains only non-nested increment instructions that do not interfere with the controlling variable, it generates an optimized sequence of ADD and CLR instructions. Otherwise, it produces standard loop code using DJZ and JMP instructions.

The generated machine code runs on a virtual machine with operations:
- INC for incrementing,
- ADD for adding values,
- CLR for clearing,
- DJZ for decrement-or-jump, 
- JMP for unconditional jump, and
- HLT to halt execution.
