# MIPS Instruction Simulator

An interactive C++ program that accepts 32-bit hexadecimal MIPS instructions, displays their decoded fields and binary representation, and executes them against a simplified register and memory model.

This is an educational simulator for a focused subset of MIPS instructions, not a complete MIPS emulator.

## Features

- Validates 8-digit hexadecimal instructions.
- Decodes R-type and I-type fields.
- Shows binary, decoded fields, and assembly-style output.
- Simulates 16 registers and 16 word-addressed memory locations.
- Executes `add`, `sub`, `and`, `or`, `xor`, `addi`, `lw`, and `sw`.
- Checks register and memory bounds.

## Files

- `mips_simulator.cpp` — simulator implementation.
- `BrentFoxworth_MIPS_Code.pdf` — original supplied project artifact.
- `.gitignore` — local build and workspace exclusions.

## Build

Use a C++17-compatible compiler:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic mips_simulator.cpp -o mips_simulator
```

## Run

```bash
./mips_simulator
```

Enter an 8-digit hexadecimal instruction at each prompt; enter `q` or `Q` to exit.

Example input:

```text
20010005  # addi $1, $0, 5
20020003  # addi $2, $0, 3
00221820  # add  $3, $1, $2
q
```

## Notes

Registers and memory initialize to their indices. Memory operands are interpreted as word addresses, converting byte offsets to an index by dividing by four.

The source was recovered from the original project PDF. The repository version adds the standard `<cstdint>` header required for `int16_t` and corrects the store instruction to use the validated word index.

## License

No license was supplied with the original artifact. All rights are reserved unless the owner adds a license file.
