// ConsoleApplication1.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "Str_Switch.h"
#include "Comp.h"
#include <queue>
using namespace std;

void compile1(queue <string> &comms, string progName);
void compile2(int flags[N], tableEntry t[N], long SML[N], string progName);

void getCode(queue <string> &comms, string path);

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Rus");
	queue <string> comms;
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
			cout << "������� �������:" << endl
				<< "list	��� ��������� ��������� ���� ���������" << endl
				<< "start	��� ������ ����������" << endl;
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
void compile1(queue <string> &comms, string progName)
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

	Comp manufact(SML, flags, symbolTable); //���������� ������

	while (!comms.empty())
	{	
		com = comms.front();
		comms.pop();
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
		long cell = SML[i] % 1000;
		bool isGoTo = false;
		int m, k;
		for (m = i + 1; SML[m] == 0; m++);
		for (k = m + 1; SML[k] == 0; k++);
		if (SML[i] == STORE * 1000 + cell && SML[m] == LOAD * 1000 + cell && SML[k] / 1000 == STORE)
			{
				for (int j = 0; j < N; j++)
					if (flags[j] == i || flags[j] == i + 1 || flags[j] == i + 2)
					{
						isGoTo = true;
						break;
					}
				if (!isGoTo)
				{
					SML[i] = STORE * 1000 + SML[i + 2] % 1000;
					SML[i + 1] = SML[i + 2] = 0;
				}
			}
		//TODO ����������� ����������� ���� �������� 0-�� ������. ������� SML vector
	}
	progName += ".sml";
	ofstream outcomp(progName, ios::binary | ios::out);
	for (int i = 0; i < N; i++)
	{
		//outcomp << SML[i] << endl;
		outcomp.write((char*)&SML[i], sizeof SML[i]);
	}
	/*cout << "��� ������� ��������� �� ���������� ����������� ������� ����� �������, ��� ������ Esc" << endl;
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


void getCode(queue <string> &comms, string path)
{
	if (path!="")
	{
		ifstream source(path);
		while (!source.eof())
		{
			//cout << setw(2) << i + 1 << " ";
			string command;
			getline(source, command);
			comms.push(command);
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
			comms.push(command);
			if (command.find("end") != -1 || i > 498)
				break;
			i++;
		}
	}
}