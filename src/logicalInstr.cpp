#include "utilities.h"
#include "logical.h"
#include "arithmetic.h"
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

vector<string> logical_instr = {"CMA", "CMP"};

void complement(string registers[], string &pc)
{
    /* To find complement, we subtract the number from FF */
    string data1 = "FF";
    string data2 = registers[0];
    int d1, d2, res = 0;
    string result(2, '0');
    short by = 0; // borrow

    d1 = hexaCharToDec(data1[1]);
    d2 = hexaCharToDec(data2[1]);
    if(d1 < d2)
    {
        res += 16;
        by = 1;
    }
    res += d1 - d2; // add 16 if carry 
    result[1] = decToHexaChar(res);

    res = 0;
    d1 = hexaCharToDec(data1[0]);
    d2 = hexaCharToDec(data2[0]);
    short finalBy = 0;
    if(by == 1 && d1 == 0)
    {
        finalBy = 1;
        res = 15 + d1 - d2;
    }
    else if(d1 < d2)
    {
        finalBy = 1;
        res = 16 + d1 - d2 - by;
    }
    else
    res = d1 - d2 - by;
    result[0] = decToHexaChar(res);
    registers[0] = result;

    pc = incrementProgramCounter(pc);
}

void compare(string operand, string registers[], vector<bool> &flag, string &pc, bool immediate)
{
    string data1 = registers[0];
    string data2;
    int reg;
    if(immediate)
    data2 = operand;
    else
    {
        if(operand[0] == 'H')
        reg = 5;
        else if(operand[0] == 'L')
        reg = 6;
        else
        reg = operand[0] - 'A';

        data2 = registers[reg];
    }
    
    int d1, d2, res = 0;
    string result(2, '0');
    short by = 0; // borrow

    d1 = hexaCharToDec(data1[1]);
    d2 = hexaCharToDec(data2[1]);
    if(d1 < d2)
    {
        flag[2] = 1; // auxillary carry flag set
        res += 16;
        by = 1;
    }
    else
    {
        flag[2] = 0;
    }
    res += d1 - d2; // add 16 if carry 
    result[1] = decToHexaChar(res);

    res = 0;
    d1 = hexaCharToDec(data1[0]);
    d2 = hexaCharToDec(data2[0]);
    short finalBy = 0;
    if(by == 1 && d1 == 0)
    {
        finalBy = 1;
        res = 15 + d1 - d2;
    }
    else if(d1 < d2)
    {
        finalBy = 1;
        res = 16 + d1 - d2 - by;
    }
    else
    res = d1 - d2 - by;
    result[0] = decToHexaChar(res);
    
    if(finalBy == 1) 
    {
        flag[4] = 1; // carry flag set
        flag[0] = 1; // sign flag set (negative)
    }
    else
    {
        flag[4] = 0;
        flag[0] = 0;
    }

    if(result == "00")
    flag[1] = 1; // zero flag set
    else
    flag[1] = 0;

    if(checkParity(result))
    flag[3] = 1; // parity flag set
    else
    flag[3] = 0;

    pc = incrementProgramCounter(pc);
    if(immediate)
    pc = incrementProgramCounter(pc);
}

void executeLogicalInstr(string opcode, string operand, string registers[], vector<bool> &flag, map<string, string> &memory, string &pc)
{
    auto idx = find(logical_instr.begin(), logical_instr.end(), opcode);
    if(*idx == "CMA")
    {
        complement(registers, pc);
    }
    else if(*idx == "CMP")
    {
        compare(operand, registers, flag, pc, false);
    }
    else if(*idx == "CPI")
    {
        compare(operand, registers, flag, pc, true);
    }
}