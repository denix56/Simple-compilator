
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using std::system;
using std::cerr;
#include <string>
using std::getline;
using std::stoi;
#include "Expression.h"



int checkChar(char v, tableEntry t[N], char type, int order)
{
	for (int i = 0; i < order; i++)
		if (v == t[i].getSymbol() && t[i].getType() == type)
			return i;
	return -1;
}

Expression::Expression()
{
	getline(cin, infix);
}
Expression::Expression(string ex) 
	: infix(ex)
{
}
Expression::~Expression()
{
}

bool Expression::isPropOperation(char c, char comp) const
{
	switch (comp)
	{
	case '+':
	case '-':
		return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
	case '*':
	case '/':
	case '%':
		return c == '*' || c == '/' || c == '^' || c == '%';
	case '^':
		return c == '^';
	default:
		return false;
	}
}

string Expression::convertToPostfix()
{
	Stack <char> express;
	string postfix;
	express.push('(');
	infix += ')';
	unsigned i = 0;
	while (i < infix.length())
	{
		if (isNumber(infix[i])) //число
		{
			if (i && infix[i - 1] == ' ')
				postfix += " ";
			postfix += infix[i];
		}
		else 
			if (infix[i] == '(') //левая скобка
				express.push('(');
			else
				if (isPropOperation(infix[i])) //оператор
				{
					if (isPropOperation(infix[i - 1]))
						return "Invalid expression";
					char vertex;
					while (express.pop(vertex))
						if (isPropOperation(vertex, infix[i]))
						{
							postfix += " ";
							postfix += vertex;
						}
						else
						{
							express.push(vertex);
							express.push(infix[i]);
							break;
						}
				}
				else
					if (infix[i] == ')')//правая скобка
					{
						char vertex;
						while (express.pop(vertex))
						{
							if (vertex == '(')
								break;
							if (isPropOperation(vertex))
							{
								postfix += " ";
								postfix += vertex;
							}
						}
					}
					else
						if (infix[i] != ' ') //всё остальное
							return "Invalid expression: unk symb";			
		i++;
	}
	return postfix;
}



int Expression::evaluatePostfixExpression(tableEntry symbolTable[N], long SML[N], int &order, int &varOrder, int &memOrder)
{
	const string postfix = convertToPostfix();
	if (postfix == "Invalid expression")
	{
		cerr << postfix << endl;
		return 0;
	}
	unsigned i = 0;
	Stack <int> numbers;
	string value;
	while (i < postfix.length())
	{
		if (postfix[i] >= 'a' && postfix[i] <= 'z')
		{
			int varAdr = checkChar(postfix[i], symbolTable, 'V');
			if (varAdr == -1)
			{
				if (varOrder == memOrder)
					throw 4;
				symbolTable[order] = tableEntry(postfix[i], 'V', varOrder);
				varOrder--;
				numbers.push(0);
			}
			else
			{
				numbers.push(SML[symbolTable[varAdr].getLoc()]);
			}
		}
		else
			if (isNumber(postfix[i]))
			{
				value += postfix[i];

			}
			else
			{
				if (i && isNumber(postfix[i - 1]))
				{
					if (varOrder == memOrder)
						throw 4;
					symbolTable[order] = tableEntry(stoi(value), 'C', varOrder);
					SML[varOrder] = stoi(value);
					varOrder--;
					numbers.push(varOrder);
					value.clear();
				}
				if (isPropOperation(postfix[i]))
				{
					int x, y;
					numbers.pop(x);
					numbers.pop(y);
					numbers.push(calculate(y, x, postfix[i],symbolTable,SML,varOrder,memOrder));
				}
			}
			i++;
		}

	int res;
	numbers.pop(res);
	return res;
}

int Expression::calculate(int a, int b, char op, tableEntry t[N], long SML[N], int &varOrder, int &memOrder)
{
	//добавлять функцией
	SML[memOrder++] = LOAD * (int)pow(10, 1+floor(log10(a))) + a;
	switch (op)
	{
	case '+':
		SML[memOrder++] = ADD * (int)pow(10, 1+floor(log10(b))) + b;
		SML[memOrder++] = STORE * (int)pow(10, 1+floor(log10(varOrder))) + varOrder;
		return varOrder;
	case '-':
		SML[memOrder++] = SUBSTRACT * (int)pow(10, 1+floor(log10(b))) + b;
		SML[memOrder++] = STORE * (int)pow(10, 1+floor(log10(varOrder))) + varOrder;
		return varOrder;
	case '*':
		SML[memOrder++] = MULTIPLY * (int)pow(10, 1+floor(log10(b))) + b;
		SML[memOrder++] = STORE * (int)pow(10, 1+floor(log10(varOrder))) + varOrder;
		return varOrder;
	case '/':
		if (b == 0)
		{
			cerr << "Divide by 0" << endl;
			throw 5;
		}
		SML[memOrder++] = DIVIDE * (int)pow(10, 1+floor(log10(b))) + b;
		SML[memOrder++] = STORE * (int)pow(10, 1+floor(log10(varOrder))) + varOrder;
		return varOrder;
	case '%':
		if (b == 0)
		{
			cerr << "Divide by 0" << endl;
			return 0;
		}
		SML[memOrder++] = DIVIDE * (int)pow(10, 1+floor(log10(b))) + b;
		SML[memOrder++] = MULTIPLY * (int)pow(10, 1+floor(log10(b))) + b;
		SML[memOrder++] = STORE * (int)pow(10, 1+floor(log10(varOrder))) + varOrder;
		SML[memOrder++] = LOAD * (int)pow(10, 1+floor(log10(a))) + a;
		SML[memOrder++] = SUBSTRACT * (int)pow(10, 1+floor(log10(varOrder))) + varOrder;
		SML[memOrder++] = STORE * (int)pow(10, 1+floor(log10(varOrder))) + varOrder;
		return varOrder;
	case '^':
		SML[varOrder--] = 1;
		SML[memOrder++] = LOAD * (int)pow(10, 1+floor(log10(b))) + b;
		SML[memOrder++] = STORE * (int)pow(10, 1+floor(log10(varOrder))) + varOrder;
		varOrder--;
		SML[memOrder++] = LOAD * (int)pow(10, 1 + floor(log10(a))) + a;
		SML[memOrder++] = BRANCHZERO * (int)pow(10, 1 + floor(log10(10))); // 2 проход
		SML[memOrder++] = MULTIPLY * (int)pow(10, 1+floor(log10(a))) + a;
		SML[memOrder++] = STORE * (int)pow(10, 1 + floor(log10(varOrder))) + varOrder;
		SML[memOrder++] = LOAD * (int)pow(10, 1 + floor(log10(varOrder + 1))) + varOrder + 1;
		SML[memOrder++] = SUBSTRACT * (int)pow(10, 1 + floor(log10(varOrder + 2))) + varOrder + 2;
		SML[memOrder++] = STORE * (int)pow(10, 1 + floor(log10(varOrder + 1))) + varOrder + 1;
		SML[memOrder++] = BRANCH * (int)pow(10, 1 + floor(log10(memOrder - 6))) + memOrder - 6;
		return varOrder;
	default:
		cerr << "Unrecognized operator" << endl;
		return 0;
	}
}