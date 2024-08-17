#include "utilities.h"
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

vector<string> oneTokenInstr = {"CMA", "HLT", "XCHG"};
vector<string> twoTokenInstr = {"MOV", "LDA", "STA", "LHLD", "SHLD", "STAX", "LDAX", "ADD", "ADI", "ADC", "SUB", "INR", "DCR", "INX", "DCX", "DAD", "SUI", "CMP", "JMP", "JC", "JNC", "JZ", "JNZ"};
// nondata2Token = {"MOV",  "ADD", "SUB", "INR", dcr ,inx, dcx, dad, sui, cmp}
vector<string> twoToken8DataInstr = { "ADI", "SUI", "CPI"};
vector<string> twoToken16DataInstr = {"LDA", "STA", "LHLD", "SHLD", "JMP", "JNC", "JZ","JNZ"};
vector<string> thrTokenInstr = {"MVI", "LXI", "SET"};

vector<string> oneByteInstr = {"MOV", "XCHG", "ADD", "ADC", "LDAX", "STAX", "SUB", "INR", "DCR", "INX", "DCX", "DAD", "CMA", "CMP"};
vector<string> twoByteInstr= {"MVI", "ADI", "SUI", "CPI"};
vector<string> thrByteInstr= {"LXI", "LDA", "STA", "LHLD", "SHLD", "JMP", "JC", "JNC", "JZ", "JNZ"};

bool isValidAddress(string adr) // checks if an address is a valid address
{
    for(int i=0; i<4; i++)
    {
        if( !((adr[i] >= '0' && adr[i] <= '9') || (adr[i] >= 'A' && adr[i] <= 'F')))
        return false;
    }
    return true;
}

bool isValidOpcode(string opcode)
{
    vector<string> instructions = {"MOV", "MVI", "LXI", "LDA", "STA", "LHLD", "SHLD", "STAX", "LDAX", "XCHG", "ADD", "ADI", "ADC", "SUB", "INR", "DCR", "INX", "DCX", "DAD", "SUI", "CMA", "CMP", "CPI", "JMP", "JC", "JNZ", "SET", "HLT"};

    for(auto i: instructions)
    {
        if(i == opcode)
        {
            return true;
        }
    }
    return false;
}

bool isValid8BitData(string data)
{
    if(data.length() != 2)
    return false;
    else
    {
        if((data[0] >= '0' && data[0] <='9' || data[0] >= 'A' && data[0] <= 'F') && (data[1] >= '0' && data[1] <='9' || data[1] >= 'A' && data[1] <= 'F'))
        return true;
        else return false;
    }
}

bool checkParity(string data) // return true for even parity else returns false
{
    int n = 0;
    if(data[1] > '9')
        n += data[1] - 'A' + 10;
    else
        n += data[1] - '0';

    if(data[0] > '9')
        n += (data[0] - 'A' + 10) * 16;
    else
        n += (data[0] - '0') * 16;

    int count = 0;
    while (n != 0)
    {
        if(n & 1)
        count ++;

        n >>= 1;
    }
    if(count % 2 == 0)
    return true;
    else
    return false;
    
}

bool isValidRegister(string reg)
{
    if(reg == "A" || reg == "B" || reg == "C" || reg == "D" || reg == "E" || reg == "H" || reg == "L")
    return true;
    else
    return false;
}

bool isValidRegisterX(string reg)
{
    if(reg == "B" || reg == "D" || reg == "H")
    return true;
    else 
    return false;
}

bool isValidRegisterMov(char reg)
{
    if(reg == 'A' || reg == 'B' || reg == 'C' || reg == 'D' || reg == 'E' || reg == 'H' || reg == 'L' || reg == 'M')
    return true;
    else
    return false;
}

bool isValidInstruction(string instr, map<string, string> &memory, string &pc) // checks if an instrution is a valid instruction
{
    if(instr == "")
    return false;

    stringstream ss(instr);
    string temp;
    vector<string> tokens;
    while(getline(ss, temp, ' ')) // separates strings using ' ' as delimiter
    {
        tokens.push_back(temp);
    }

    string opcode = tokens[0];
    if(!isValidOpcode(opcode)) // checks if opcode is valid
    return false;

    int n_tokens = tokens.size();
    switch (n_tokens)
    {
    case 1: // 1 token instructions
        if(find(oneTokenInstr.begin(), oneTokenInstr.end(), opcode) == oneTokenInstr.end())
        return false;
        else
        {
            addToMemory(memory, opcode, "", "", pc);
            return true;
        }

    case 2: // 2 token instructions
        if(find(twoTokenInstr.begin(), twoTokenInstr.end(), opcode) == twoTokenInstr.end())
        return false;
        else
        {
            if(find(twoToken8DataInstr.begin(), twoToken8DataInstr.end(), opcode) != twoToken8DataInstr.end()) // data1 is 8bit
            {
                string data1 = tokens[1];
                if(!isValid8BitData(data1))
                return false;
                else
                {
                    addToMemory(memory, opcode, data1, "", pc);
                    return true;
                }
            }
            else if(find(twoToken16DataInstr.begin(), twoToken16DataInstr.end(), opcode) != twoToken16DataInstr.end()) // data1 is 16bit
            {
                string data1 = tokens[1].substr(2);
                string data2 = tokens[1].substr(0, 2);
                if(!isValid8BitData(data1) || !isValid8BitData(data2))
                return false;
                else
                {
                    addToMemory(memory, opcode, data1, data2, pc);
                    return true;
                }
            }
            else // register addressing mode
            {
                if(opcode == "INX" || opcode == "DCX" || opcode == "STAX" || opcode == "LDAX" || opcode == "DAD")
                {
                    if(!isValidRegisterX(tokens[1]))
                    return false;
                }
                else if(opcode == "MOV")
                {
                    if(tokens[1][0] == 'M' && tokens[1][2] == 'M')
                    return false;

                    char dst = tokens[1][0], src = tokens[1][2];
                    if(!isValidRegisterMov(dst) && !isValidRegisterMov(src))
                    return false;
                }
                else
                {
                    if(!isValidRegister(tokens[1]))
                    return false;
                }

                opcode += " " + tokens[1];
                addToMemory(memory, opcode, "", "", pc);
                return true;
            }
        }
    
    case 3: // 3 token instruction
        if(find(thrTokenInstr.begin(), thrTokenInstr.end(), opcode) == thrTokenInstr.end())
        return false;
        else
        {
            if(opcode == "MVI")
            {
                if(!isValidRegisterMov(tokens[1][0]))
                return false;

                opcode += " " + tokens[1];
                string data1 = tokens[2];
                if(!isValid8BitData(data1))
                return false;
                else
                {
                    addToMemory(memory, opcode, data1, "", pc);
                    return true;
                }
            }
            else if(opcode == "LXI") // for LXI
            {
                if(!isValidRegisterX(tokens[1]))
                return false;
                else
                opcode += " " + tokens[1];

                string data1 = tokens[2].substr(2);
                string data2 = tokens[2].substr(0, 2);
                if(!isValid8BitData(data1) || !isValid8BitData(data2))
                return false;
                else
                {
                    addToMemory(memory, opcode, data1, data2, pc);
                    return true;
                }
            }
            else // for SET
            {
                if(!isValidAddress(tokens[1]))
                return false;

                if(!isValid8BitData(tokens[2]))
                return false;

                string data1 = tokens[1].substr(2);
                string data2 = tokens[1].substr(0, 2);
                string data3 = tokens[2];
                addToMemorySet(memory, opcode, data1, data2, data3, pc);
                return true;
            }
        }
    default:
        return false;
    }
    return true;
}

