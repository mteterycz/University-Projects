# Dividing

Implement in assembler a function called from C with the following declaration:
`int64_t mdiv(int64_t *x, size_t n, int64_t y);`

The function performs integer division with remainder. The function treats the divisor, divisor, quotient and remainder as numbers written in complement coding to two. The first and second parameters of the function specify the divisor: x is a pointer to a non-empty array of n 64-bit numbers. The divisor has 64 * n bits and is stored in memory in thin-endian (little-endian) order. The third parameter y is the divisor. The result of the function is the remainder of dividing x by y. The function places the quotient in the x array.

# Compilation

`nasm -f elf64 -w+all -w+error -o mdiv.o mdiv.asm`

# Testing

`gcc -c -Wall -Wextra -std=c17 -O2 -o mdiv_example.o mdiv_example.c`

`gcc -z noexecstack -o mdiv_example mdiv_example.o mdiv.o`

`./mdiv_example`