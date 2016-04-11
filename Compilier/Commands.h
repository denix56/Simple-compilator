#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "Expression.h"
using namespace std;

static const int READ = 10,
WRITE = 11,
LOAD = 20,
STORE = 21,
ADD = 30,
SUBSTRACT = 31,
DIVIDE = 32,
MULTIPLY = 33,
BRANCH = 40,
BRANCHNEG = 41,
BRANCHZERO = 42,
HALT = 43;

void input(string command, int varOrder, map<char, int> variables, ofstream &outcomp)
{
	char name = command.substr(command.find(' ') + 1, command.find(' '))[0];
	if (variables.find(name) == variables.end())
		variables.insert(pair<char, int>(name, varOrder - 1));
	else
		varOrder = variables[name];
	int in = READ * round(log10(varOrder)) * 10 + varOrder - 1; //т.к. массив с 0
	outcomp.write((char*)&in, sizeof(in)); // запись в бинарник
}

void let(string command, int varOrder, map<char, int> variables, ofstream &outcomp)
{
	Expression exp(command.substr(command.find('= ') + 1));
}