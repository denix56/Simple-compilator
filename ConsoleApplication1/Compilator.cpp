// ConsoleApplication1.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <Windows.h>

#include "tableEntry.h"
#include "Str_Switch.h"
#include "QueueStr.h"
#include "Expression.h"
using namespace std;

bool compile1(QueueStr &comms);
void compile2(int flags[N], tableEntry t[N], long SML[N]);
void getCode(QueueStr &comms, char* path);

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Rus");
	QueueStr comms;
	try
	{
		if (argc == 1)
			getCode(comms, "");
			
		else
		{
			getCode(comms, argv[1]);
			cout << "¬ведите команду:" << endl
				<< "list	дл€ просмотра исходного кода программы" << endl
				<< "start	дл€ начала компил€ции" << endl;
			string command;
			
			bool doIt = true;
			while (doIt)
			{
				getline(cin, command);
				SWITCH(command)
				{
					CASE("list") :
					{
						ifstream source(argv[1]);
						int i = 1;
						while (!source.eof())
						{
							string line;
							getline(source, line);
							cout << i << " " << line << endl;
							i++;
						}
						cout << endl;
						break;
					}
					CASE("start") :
					{
						doIt = false;
						compile1(comms);
						break;
					}
				DEFAULT:
					cout << "Error command" << endl;	
					break;
				}
			}
		}
		
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
	int flags[N];
	for (int i = 0; i < N; i++)
		flags[i] = -1;
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
					SML[memOrder++] = WRITE * 100 + varAdr;
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
					SML[memOrder++] = BRANCH * 100 + lineAdr;
					order++;
					break;
				}

				CASE("end") :
				{
					symbolTable[order++] = tableEntry(order, 'L', memOrder);
					SML[memOrder] = HALT * 100;
					break;
				}
				CASE("let") :
				{
					symbolTable[order++] = tableEntry(order, 'L', memOrder);
					char varName = strtok(NULL, " ")[0];
					int varAdr = checkChar(varName, symbolTable,'V',order);
					if (varAdr == -1)
					{
						symbolTable[order++] = tableEntry(varName, 'V', varOrder);
						varAdr = varOrder;
					}
					if (strtok(NULL, " ")[0] != '=')
						throw exception("Expected '=' ");

					char *expr= strtok(NULL, "\0");
					Expression ex(expr);
					varOrder--;
					int res = ex.evaluatePostfixExpression(symbolTable, SML, flags, order, varOrder, memOrder);		
					SML[memOrder++] = LOAD * 100 + res;
					SML[memOrder++] = STORE * 100 + varAdr;
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
					SML[memOrder++] = LOAD * 100 + varAdr;
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
						SML[memOrder++] = SUBSTRACT * 100 + varAdr2;
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
						SML[memOrder++] = BRANCHZERO * 100 + goLine;
					}
					break;
				}
			}
		}
	SML[N - 1] = -99999;
	out.close();
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
		for (k = m + 1; SML[k] == 0; k++);
		if (SML[i] == STORE * 100 + cell && SML[m] == LOAD * 100 + cell && SML[k] / 100 == STORE)
			{
				for (int j = 0; j < N; j++)
					if (flags[j] == i || flags[j] == i + 1 || flags[j] == i + 2)
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
	}
	/*cout << "ƒл€ запуска программы на симул€торе —имплетрона нажмите любую клавишу, дл€ выхода Esc" << endl;
	bool doIt = true;
	char c;
	cin >> c;
	while (doIt)
		if (c != 27)
		{
			CreateProcess(NULL, L"Simulator.exe MyProgram.sml", NULL, NULL, false, NORMAL_PRIORITY_CLASS, NULL, NULL, NULL, NULL);
			doIt = false;
		}
		else
			if (c == 27)
			{
				doIt = false;
				exit;
			}
			else
				cerr << "Invalid command" << endl;*/
	outcomp.close();
	
}


void getCode(QueueStr &comms, char* path)
{

	if (path != "")
	{
		ifstream source(path);
		while (!source.eof())
		{
			//cout << setw(2) << i + 1 << " ";
			string command;
			getline(source, command);
			comms.enqueue(command);
			if (command.find("end") != -1 && command.find("rem") == -1)
				break;
		}
		source.close();
	}
	else
	{
		int i = 0;
		while (true)
		{
			cout << setw(2) << i + 1 << " ";
			string command;
			getline(cin, command);
			comms.enqueue(command);
			if (command.find("end") != -1 || i > 98)
				break;
			i++;
		}
	}
}