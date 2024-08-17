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
#include <chrono>
#include <thread>
using namespace std;

class CPU
{
    private:
        string registers[7]; // 7 registers
        string pc, sp; // program counter and stack pointer
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
            cout << GREEN<<"\n-----------8085 MICROPROCESSOR-----------\n\n"<<RESET;
            cout << CYAN<<"Enter starting memory location: "<<RESET;
            cin >> pc;

            while(isValidAddress(pc) == false)
            {
                cout<<RED<<"Memory address is not valid. Please enter an address between 0000 and FFFF\n"<<RESET;
                cin >> pc;
            }
            cin.ignore();
            return pc;
        }

        string type_program() // returns the address of the last program instruction
        {
            string main_end = pc;
            string instr;
            cout << endl<<MAGENTA<<main_end << " "<<RESET;
            while(getline(cin, instr))
            {
                if(instr == "STOP")
                break;

                if(!isValidInstruction(instr, memory, main_end)) // check for valid instruction and updates memory if valid
                {
                    cout<<RED<<"\nPlease enter a valid instruction\n"<<RESET;
                }
                cout << MAGENTA<<main_end <<" "<<RESET;
            }
            cout<<GREEN<<"\nTyped Successfully!\n\n"<<RESET;
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
                if(instr.find('/') == 0) {
                    continue;
                }
                
                countLine ++;
                instr = trim(instr); // remove trailing spaces
                if(!isValidInstruction(instr, memory, main_end)) // check for valid instruction and updates memory if valid
                {
                    cout<<RED<<"Please enter a valid instruction at Line: " << RESET<<countLine<<endl;
                    exit(0);
                }
            }
            fin.close();
            cout<<GREEN<<"\nInput Taken successfully!"<<RESET<<endl;
            return main_end;
        }

        string execute()
        {
            string main_start = pc;
            cout<<endl<<YELLOW<<"Executing";
            for (int i = 0; i < 3; ++i) 
            {
                cout << ".";
                cout.flush();
                this_thread::sleep_for(chrono::seconds(1)); // Wait for 1 second
            }

            cout <<RESET<<endl; // Move to the next line after the animation

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
                        cout<<RED<<"Accessing invalid memory location. Exiting program\n"<<RESET;
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
            cout<<GREEN<<"\nExecution Done!\n\n"<<RESET;
            this_thread::sleep_for(chrono::seconds(1));
            return main_start;
        }

        void memoryInput()
        {
            int ch;
            do
            {
                string input;
                cout<<YELLOW<<"Do you want to set any memory location? (y/n) "<<RESET;
                cin >> input;
                while(input.length() != 1)
                {
                    cout<<RED<<"Invalid character entered. Please enter again\n"<<RESET;
                    cin >> input;
                }

                char c = input[0];
                string mem, data;
                switch(c)
                {
                    case 'y':
                        ch = 1;
                        cout<<CYAN<<"Enter memory location to set: "<<RESET;
                        cin >> mem;
                        if(!isValidAddress(mem))
                        {
                            cout<<RED<<"Invalid Address"<<RESET<<endl;
                            break;
                        }
                        cout<<CYAN<<"Enter data to set: "<<RESET;
                        cin >> data;
                        if(!isValid8BitData(data))
                        {
                            cout<<RED<<"Invalid Data"<<RESET<<endl;
                            break;
                        }
                        memory[mem] = data;
                        break;
                    case 'n':
                        ch = 0;
                        break;
                    default:
                        cout <<RED<< "Invalid character entered. Please enter again\n"<<RESET;
                }
            } while(ch != 0);
        }

        void display(string main_start, string main_end)
        {
            /* Registers */
            cout<<CYAN<<"----Content of the Registers----\n\n"<<RESET;
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
            cout<<endl<<CYAN<<"----Content of the Flag register----\n\n"<<RESET;
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

            bool inProgMem = false;
            string programMemory = "";
            string otherMemory = "";
            for(auto x: memory)
            {
                if(x.first == main_start)
                inProgMem = true;
                if(x.first >= main_end)
                inProgMem = false;

                if(inProgMem) // program memory
                programMemory += MAGENTA + x.first + RESET + " : " + x.second + "\n";
                else // other memory
                otherMemory += MAGENTA + x.first + RESET + " : " + x.second + "\n";
            }
            cout<<endl<<CYAN<<"----Program Memory----\n\n"<<RESET;
            cout<<programMemory<<endl;
            cout<<endl<<CYAN<<"----Other Memory----\n\n"<<RESET;
            cout<<otherMemory<<endl;
        }
};

int main(int argc, char **argv)
{
    CPU object;
    string main_end, main_start;
    main_start = object.input();
    if(argc == 2)
    {
        // user should include SET commands to set memory locations in file.
        string file_name = argv[1];
        main_end = object.inputFile(file_name);
    }
    else
    {
        main_end = object.type_program();
        object.memoryInput();
    }
    // cout<<main_start<<" "<<main_end<<endl;
    main_start = object.execute();
    object.display(main_start, main_end);
    return 0;
}