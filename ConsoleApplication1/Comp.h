#pragma once
#include <map>
#include <string>
#include "tableEntry.h"
#include "Expression.h"
using namespace std;
struct loop
{
	int varAdr;
	int val2Adr;
	int stepAdr;
	char varName;
	int branchAdr;
};
class Comp
{
public:
	Comp(long _SML[N], int _flags[N], tableEntry _symbolTable[N]);
	void choose(int line, string command);
private:
	map<string, void (Comp::*)()> commands = { {"rem", &Comp::rem},
											{"input", &Comp::input},
											{"print", &Comp::print},
											{"goto", &Comp::_goto},
											{"let", &Comp::let},
											{"if", &Comp::if_goto},
											{"end", &Comp::end},
											{"for", &Comp::_for},
											{"next", &Comp::next} };
	long *SML;
	int *flags;
	tableEntry *symbolTable;
	string com;
	int order = 0;
	int memOrder = 0;
	int varOrder = N - 2;	
	int line = 0;
	Stack <loop> loops;
	void rem();
	void input();
	void print();
	void _goto();
	void let();
	void if_goto();
	void _for();
	void next();
	void end();
};
