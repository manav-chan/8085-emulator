#include <iostream>
#include <map>
#include <vector>
using namespace std;
#ifndef ARITHMETIC_H
#define ARITHMETIC_H

extern vector<string> arith_instr;

void executeArithInstr(string, string, string [], vector<bool> &, map<string, string> &, string&);
int hexaCharToDec(char);
char decToHexaChar(int);
void add(string, string [], vector<bool> &, string &, bool);
void addWithCarry(string , string [], vector<bool> &, string &, bool);

void sub(string, string [], vector<bool> &, string &, bool) ;
void increment(string, string [], vector<bool> &, string &);
void decrement(string , string [], vector<bool> &, string &);
void incrementx(string , string [], vector<bool> &, string &);
void decrementx(string , string [], vector<bool> &, string &);
void doubleAdd(string , string [], vector<bool> &, string &);

#endif