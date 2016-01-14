#pragma once
#include "Stack.h"
#include <iostream>
using std::istream;
#include <string>
using std::string;
class Expression
{
public:
	Expression();
	~Expression();
	string convertToPostfix();
	int evaluatePostfixExpression();
private:
	bool isPropOperation(char c, char comp = '+') const;
	int calculate(int a, int b, char op) const;
	inline bool isNumber(char c) const
	{
		return c >= '0' && c <= '9';
	}
	bool isValid = true;
	std::string infix;
};

