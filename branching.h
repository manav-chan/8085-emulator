#include <iostream>
#include <map>
#include <vector>
using namespace std;
#ifndef BRANCHING_H
#define BRANCHING_H

extern vector<string> branching_instr;

void executeBranchingInstr(string , string, vector<bool> &, map<string, string>&, string &);
void jump(string, vector<bool> &, string &);
void jumpCarry(string, vector<bool> &, string &);
void jumpNotCarry(string, vector<bool> &, string &);
void jumpZero(string, vector<bool> &, string &);
void jumpNotZero(string, vector<bool> &, string &);

#endif