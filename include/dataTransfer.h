#include <iostream>
#include <map>
#include <vector>
using namespace std;
#ifndef DATATRANSFER_H
#define DATATRANSFER_H

extern vector<string> data_transfer_instr;

bool executeDataTransferInstr(string, string, string[], map<string, string>&, string&);
bool setToMemory(string, string, map<string, string> &, string &);
void move(string , string [], map<string, string> &, string &, bool);
void load(string , string [], map<string, string> &, string &);
void loadXI(string, string [], string &);
void set(string, string [], map<string, string> &, string &);
void loadHL(string, string [], map<string, string> &, string &);
bool setHL(string, string [], map<string, string> &, string &);
bool setX(string, string [], map<string, string> &, string &);
void loadX(string, string [], map<string, string> &, string &);
void exchange(string [], string &);

#endif