// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "tableEntry.h"
#include "Str_Switch.h"
#include "QueueStr.h"
#include "Expression.h"
using namespace std;

bool compile1(QueueStr &comms);
void compile2(int flags[N], tableEntry t[N], long SML[N]);
void getCode(QueueStr &comms);

int main()
{
	QueueStr comms;
	try
	{
		getCode(comms);
		compile1(comms);
	}
	catch (int a)
	{
		cerr << "Compilation Error " << a << endl;
		system("pause");
		return 0;
	}
	catch (exception e)
	{
		cerr << "Compilation Error " << endl;
		system("pause");
		return 0;
	}
	system("pause");
    return 0;
}

bool compile1(QueueStr &comms)
{
	tableEntry symbolTable[500];
	long SML[N] = { 0 };
	string com;
	int insCount = 0;
	int order = 0;
	int memOrder = 0;
	int varOrder = N - 2;
	int flags[N] = { -1 };
	ofstream out("MyProgram.sc", ios::out);
	while (!comms.isQueueEmpty())
	{	
		comms.dequeue(com);

		out << com << endl;
		char * command = new char[com.length()+1];
		for (unsigned i = 0; i < com.length(); i++)
			command[i] = com[i];

		command[com.length()] = '\0';	
			int line = atoi(strtok(command, " "));
			string variant = string(strtok(NULL, " "));
			SWITCH(variant)
			{
				CASE("rem") :
				{
					symbolTable[order] = tableEntry(line, 'L',memOrder);
					order++;
					break;
				}

				CASE("input") :
				{
					
					symbolTable[order++] = tableEntry(line, 'L', memOrder);
					char varName = strtok(NULL, " ")[0];
					int varAdr = checkChar(varName, symbolTable, 'V', order);
					if (varAdr==-1)
					{
						if (varOrder == memOrder)
							throw 2;
						varAdr = varOrder;
						symbolTable[order] = tableEntry(varName, 'V', varOrder);
					} 
					SML[memOrder++] = READ * 100 + varAdr;
					clog << SML[memOrder - 1] << endl;
					order++;
					varOrder--;
					break;
				}

				CASE("print") :
				{
					symbolTable[order++] = tableEntry(line, 'L', memOrder);
					char varName = strtok(NULL, " ")[0];
					int varAdr;
					int varT = checkChar(varName, symbolTable, 'V', order);
					if (varT == -1)
						throw 3;
					else
						varAdr = symbolTable[varT].getLoc();
					SML[memOrder++] = WRITE * (int)pow(10, 1+floor(log10(varAdr))) + varAdr;
					clog << SML[memOrder - 1] << endl;
					order++;
					break;
				}

				CASE("goto") :
				{
					symbolTable[order] = tableEntry(order, 'L', memOrder);
					int lineAdr = 0;
					int goLine = atoi(strtok(NULL, " "));
					int lineN = checkChar(goLine, symbolTable, 'L', order);
					if (lineN != -1)
					{		
						lineAdr = lineN;
					}
					else
					{
						flags[order] = goLine;
					}
					SML[memOrder++] = BRANCH * (int)pow(10, 1+floor(log10(lineAdr))) + lineAdr;
					clog << SML[memOrder - 1] << endl;
					order++;
					break;
				}

				CASE("end") :
				{
					symbolTable[order++] = tableEntry(order, 'L', memOrder);
					SML[memOrder] = HALT * 100;
					clog << SML[memOrder] << endl;
					break;
				}
				CASE("let") :
				{
					symbolTable[order++] = tableEntry(order, 'L', memOrder);
					char varName = strtok(NULL, " ")[0];
					int varAdr = checkChar(varName, symbolTable,'V',order);
					if (varAdr == -1)
					{
						symbolTable[order] = tableEntry(varName, 'V', varOrder);
					}
					char* rav = strtok(NULL, " ");
					clog << rav << endl;
					if (rav[0] != '=')
						throw exception("Expected '=' ");
					char *expr= strtok(NULL, " ");
					char* prob = " ";
					strcat(expr, prob);
					char* add = strtok(NULL, "");
					clog << add << endl;
					strcat(expr, add);
					clog << expr << endl;
					Expression ex(expr);
					varOrder--;
					int tmp = memOrder;
					int res = ex.evaluatePostfixExpression(symbolTable, SML, order, varOrder, memOrder);
					for (int i = tmp; i < memOrder;i++)
						clog << SML[i] << endl;
					SML[memOrder++] = LOAD * (int)pow(10, 1 + floor(log10(res))) + res;
					clog << SML[memOrder - 1] << endl;
					SML[memOrder++] = STORE * (int)pow(10, 1 + floor(log10(varAdr))) + varAdr;
					clog << SML[memOrder - 1] << endl;
					break;
				}

				CASE("if") :
				{
					symbolTable[order++] = tableEntry(order, 'L', memOrder);
					char varName = strtok(NULL, " ")[0];
					int varAdr = checkChar(varName, symbolTable, 'V', order);
					if (varAdr == -1)
					{
						symbolTable[order] = tableEntry(varName, 'V', varOrder--);
					}
					SML[memOrder++] = LOAD * (int)pow(10, 1 + floor(log10(varAdr))) + varAdr;
					clog << SML[memOrder - 1] << endl;
					if (strtok(NULL, " ") == "==")
					{
						char* str = strtok(NULL, " ");
						int varAdr2;
						if (str[0] >= '0' && str[0] <= '9' || str[0] == '-')
						{
							varAdr2 = varOrder;
							SML[varOrder--] = atoi(str);
						}
						else
						{
							varAdr2 = checkChar(str[0], symbolTable, 'V', order);
							if (varAdr2 == -1)
							{
								symbolTable[order] = tableEntry(str[0], 'V', varOrder--);
							}
						}
						SML[memOrder++] = SUBSTRACT * (int)pow(10, 1 + floor(log10(varAdr2))) + varAdr2;
						clog << SML[memOrder - 1] << endl;
						int lineAdr = 0;
						if (strtok(NULL, " ") != "goto")
							throw exception("Expected goto");
						int goLine = atoi(strtok(NULL, " "));
						int lineN = checkChar(goLine, symbolTable, 'L', order);
						if (lineN != -1)
						{
							lineAdr = lineN;
						}
						else
						{
							flags[order] = goLine;
						}
						SML[memOrder++] = BRANCHZERO * (int)pow(10, 1 + floor(log10(goLine))) + goLine;
						clog << SML[memOrder - 1] << endl;
					}
					break;
				}
			}
		}
	SML[N - 1] == -99999;	
	system("pause");
	compile2(flags, symbolTable, SML);
}

void compile2(int flags[N], tableEntry t[N], long SML[N])
{
	for (int i = 0; i < N;i++)
		if (flags[i] != -1)
			for (int j = 0; j < N; j++)
			{
				if (t[j].getSymbol() == flags[i] && t[j].getType() == 'L')
					SML[t[j].getLoc()] += flags[i];
				else
					if (t[j].getSymbol() == flags[i] && t[j].getType() != 'L')
						throw exception("The line was expected");
				break;
			}
	for (int i = 0; SML[i]; i++)
	{
		long cell = SML[i] % 100;
		bool isGoTo = false;
		int m, k;
		for (m = i + 1; SML[m] == 0; m++);
		for (k = m + 1; SML[k] == 0; k++)
			if (SML[i] == STORE * 100 + cell && SML[m] == LOAD * 100 + cell && SML[k] / 100 == STORE)
			{
				for (int j = 0; j < N; j++)
					if (flags[j] == i || flags[j] != i + 1 || flags[j] != i + 2)
					{
						isGoTo = true;
						break;
					}
				if (!isGoTo)
				{
					SML[i] = STORE * 100 + SML[i + 2] % 100;
					SML[i + 1] = SML[i + 2] = 0;
				}
			}
	}
	ofstream outcomp("MyProgram.sml", ios::binary | ios::out);
	for (int i = 0; i < N; i++)
	{
		outcomp.write((char*)&SML[i], sizeof SML[i]);
		clog << SML[i] << endl;
	}
	
}


void getCode(QueueStr &comms)
{
	//static int count = 0;
	int i = 0;
	while (true)
	{
		cout << setw(2) << i + 1 << " ";
		string command;
		getline(cin, command);
		//char* com = new char[command.length()+1];
		//for (int i = 0; i < command.length(); i++)
		//	com[i] = command[i];
		//com[command.length()] = '\0';
	//	clog << endl << command << " " << com << endl;
		comms.enqueue(command);
		if (command.find("end") != -1 || i>98)
			break;
		i++;
	}
}



