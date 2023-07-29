#include "utilities.h"
#include "arithmetic.h"
#include "dataTransfer.h"
#include "logical.h"
#include "branching.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

class CPU
{
    private:
        string registers[7];
        string pc, sp;
        vector<bool> flag; // flag register
        map<string, string> memory;
        string main_end; // stores the address of the last instruction of the main block.
    public:
        CPU()
        {
            for(int i=0; i<7; i++)
            registers[i] = "00";

            for(int i=0; i<5; i++)
            flag.push_back(0);

            pc = sp = "";
        }

        string input()
        {
            cout << "8085 MICROPROCESSOR\n";
            cout << "Enter starting memory location: ";
            cin >> pc;

            while(isValidAddress(pc) == false)
            {
                cout<<"Memory address is not valid. Please enter an address between 0000 and FFFF\n";
                cin >> pc;
            }
            cin.ignore();
            return pc;
        }

        string type_program() // returns the address of the last program instruction
        {
            string main_end = pc;
            string instr;
            cout << main_end << " ";
            while(getline(cin, instr))
            {
                if(instr == "STOP")
                break;

                if(!isValidInstruction(instr, memory, main_end)) // check for valid instruction and updates memory if valid
                {
                    cout<<"\nPlease enter a valid instruction\n";
                }
                cout << main_end <<" ";
            }
            cout<<"Typed Successfully\n";
            return main_end;
        }

        string inputFile(string name)
        {
            ifstream fin;
            fin.open(name);

            string main_end = pc;
            string instr;
            int countLine = 0;
            while(getline(fin, instr))
            {
                countLine ++;
                if(!isValidInstruction(instr, memory, main_end)) // check for valid instruction and updates memory if valid
                {
                    cout<<"\nPlease enter a valid instruction at Line: " << countLine<<endl;
                }
            }
            fin.close();
            cout<<"Input Taken successfully"<<endl;
            return main_end;
        }

        string execute()
        {
            string main_start = pc;
            cout<<"Executing"<<endl;
            while(memory[pc] != "HLT")
            {
                string data = memory[pc];
                auto found = data.find(' '); // extracting opcode and operands from an instruction
    
                string opcode = "", operand = ""; 
                if(found == string::npos)
                {
                    opcode = data;
                }
                else
                {
                    opcode = data.substr(0, found);
                    operand = data.substr(found + 1);
                }

                if(find(arith_instr.begin(), arith_instr.end(), opcode) != arith_instr.end()) // checks if arithmetic instruction
                {
                    executeArithInstr(opcode, operand, registers, flag, memory, pc); // executes the arithmetic instruction and updates the program counter
                }
                else if(find(data_transfer_instr.begin(), data_transfer_instr.end(), opcode) != data_transfer_instr.end()) // checks if data transfer instruction
                {
                    if(!executeDataTransferInstr(opcode, operand, registers, memory, pc))
                    {
                        cout<<"Accessing invalid memory location. Exiting program\n";
                        exit(0);
                    }
                }
                else if(find(logical_instr.begin(), logical_instr.end(), opcode) != logical_instr.end())
                {
                    executeLogicalInstr(opcode, operand, registers, flag, memory, pc);
                }
                else if(find(branching_instr.begin(), branching_instr.end(), opcode) != branching_instr.end())
                {
                    executeBranchingInstr(opcode, operand, flag, memory, pc);
                }
            }
            cout<<"Execution Done\n";
            return main_start;
        }

        void memoryInput()
        {
            int ch;
            do
            {
                char c;
                cout<<"Do you want to set any memory location? (y/n) ";
                cin >> c;
                
                string mem, data;
                switch(c)
                {
                    case 'y':
                        ch = 1;
                        cout<<"Enter memory location to set: ";
                        cin >> mem;
                        if(!isValidAddress(mem))
                        {
                            cout<<"Invalid Address"<<endl;
                            break;
                        }
                        cout<<"Enter data to set: ";
                        cin >> data;
                        if(!isValid8BitData(data))
                        {
                            cout<<"Invalid Data"<<endl;
                            break;
                        }
                        memory[mem] = data;
                        break;
                    case 'n':
                        ch = 0;
                        break;
                    default:
                        cout << "Invalid character entered. Please enter again\n";    
                }
            } while(ch != 0);
        }

        void display(string main_start, string main_end)
        {
            /* Registers */
            cout<<"----Content of the Registers----\n";
            for(int i=0; i<7; i++)
            {
                if(i == 5)
                cout<<"H : ";
                else if(i == 6)
                cout<<"L : ";
                else
                cout<<(char)('A' + i)<<" : ";

                cout<<registers[i]<<endl;
            }
            cout<<endl<<"----Content of the Flag register----\n";
            /* Flags */
            cout<<"Sign Flag : "<<flag[0]<<endl;
            cout<<"Zero Flag : "<<flag[1]<<endl;
            cout<<"Auxillary Carry Flag : "<<flag[2]<<endl;
            cout<<"Parity Flag : "<<flag[3]<<endl;
            cout<<"Carry Flag : "<<flag[4]<<endl;

            // string memIp;
            // cout<<endl<<"Enter memory location to display: ";
            // cin>>memIp;
            // cout<<memIp<<" : "<<memory[memIp]<<endl;

            cout<<endl<<"----Occupied Memory Locations----\n";
            bool f1 = false, f2 = false;
            for(auto x: memory)
            {
                if(x.first == main_start)
                f1 = true;
                if(x.first == main_end)
                f2 = true;

                // if((f1 && f2) || (!f1 && !f2)) // memmory other than program memory
                cout<<x.first<<" : "<<x.second<<" "<<endl;
            }

        }
};

int main(int argc, char **argv)
{
    CPU object;
    string main_end, main_start;
    main_start = object.input();
    if(argc == 2)
    {
        string file_name = argv[1];
        main_end = object.inputFile(file_name);
    }
    else
    {
        main_end = object.type_program();
    }
    // cout<<main_start<<" "<<main_end<<endl;
    object.memoryInput();
    main_start = object.execute();
    object.display(main_start, main_end);
    return 0;
}