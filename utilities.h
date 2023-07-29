#include <iostream>
#include <map>
#include <vector>
using namespace std;
#ifndef UTILITIES_H
#define UTILITIES_H

extern vector<string> oneTokenInstr;
extern vector<string> twoTokenInstr;
// nondata2Token = {"MOV",  "ADD", "SUB", "INR", dcr ,inx, dcx, dad, sui, cmp}
extern vector<string> twoToken8DataInstr;
extern vector<string> twoToken16DataInstr;
extern vector<string> thrTokenInstr;

extern vector<string> oneByteInstr;
extern vector<string> twoByteInstr;
extern vector<string> thrByteInstr;

bool isValidAddress(string);
bool isValidInstruction(string, map<string,string>&, string&);
bool isValidOpcode(string);
bool isValid8BitData(string);

void addToMemorySet(map<string, string>&, string, string, string, string, string&);
void addToMemory(map<string, string>&, string, string, string, string&);
string incrementProgramCounter(string);
char plusOne(char);
char minusOne(char); 

bool checkParity(string);
bool isValidRegister(string);

#endif