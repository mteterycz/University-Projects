# Program **crc**

## Description
The **crc** program, written in assembly for the Linux system, calculates the checksum of data contained in a sparse file using a cyclic redundancy check (**CRC**). The program analyzes the file structure according to the given format, reading data in segments and skipping gaps (offsets). The CRC polynomial, provided as an argument, is given in binary form.

## Functionality
### 1. **Reading the Sparse File**
   - The file consists of a sequence of fragments where:
     - The first 2 bytes specify the length of the data in the fragment.
     - The following bytes contain the actual data.
     - The last 4 bytes indicate the offset to the next fragment (a signed integer in little-endian format).
   - The program iteratively reads fragments, processes offsets, and gathers data for calculations.

### 2. **Computing the CRC Checksum**
   - The program reads the CRC polynomial provided as the second argument.
   - It converts the polynomial to binary form and uses it for calculations on the file data.
   - The CRC algorithm processes the file byte by byte, considering the most significant bit first.

### 3. **Error Handling and Optimization**
   - The program verifies the validity of input parameters.
   - It handles errors from system functions (e.g., failed file opening).
   - It buffers reads for improved performance – the optimal buffer size is determined experimentally and documented in the code comments.

### 4. **Program Termination**
   - Upon completion, the program outputs the computed checksum as a sequence of zeros and ones, ending with a newline character (`\n`).
   - Before terminating, the program explicitly closes the file.
   - Exit codes:
     - `0` – if the operation was successful.
     - `1` – in case of an error (e.g., invalid arguments, system function failure).

## Used System Functions
The program exclusively uses Linux system functions:
- `sys_open`
- `sys_read`
- `sys_write`
- `sys_lseek`
- `sys_close`
- `sys_exit`

# Compilation

`nasm -f elf64 -w+all -w+error -o crc.o crc.asm`

`ld --fatal-warnings -o crc crc.o`

# Usage 

`./crc file crc_poly`

By leveraging assembly language, the program is optimized for performance and resource efficiency.

