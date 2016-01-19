// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "Str_Switch.h"
#include "QueueStr.h"
#include "Comp.h"
using namespace std;

void compile1(QueueStr &comms, string progName);
void compile2(int flags[N], tableEntry t[N], long SML[N], string progName);

void getCode(QueueStr &comms, string path);

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Rus");
	QueueStr comms;
	try
	{
		if (argc == 1)
		{
			getCode(comms, "");
			compile1(comms, "");
		}
			
		else
		{
			getCode(comms, argv[1]);
			cout << "Введите команду:" << endl
				<< "list	для просмотра исходного кода программы" << endl
				<< "start	для начала компиляции" << endl;
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
						compile1(comms, strtok(argv[1], "."));
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
    return 0;
}

//
void compile1(QueueStr &comms, string progName)
{
	tableEntry symbolTable[500];
	long SML[N] = { 0 };
	string com;
	int flags[N];
	for (int i = 0; i < N; i++)
		flags[i] = -1;
	string name = progName;
	if (progName != "")
	{
		name += ".sc";
	}
	else
		name = "MyProgram.sc";
	ofstream out(name);

	Comp manufact(SML, flags, symbolTable); //обработчик команд

	while (!comms.isQueueEmpty())
	{	
		comms.dequeue(com);
		out << com << endl;
		char * command = new char[com.length()+1];
		for (unsigned i = 0; i < com.length(); i++)
			command[i] = com[i];

		command[com.length()] = '\0';	
		int line = atoi(strtok(command, " "));

		manufact.choose(line, string(strtok(NULL, " ")));
		}
	SML[N - 1] = -99999;
	out.close();
	compile2(flags, symbolTable, SML, name.substr(0,name.find(".")));
}

void compile2(int flags[N], tableEntry t[N], long SML[N], string progName)
{
	for (int i = 0; i < N;i++)
		if (flags[i] != -1)
			for (int j = i; j < N; j++)
			{
				clog << t[i].getSymbol() << "	" << t[i].getType();
				if (t[j].getSymbol() == flags[i] && t[j].getType() == 'L')
				{
					SML[t[i].getLoc()] += t[j].getLoc();
					break;
				}
				else
					if (t[j].getSymbol() == flags[i] && t[j].getType() != 'L')
						throw exception("The line was expected");
				
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
		//TODO Реализовать оптимизацию путём удаления 0-ых команд. Сделать SML vector
	}
	progName += ".sml";
	ofstream outcomp(progName, ios::binary | ios::out);
	for (int i = 0; i < N; i++)
	{
		//outcomp << SML[i] << endl;
		outcomp.write((char*)&SML[i], sizeof SML[i]);
	}
	/*cout << "Для запуска программы на симуляторе Симплетрона нажмите любую клавишу, для выхода Esc" << endl;
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


void getCode(QueueStr &comms, string path)
{
	if (path!="")
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