#include <iostream>
#include <map>
#include <vector>
using namespace std;
#ifndef LOGICAL_H
#define LOGICAL_H

extern vector<string> logical_instr;

void executeLogicalInstr(string , string , string [], vector<bool> &, map<string, string> &, string &);
void complement(string [], string &);
void compare(string , string [], vector<bool> &, string &, bool);

#endif