#pragma once
#include <iostream>
using std::istream;
#include <string>
using std::string;
#include "Stack.h"
#include "Queue.h"
#include "tableEntry.h"

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

class Expression
{
public:
	Expression();
	Expression(string ex);
	~Expression();
	string convertToPostfix();
	int evaluatePostfixExpression(tableEntry symbolTable[N], long SML[N], int flags[N], int &order, int &varOrder, int &memOrder);
private:
	bool isPropOperation(char c, char comp = '+') const;
	int calculate(int a, int b, char op, tableEntry t[N], long SML[N], int flags[N], int &varOrder, int &memOrder);
	inline bool isNumber(char c) const
	{
		return c >= '0' && c <= '9';
	}
	bool isValid = true;
	std::string infix;
};

int checkChar(char v, tableEntry t[N], char type, int order = N);