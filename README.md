# 8085-emulator
8085 Emulator built using C++. It is a terminal application which accepts Assembly Level Language in 2 modes - either as an assembly language file or as line by line input.

## Introduction
Implemented the following instructions:
1. **Load and Store** - MOV, MVI, LXI, LDA, STA, LHLD, SHLD, STAX, XCHG
2. **Arithmetic** - ADD, ADI, SUB, INR, DCR, INX, DCX, DAD, SUI
3. **Branching** - JMP, JC, JNC, JZ, JNZ
- Also implemented an additional SET command to set data into memory locations. Example usage:
  - `SET 2050 1A`

## Setting up
- Create `build` directory.
```terminal
mkdir build
```
- Build the project at root level.
```terminal
make
```
- Test using the following command.
```terminal
./build/emulator tests/test1.txt 
```
- Test line by line input by writing the code in test1.txt without the SET commands and input the suitable memory locations.
```terminal
./build/emulator
```
