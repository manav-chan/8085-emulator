#include "dataTransfer.h"
#include "utilities.h"
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

vector<string> data_transfer_instr = {"SET", "MOV", "MVI", "LXI", "LDA", "STA", "LHLD", "SHLD", "STAX", "LDAX", "XCHG"};

bool setToMemory(string adr, string data, map<string, string> &memory, string &pc)
{
    if(memory.find(adr) != memory.end()) // memory is occupied by some other data.
    return false;

    memory[adr] = data;
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    return true;
}

void move(string operand, string registers[], map<string, string> &memory, string &pc, bool immediate) // operand in format "A,B" ; for immediate in format "A 00"
{
    
    if(operand[0] == 'M') // destination M
    {
        string adr = registers[5] + registers[6];
        if(immediate)
        {
            string data = operand.substr(2);
            memory[adr] = data;
        }
        else
        {
            int src;
            if(operand[2] == 'H')
            src = 5;
            else if(operand[2] == 'L')
            src = 6;
            else
            src = operand[2] - 'A';

            memory[adr] = registers[src];
        }
    }
    if(operand[2] == 'M') // source M
    {
        string adr = registers[5] + registers[6];
        int dst;
        if(operand[0] == 'H')
        dst = 5;
        else if(operand[0] == 'L')
        dst = 6;
        else
        dst = operand[0] - 'A';

        registers[dst] = memory[adr];
    }
    else
    {
        int dst;
        if(operand[0] == 'H')
        dst = 5;
        else if(operand[0] == 'L')
        dst = 6;
        else
        dst = operand[0] - 'A';

        if(immediate)
        {
            string data = operand.substr(2);
            registers[dst] = data;
        }
        else
        {
            int src;
            if(operand[2] == 'H')
            src = 5;
            else if(operand[2] == 'L')
            src = 6;
            else
            src = operand[2] - 'A';

            registers[dst] = registers[src];
        }
    }
    pc = incrementProgramCounter(pc);
    if(immediate)
    pc = incrementProgramCounter(pc);
}

void load(string operand, string registers[], map<string, string> &memory, string &pc) // address is in operand
{
    registers[0] = memory[operand]; // we can overload map function if it does not find key present (org. behaviour is insert empty string to newly inserted key element in map)
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
}

void loadXI(string operand, string registers[], string &pc) // operand in format "B 2050"
{
    int dst;
    if(operand[0] == 'H')
    dst = 5;
    else
    dst = operand[0] - 'A';

    string data = operand.substr(2);

    registers[dst] = data.substr(0, 2);
    registers[dst + 1] = data.substr(2);
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
}

void set(string operand, string registers[], map<string, string> &memory, string &pc) // operand in format "2050"
{
    memory[operand] = registers[0];
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
}

void loadHL(string operand, string registers[], map<string, string> &memory, string &pc) // operand in format "2050"
{
    registers[5] = memory[operand];
    operand = incrementProgramCounter(operand);
    registers[6] = memory[operand];

    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
}  

bool setHL(string operand, string registers[], map<string, string> &memory, string &pc) // operand in format "2050"
{
    memory[operand] = registers[6];
    operand = incrementProgramCounter(operand);
    memory[operand] = registers[5];

    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    pc = incrementProgramCounter(pc);
    return true;
}

bool setX(string operand, string registers[], map<string, string> &memory, string &pc) // operand in format "H"
{
    int reg;
    if(operand[0] == 'H')
    reg = 5;
    else
    reg = operand[0] - 'A';

    string addr = registers[reg] + registers[reg + 1];
    memory[addr] = registers[0];

    pc = incrementProgramCounter(pc);
    return true;
}

void loadX(string operand, string registers[], map<string, string> &memory, string &pc) // operand in format "H"
{
    int reg;
    if(operand[0] == 'H')
    reg = 5;
    else
    reg = operand[0] - 'A';

    string addr = registers[reg] + registers[reg + 1];
    registers[0] = memory[addr];

    pc = incrementProgramCounter(pc);
}

void exchange(string registers[], string &pc) // exchanges content of HL and DE register pair
{
    string temp;
    temp = registers[5];
    registers[5] = registers[3];
    registers[3] = temp;

    temp = registers[6];
    registers[6] = registers[4];
    registers[4] = temp;

    pc = incrementProgramCounter(pc);
}

bool executeDataTransferInstr(string opcode, string operand, string registers[], map<string, string> &memory, string &pc)
{
    auto idx = find(data_transfer_instr.begin(), data_transfer_instr.end(), opcode);
    if(*idx == "SET")
    {
        string temp_pc = incrementProgramCounter(pc);
        string temp2_pc = incrementProgramCounter(temp_pc);

        string adr = memory[temp2_pc] + memory[temp_pc];
        string data = memory[incrementProgramCounter(temp2_pc)];

        if(!setToMemory(adr, data, memory, pc))
        return false;
    }
    else if(*idx == "MOV")
    {
        move(operand, registers, memory, pc, false);
    }
    else if(*idx == "MVI")
    {
        operand += " " + memory[incrementProgramCounter(pc)];
        move(operand, registers, memory, pc, true);
    }
    else if(*idx == "LXI")
    {
        operand += " " + memory[incrementProgramCounter(pc)];
        loadXI(operand, registers, pc);
    }
    else if(*idx == "LDA")
    {
        string temp_pc = incrementProgramCounter(pc);
        string temp2_pc = incrementProgramCounter(temp_pc);
        operand = memory[temp2_pc] + memory[temp_pc];
        
        load(operand, registers, memory, pc);
    }
    else if(*idx == "STA")
    {
        string temp_pc = incrementProgramCounter(pc);
        string temp2_pc = incrementProgramCounter(temp_pc);
        operand = memory[temp2_pc] + memory[temp_pc];

        set(operand, registers, memory, pc);
    }
    else if(*idx == "LHLD")
    {
        string temp_pc = incrementProgramCounter(pc);
        string temp2_pc = incrementProgramCounter(temp_pc);
        operand = memory[temp2_pc] + memory[temp_pc];

        loadHL(operand, registers, memory, pc);
    }
    else if(*idx == "SHLD")
    {
        string temp_pc = incrementProgramCounter(pc);
        string temp2_pc = incrementProgramCounter(temp_pc);
        operand = memory[temp2_pc] + memory[temp_pc];
        
        if(!setHL(operand, registers, memory, pc))
        return false;
    }
    else if(*idx == "STAX")
    {
        if(!setX(operand, registers, memory, pc))
        return false;
    }
    else if(*idx == "LDAX")
    {
        loadX(operand, registers, memory, pc);
    }
    else if(*idx == "XCHG")
    {
        exchange(registers, pc);
    }
    return true;
}