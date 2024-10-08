# 8085-emulator
8085 Emulator built using C++. It is a terminal application which accepts Assembly Level Language in 2 modes - either as an assembly language file or as line by line input. In the absence of an 8085 microprocessor, this emulator can be used for learning purposes.  

## Introduction

- Implemented the following instructions:
  1. **Load and Store** - MOV, MVI, LXI, LDA, STA, LHLD, SHLD, STAX, XCHG
  2. **Arithmetic** - ADD, ADI, SUB, INR, DCR, INX, DCX, DAD, SUI
  3. **Branching** - JMP, JC, JNC, JZ, JNZ
  - Also implemented an additional SET command to set data into memory locations. Example usage:
    - `SET 2050 1A`

- Works in 2 modes:
  1. **File mode** - Append filename in the run command.
     ```terminal
     ./build/emulator <file_name>
     ```
  2. **Line by line mode** - Run command.
     ```terminal
     ./build/emulator
     ```
     - Enter `STOP` to stop emulator from taking input.

- Displays the content of A, B, C, D, E, H, L, flag (5 flags) registers and the used memory locations.
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

## Examples
- Program to swap numbers at 2 different memory locations.
- <img  src="https://github.com/user-attachments/assets/4ad532bd-754a-4757-964c-45370be4e044">
- Program to find the greatest of 2 numbers (Result in 3050).
- <img  src="https://github.com/user-attachments/assets/e912e641-de7a-4d84-b3c5-1f7831460ecf">

