
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using std::system;
using std::cerr;
#include <string>
using std::string;
using std::getline;
using std::stoi;
#include "Expression.h"
Expression::Expression()
{
	getline(cin, infix);
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
		return  c == '*' || c == '/' || c == '^' || c == '%';
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
					if (infix[i] == ')')  //правая скобка
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
							return "Invalid expression";			
		i++;
	}
	return postfix;
}


int Expression::evaluatePostfixExpression()
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
		if (isNumber(postfix[i]))
		{
			value += postfix[i];
		}
		else
		{
			if (i && isNumber(postfix[i - 1]))
			{
				numbers.push(stoi(value));
				value.clear();
			}
			if (isPropOperation(postfix[i]))
			{
				int x, y;
				numbers.pop(x);
				numbers.pop(y);
				numbers.push(calculate(y, x, postfix[i]));
			}
		}
		i++;
	}
	int res;
	numbers.pop(res);
	return res;
}

int Expression::calculate(int a, int b, char op) const
{
	switch (op)
	{
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		if (b == 0)
		{
			cerr << "Divide by 0" << endl;
			return 0;
		}
		return a / b;
	case '%':
		if (b == 0)
		{
			cerr << "Divide by 0" << endl;
			return 0;
		}
		return a % b;
	case '^':
		return a ^ b;
	default:
		cerr << "Unrecognized operator" << endl;
		return 0;
	}
}