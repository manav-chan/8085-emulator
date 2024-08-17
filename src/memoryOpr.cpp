#include "utilities.h"
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

char plusOne(char c) // adds 1 in hexa
{
    if(c >= '0' && c <= '8' || c >= 'A' && c <= 'E')
    c = (char)(c + 1);
    else if(c == '9')
    c = 'A';

    return c;
}

char minusOne(char c) // subs 1 in hexa
{
    if(c >= '1' && c <= '9' || c >= 'B' && c <= 'E')
    c = (char)(c - 1);
    else if(c == 'A')
    c = '9';

    return c;
}


string incrementProgramCounter(string pc)
{
    if(pc[3] != 'F')
    pc[3] = plusOne(pc[3]);
    else
    {
        if(pc[2] != 'F')
        {
            pc[2] = plusOne(pc[2]);
            pc[3] = '0';
        }
        else
        {
            if(pc[1] != 'F')
            {
                pc[1] = plusOne(pc[1]);
                pc[2] = pc[3] = '0';
            }
            else
            {
                if(pc[0] != 'F')
                {
                    pc[0] = plusOne(pc[0]);
                    pc[1] = pc[2] = pc[3] = '0';
                }
                else
                pc = "0000";
            }
        }
    }
    return pc;
}

void addToMemorySet(map<string, string> &memory, string opcode, string data1, string data2, string data3, string &pc)
{
    memory[pc] = opcode;

    pc = incrementProgramCounter(pc);
    memory[pc] = data1;
    pc = incrementProgramCounter(pc);
    memory[pc] = data2;
    pc = incrementProgramCounter(pc);
    memory[pc] = data3;
    pc = incrementProgramCounter(pc);
}

void addToMemory(map<string, string> &memory, string opcode, string data1, string data2, string &pc) // adds the instruction to memory and increments the program counter
{
    memory[pc] = opcode;

    pc = incrementProgramCounter(pc);
    if(data1 != "")
    {
        memory[pc] = data1;
        pc = incrementProgramCounter(pc);
    }
    if(data2 != "")
    {
        memory[pc] = data2;
        pc = incrementProgramCounter(pc);
    }  
}