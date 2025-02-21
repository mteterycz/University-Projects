# NAND Gate Library  

## Description  

The project implements a dynamically loaded library in C for handling combinational logic circuits based on NAND gates. Each NAND gate can have between 0 and n inputs and one output, and its behavior follows the rules of this logical operation. The library allows creating and deleting gates, connecting them into circuits, and computing output signal values and critical path lengths.  

## Features  

- Creating and deleting NAND gates  
- Connecting inputs to other gates or Boolean signals  
- Computing output signal values and critical path lengths  
- Checking the number of connections at the gate output  
- Iterating over gates connected to a given output  

## Interface  

The library provides functions defined in the header file `nand.h`, including:  
- `nand_new(unsigned n)` – creates a NAND gate with n inputs  
- `nand_delete(nand_t *g)` – deletes a NAND gate  
- `nand_connect_nand(nand_t *g_out, nand_t *g_in, unsigned k)` – connects gates  
- `nand_connect_signal(bool const *s, nand_t *g, unsigned k)` – connects logical values  
- `nand_evaluate(nand_t **g, bool *s, size_t m)` – computes output signal values and the critical path length  

The library handles error cases such as memory shortages, incorrect connections, or cycles in the circuit.  

## Testing

- `./testy test_name`
- Test names: `simple`, `example`, `memory`
