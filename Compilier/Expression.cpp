
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using std::system;
using std::cerr;
#include <string>
#include "Expression.h"
using namespace std;


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
		if (isNumber(infix[i]) || infix[i] >= 'a' && infix[i] <= 'z') //�����
		{
			if (i && infix[i - 1] == ' ')
				postfix += " ";
			postfix += infix[i];
		}
		else
				if (infix[i] == '(') //����� ������
					express.push('(');
			else
				if (isPropOperation(infix[i])) //��������
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
					if (infix[i] == ')')//������ ������
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
						if (infix[i] != ' ') //�� ���������
						{
							return "Invalid expression: unk symb";
						}
		i++;
	}
	return postfix;
}



int Expression::evaluatePostfixExpression(tableEntry symbolTable[N], long SML[N], int flags[N], int &order, int &varOrder, int &memOrder)
{
	string postfix = convertToPostfix();
	if (postfix == "Invalid expression")
	{
		return 0;
	}
	unsigned i = 0;
	Stack <int> numbers;
	string value;
	while (i < postfix.length())
	{
		if (postfix[i] >= 'a' && postfix[i] <= 'z')
		{
			int varTab = checkChar(postfix[i], symbolTable, 'V');
			if (varTab == -1)
			{
				if (varOrder == memOrder)
				{
					throw 4;
				}
				symbolTable[order] = tableEntry(postfix[i], 'V', varOrder--);
				numbers.push(0);
			}
			else
			{
				numbers.push(symbolTable[varTab].getLoc());
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
					numbers.push(varOrder);
					varOrder--;
					value.clear();
				}
				if (isPropOperation(postfix[i]))
				{
					if (postfix[i] == '/' && i + 2 < postfix.length() && postfix[i + 2] == '^')
					{
						int x, y, z;
						numbers.pop(x);
						numbers.pop(y);
						numbers.pop(z);
						numbers.push(calculate(z, y, '^', symbolTable, SML, flags, varOrder, memOrder));
						numbers.pop(z);
						// \ - ������ ���������� �����
						varOrder--;
						numbers.push(calculate(z, x, '\\', symbolTable, SML, flags, varOrder, memOrder));
						postfix.erase(i + 1, 2);
					}
					else
					{
						int x, y;
						numbers.pop(x);
						numbers.pop(y);
						numbers.push(calculate(y, x, postfix[i], symbolTable, SML, flags, varOrder, memOrder));
					}
					varOrder--;
				}
			}
			i++;
		}
	int res;
	numbers.pop(res);
	return res;
}

int Expression::calculate(int a, int b, char op, tableEntry t[N], long SML[N], int flags[N], int &varOrder, int &memOrder)
{
	//��������� ��������
	switch (op)
	{
	case '+':
		SML[memOrder++] = LOAD * 1000 + a;
		SML[memOrder++] = ADD * 1000 + b;
		SML[memOrder++] = STORE * 1000 + varOrder;
		return varOrder;
	case '-':
		SML[memOrder++] = LOAD * 1000 + a;
		SML[memOrder++] = SUBSTRACT * 1000 + b;
		SML[memOrder++] = STORE * 1000 + varOrder;
		return varOrder;
	case '*':
		SML[memOrder++] = LOAD * 1000 + a;
		SML[memOrder++] = MULTIPLY * 1000 + b;
		SML[memOrder++] = STORE * 1000 + varOrder;
		return varOrder;
	case '/':
		if (b == 0)
		{
			throw 5;
		}
		SML[memOrder++] = LOAD * 1000 + a;
		SML[memOrder++] = DIVIDE * 1000 + b;
		SML[memOrder++] = STORE * 1000 + varOrder;
		return varOrder;
	case '%':
		if (b == 0)
		{
			return 0;
		}
		SML[memOrder++] = LOAD * 1000 + a;
		SML[memOrder++] = DIVIDE * 1000 + b;
		SML[memOrder++] = MULTIPLY * 1000 + b;
		SML[memOrder++] = STORE * 1000 + varOrder;
		SML[memOrder++] = LOAD * 1000 + a;
		SML[memOrder++] = SUBSTRACT * 1000 + varOrder;
		SML[memOrder++] = STORE * 1000 + varOrder;
		return varOrder;
	case '^':
		//
		SML[varOrder--] = 1;
		SML[memOrder++] = LOAD * 1000 + b;
		SML[memOrder++] = BRANCHZERO * 1000 + memOrder + 3;//
		SML[memOrder++] = BRANCHPOS * 1000 + memOrder + 4;//
		//SML[memOrder++] = BRANCHNEG * 1000 + memOrder + 18;//
		
		//���� ������� �������
		SML[memOrder++] = LOAD * 1000 + varOrder + 2;
		SML[memOrder++] = STORE * 1000 + varOrder + 1;
		SML[memOrder++] = BRANCH * 1000 + memOrder + 14;

		//���� �������������
		
		SML[memOrder++] = SUBSTRACT * 1000 + varOrder + 1;
		SML[memOrder++] = STORE * 1000 + varOrder;
		varOrder--;// ��� �����
		SML[memOrder++] = LOAD * 1000 + a;
		SML[memOrder++] = STORE * 1000 + varOrder;
		SML[memOrder++] = LOAD * 1000 + varOrder + 1;
		SML[memOrder++] = BRANCHZERO * 1000 + memOrder + 8; //  ������
		SML[memOrder++] = LOAD * 1000 + varOrder;
		SML[memOrder++] = MULTIPLY * 1000 + a;
		SML[memOrder++] = STORE * 1000 + varOrder;
		SML[memOrder++] = LOAD * 1000 + varOrder + 1;
		SML[memOrder++] = SUBSTRACT * 1000 + varOrder + 2;
		SML[memOrder++] = STORE * 1000 + varOrder + 1;
		SML[memOrder++] = BRANCH * 1000 + memOrder - 7;
		SML[memOrder++] = LOAD * 1000 + varOrder;
		//SML[memOrder++] = BRANCH * 1000 + memOrder + 21;//������� � ��������� �������
		return varOrder;
		//������
	case '\\':
		SML[memOrder++] = LOAD * 1000 + a;//
		SML[memOrder++] = BRANCHNEG * 1000 + memOrder + 3;
		SML[memOrder++] = BRANCHZERO * 1000 + memOrder + 3;
		SML[memOrder++] = BRANCHPOS * 1000 + memOrder + 3;//
		SML[memOrder++] = SUBSTRACT * 1000 + a;//
		SML[memOrder++] = BRANCH * 1000 + memOrder + 15;//
		SML[memOrder++] = STORE * 1000 + varOrder;// result
		varOrder--;

		SML[memOrder++] = STORE * 1000 + varOrder;//div
		varOrder--;

		SML[varOrder--] = 2;
		//SML[memOrder++] = BRANCHZERO * 1000 + memOrder + 1;//����
		SML[memOrder++] = LOAD * 1000 + a;
		SML[memOrder++] = DIVIDE * 1000 + varOrder + 2;
		SML[memOrder++] = ADD * 1000 + varOrder + 2;
		SML[memOrder++] = DIVIDE * 1000 + varOrder + 1;
		SML[memOrder++] = STORE * 1000 + varOrder + 2;
		SML[memOrder++] = SUBSTRACT * 1000 + varOrder + 3;
		SML[memOrder++] = BRANCHNEG * 1000 + memOrder + 2;
		SML[memOrder++] = BRANCHZERO * 1000 + memOrder + 4;
		SML[memOrder++] = LOAD * 1000 + varOrder + 2;
		SML[memOrder++] = STORE * 1000 + varOrder + 3;
		SML[memOrder++] = BRANCH * 1000 + memOrder - 10;//
		varOrder += 3;
		return varOrder;
	default:
		cerr << "Unrecognized operator" << endl;
		throw std::exception("Unrecognized operator");
		return 0;
	}
}