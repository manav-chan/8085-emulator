#include "utilities.h"
#include "branching.h"
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

vector<string> branching_instr = {"JMP", "JC", "JNC", "JZ", "JNZ"};

void jump(string operand, vector<bool> &flag, string &pc) // operand in format "2050"
{
    pc = operand;
}

void jumpCarry(string operand, vector<bool> &flag, string &pc)
{
    if(flag[4] == 1)
    {
        pc = operand;
    }
    else
    {
        pc = incrementProgramCounter(pc);
    }
}

void jumpNotCarry(string operand, vector<bool> &flag, string &pc)
{
    if(flag[4] == 0)
    {
        pc = operand;
    }
    else
    {
        pc = incrementProgramCounter(pc);
    }
}

void jumpZero(string operand, vector<bool> &flag, string &pc)
{
    if(flag[1] == 1)
    {
        pc = operand;
    }
    else
    {
        pc = incrementProgramCounter(pc);
    }
}

void jumpNotZero(string operand, vector<bool> &flag, string &pc)
{
    if(flag[1] == 0)
    {
        pc = operand;
    }
    else
    {
        pc = incrementProgramCounter(pc);
    }
}

void executeBranchingInstr(string opcode, string operand, vector<bool> &flag, map<string, string> &memory, string &pc)
{
    auto idx = find(branching_instr.begin(), branching_instr.end(), opcode);
    if(*idx == "JMP")
    {
        operand = memory[incrementProgramCounter(pc)];
        jump(operand, flag, pc);
    }
    else if(*idx == "JC")
    {
        operand = memory[incrementProgramCounter(pc)];
        jumpCarry(operand, flag, pc);
    }
    else if(*idx == "JNC")
    {
        operand = memory[incrementProgramCounter(pc)];
        jumpNotCarry(operand, flag, pc);
    }
    else if(*idx == "JZ")
    {
        operand = memory[incrementProgramCounter(pc)];
        jumpZero(operand, flag, pc);
    }
    else if(*idx == "JNZ")
    {
        operand = memory[incrementProgramCounter(pc)];
        jumpNotZero(operand, flag, pc);
    }
}