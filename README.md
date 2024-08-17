# 8085-emulator
8085 Emulator built using C++. It is a terminal application which accepts Assembly Level Language in 2 modes - either as an assembly language file or as line by line input.

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
