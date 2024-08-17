#include "utilities.h"
#include "arithmetic.h"
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

vector<string> arith_instr = {"ADD", "ADI", "ADC", "SUB", "INR", "DCR", "INX", "DCX", "DAD", "SUI"};

int hexaCharToDec(char data)
{
    if(data >= '0' && data <= '9')
    return data - '0';
    else
    return data - 'A' + 10;
}

char decToHexaChar(int data)
{
    if(data >= 10)
    return data - 10 + 'A';
    else
    return data + '0';
}

void add(string operand, string registers[], vector<bool> &flag, string &pc, bool immediate)
{
    int reg;
    string resultant(2, '0');

    int data1, data2, res;
    data1 = hexaCharToDec(registers[0][1]);

    if(immediate == true)   
    data2 = hexaCharToDec(operand[1]);
    else
    {
        if(operand[0] == 'H')
        reg = 5;
        else if(operand[0] == 'L')
        reg = 6;
        else
        reg = operand[0] - 'A';

        data2 = hexaCharToDec(registers[reg][1]);
    }
    
    short cy = 0;
    res = data1 + data2;
    if(res > 15)
    {
        cy = 1;
        flag[2] = 1; // auxillary carry flag set
        res = res % 16;
    }
    else
    flag[2] = 0;

    resultant[1] = decToHexaChar(res);

    data1 = hexaCharToDec(registers[0][0]);
    if(immediate == true)
    data2 = hexaCharToDec(operand[0]);
    else
    data2 = hexaCharToDec(registers[reg][0]);

    res = data1 + data2 + cy;

    if(res > 15)
    {
        flag[4] = 1; // carry set flag set
        res = res % 16;
    }
    else
    flag[4] = 0;

    resultant[0] = decToHexaChar(res);
    registers[0] = resultant; // stroring result in accumulator

    /*update the flag register*/
    if(registers[0][0] > '7')
    flag[0] = 1; // sign flag 
    else
    flag[0] = 0;    

    if(registers[0] == "00")
    flag[1] = 1; // zero flag
    else
    flag[1] = 0;

    if(checkParity(registers[0]))
    flag[3] = 1; // parity flag
    else
    flag[3] = 0;

    pc = incrementProgramCounter(pc);
    if(immediate)
    pc = incrementProgramCounter(pc);
}

void addWithCarry(string operand, string registers[], vector<bool> &flag, string &pc, bool immediate)
{
    int reg;
    string resultant(2, '0');

    int data1, data2, res;
    data1 = hexaCharToDec(registers[0][1]);

    if(immediate == true)   
    data2 = hexaCharToDec(operand[1]);
    else
    {
        if(operand[0] == 'H')
        reg = 5;
        else if(operand[0] == 'L')
        reg = 6;
        else
        reg = operand[0] - 'A';

        data2 = hexaCharToDec(registers[reg][1]);
    }
    
    short cy = 0;
    res = data1 + data2 + flag[4];
    if(res > 15)
    {
        cy = 1;
        flag[2] = 1; // auxillary carry flag set
        res = res % 16;
    }
    else
    flag[2] = 0;

    resultant[1] = decToHexaChar(res);

    data1 = hexaCharToDec(registers[0][0]);
    if(immediate == true)
    data2 = hexaCharToDec(operand[0]);
    else
    data2 = hexaCharToDec(registers[reg][0]);

    res = data1 + data2 + cy;

    if(res > 15)
    {
        flag[4] = 1; // carry set flag set
        res = res % 16;
    }
    else
    flag[4] = 0;

    resultant[0] = decToHexaChar(res);
    registers[0] = resultant; // stroring result in accumulator

    /*update the flag register*/
    if(registers[0][0] > '7')
    flag[0] = 1; // sign flag 
    else
    flag[0] = 0;    

    if(registers[0] == "00")
    flag[1] = 1; // zero flag
    else
    flag[1] = 0;

    if(checkParity(registers[0]))
    flag[3] = 1; // parity flag
    else
    flag[3] = 0;

    pc = incrementProgramCounter(pc);
    if(immediate)
    pc = incrementProgramCounter(pc);
}

void sub(string operand, string registers[], vector<bool> &flag, string &pc, bool immediate) // yet to define
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

    registers[0] = result; //storing result in accumulator
    
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

    if(registers[0] == "00")
    flag[1] = 1; // zero flag set
    else
    flag[1] = 0;

    if(checkParity(registers[0]))
    flag[3] = 1; // parity flag set
    else
    flag[3] = 0;

    pc = incrementProgramCounter(pc);
    if(immediate)
    pc = incrementProgramCounter(pc);
}

void increment(string operand, string registers[], vector<bool> &flag, string &pc)
{
    int reg;
    if(operand[0] == 'H')
    reg = 5;
    else if(operand[0] == 'L')
    reg = 6;
    else
    reg = operand[0] - 'A'; 
    
    string data = registers[reg];

    if(data[1] != 'F')
    {
        data[1] = plusOne(data[1]);
        flag[2] = 0;
    }
    else
    {
        flag[2] = 1;
        if(data[0] != 'F')
        {
            data[0] = plusOne(data[0]);
            data[1] = '0';
        }
        else
        {
            data = "00";
        }
    }

    //tired honestly, and i feel sorrow to the core of my being. it's awful, i wanna get out, it's too much even for me...please
    // i still do on 29th of april, man april really just passed, the most beautiful month there is for the saddest boy ever
    registers[reg] = data;
    if(registers[reg][0] > '7')
    flag[0] = 1; // sign flag 
    else
    flag[0] = 0;    

    if(registers[reg] == "00")
    flag[1] = 1; // zero flag
    else
    flag[1] = 0;

    if(checkParity(registers[reg]))
    flag[3] = 1; // parity flag
    else
    flag[3] = 0;

    pc = incrementProgramCounter(pc);
}

void decrement(string operand, string registers[], vector<bool> &flag, string &pc)
{
    int reg;
    if(operand[0] == 'H')
    reg = 5;
    else if(operand[0] == 'L')
    reg = 6;
    else
    reg = operand[0] - 'A'; 

    string data = registers[reg];

    if(data[1] != '0')
    {
        data[1] = minusOne(data[1]);
        flag[2] = 1;
    }
    else
    {
        flag[2] = 0;
        if(data[0] != '0')
        {
            data[0] = minusOne(data[0]);
            data[1] = 'F';
        }
        else
        {
            data = "FF";
        }
    }

    registers[reg] = data;
    if(registers[reg][0] > '7')
    flag[0] = 1; // sign flag 
    else
    flag[0] = 0;    

    if(registers[reg] == "00")
    flag[1] = 1; // zero flag
    else
    flag[1] = 0;

    if(checkParity(registers[reg]))
    flag[3] = 1; // parity flag
    else
    flag[3] = 0;

    pc = incrementProgramCounter(pc);
}


string increment16Data(string pc) // utility function
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

void incrementx(string operand, string registers[], vector<bool> &flag, string &pc)
{
    int reg;
    if(operand[0] == 'H')
    reg = 5;
    else
    reg = operand[0] - 'A';

    string data = registers[reg] + registers[reg+1];
    data = increment16Data(data);
    
    registers[reg] = data.substr(0, 2);
    registers[reg + 1] = data.substr(2);

    pc = incrementProgramCounter(pc);
}

string decrement16Data(string pc) // utility function
{
    if(pc[3] != '0')
    pc[3] = minusOne(pc[3]);
    else
    {
        if(pc[2] != '0')
        {
            pc[2] = minusOne(pc[2]);
            pc[3] = 'F';
        }
        else
        {
            if(pc[1] != '0')
            {
                pc[1] = minusOne(pc[1]);
                pc[2] = pc[3] = 'F';
            }
            else
            {
                if(pc[0] != '0')
                {
                    pc[0] = minusOne(pc[0]);
                    pc[1] = pc[2] = pc[3] = 'F';
                }
                else
                pc = "FFFF";
            }
        }
    }
    return pc;
}

void decrementx(string operand, string registers[], vector<bool> &flag, string &pc)
{
    int reg;
    if(operand[0] == 'H')
    reg = 5;
    else
    reg = operand[0] - 'A';

    string data = registers[reg] + registers[reg+1];
    data = decrement16Data(data);

    registers[reg] = data.substr(0, 2);
    registers[reg + 1] = data.substr(2);

    pc = incrementProgramCounter(pc);
}

void doubleAdd(string operand, string registers[], vector<bool> &flag, string &pc)
{
    string data1 = registers[5] + registers[6]; // destination is HL register pair
    int reg;
    if(operand[0] == 'H')
    reg = 5;
    else
    reg = operand[0] - 'A';

    string data2 = registers[reg] + registers[reg+1];
    
    string result(4, '0');
    short cy = 0;
    for(int i=3; i>=0; i--)
    {
        int d1, d2, res;
        d1 = hexaCharToDec(data1[i]);
        d2 = hexaCharToDec(data2[i]);

        res = d1 + d2 + cy;
        if(res > 15)
        {
            cy = 1;
            res = res % 16;
        }
        result[i] = decToHexaChar(res);
    }
    if(cy == 1) // set carry flag if carry generated
    flag[4] = 1;
    else
    flag[4] = 0;

    registers[reg] = result.substr(0, 2);
    registers[reg + 1] = result.substr(2);
    pc = incrementProgramCounter(pc);

}

void executeArithInstr(string opcode, string operand, string registers[], vector<bool> &flag, map<string, string> &memory, string &pc)
{
    auto idx = find(arith_instr.begin(), arith_instr.end(), opcode);
    if(*idx == "ADD")
    add(operand, registers, flag, pc, false); // add with register data
    else if(*idx == "ADI")
    {
        operand = memory[incrementProgramCounter(pc)];
        add(operand, registers, flag, pc, true); // add with immediate data
    }
    else if(*idx == "SUB")
    sub(operand, registers, flag, pc, false);
    else if(*idx == "SUI")
    {
        operand = memory[incrementProgramCounter(pc)];
        sub(operand, registers, flag, pc, true);
    }
    else if(*idx == "INR")
    increment(operand, registers, flag, pc); // increment 8 bit data
    else if(*idx == "DCR")
    decrement(operand, registers, flag, pc); // decrement 8 bit data
    else if(*idx == "INX")
    incrementx(operand, registers, flag, pc); // increment 16 bit data
    else if(*idx == "DCX")
    decrementx(operand, registers, flag, pc); // decrement 16 bit data
}