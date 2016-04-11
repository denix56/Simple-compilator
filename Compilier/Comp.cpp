#include "Comp.h"

Comp::Comp(long _SML[N], int _flags[N], tableEntry _symbolTable[N])
{
	SML = _SML;
	flags = _flags;
	symbolTable = _symbolTable;
}

void Comp::choose(int _line, string name)
{
	line = _line;
	(this->*commands[name])(); //вызов подходящей функции
}

void Comp::rem()
{
	symbolTable[order] = tableEntry(line, 'L', memOrder);
	order++;
}

void Comp::input()
{
	symbolTable[order++] = tableEntry(line, 'L', memOrder);
	char varName = strtok(NULL, " ")[0];
	int varTab = checkChar(varName, symbolTable, 'V', order);
	int varAdr;
	if (varTab == -1)
	{
		if (varOrder == memOrder)
			throw exception("Out of memory");
		varAdr = varOrder;
		symbolTable[order] = tableEntry(varName, 'V', varOrder);
	}
	else
		varAdr = symbolTable[varTab].getLoc();
	SML[memOrder++] = READ * 1000 + varAdr;
	order++;
	varOrder--;
}

void Comp::print()
{
	symbolTable[order++] = tableEntry(line, 'L', memOrder);
	char varName = strtok(NULL, " ")[0];
	int varAdr;
	int varT = checkChar(varName, symbolTable, 'V', order);
	if (varT == -1)
		throw 3;
	else
		varAdr = symbolTable[varT].getLoc();
	SML[memOrder++] = WRITE * 1000 + varAdr;
	order++;
}

void Comp::_goto()
{
	symbolTable[order] = tableEntry(line, 'L', memOrder);
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
	SML[memOrder++] = BRANCH * 1000 + lineAdr;
	order++;
}

void Comp::let()
{
	symbolTable[order++] = tableEntry(line, 'L', memOrder);
	char varName = strtok(NULL, " ")[0];
	int varTab= checkChar(varName, symbolTable, 'V', order);
	int varAdr;
	if (varTab == -1)
	{
		varAdr = varOrder;
		symbolTable[order++] = tableEntry(varName, 'V', varOrder--);	
	}
	else
		varAdr = symbolTable[varTab].getLoc();
	if (strtok(NULL, " ")[0] != '=')
		throw exception("Expected '=' ");

	char *expr = strtok(NULL, "\0");
	Expression ex(expr);
	int res = ex.evaluatePostfixExpression(symbolTable, SML, flags, order, varOrder, memOrder);
	SML[memOrder++] = LOAD * 1000 + res;
	SML[memOrder++] = STORE * 1000 + varAdr;
}

void Comp::if_goto()
{
	symbolTable[order++] = tableEntry(line, 'L', memOrder);
	char varName = strtok(NULL, " ")[0];
	int varTab = checkChar(varName, symbolTable, 'V', order);
	int varAdr;

	if (varTab == -1)
		symbolTable[order++] = tableEntry(varName, 'V', varOrder--);
	else
		varAdr = symbolTable[varTab].getLoc();

	char* symb = strtok(NULL, " ");
	if (strcmp(symb, "==")==0 || strcmp(symb, ">") == 0 || strcmp(symb, "<") == 0 || 
		strcmp(symb, "<=") == 0 || strcmp(symb, ">=") == 0 || strcmp(symb, "!=") == 0)
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
			int varTab2 = checkChar(str[0], symbolTable, 'V', order);
			if (varTab2 == -1)
			{
				varAdr2 = varOrder;
				symbolTable[order++] = tableEntry(str[0], 'V', varOrder--);
			}
			else
				varAdr2 = symbolTable[varTab2].getLoc();
		}
		int lineAdr = 0;
		if (strcmp(strtok(NULL, " "), "goto") != 0)
			throw exception("Expected goto");
		int goLine = atoi(strtok(NULL, " "));
		int lineN = checkChar(goLine, symbolTable, 'L', order);

		if (lineN != -1)
			lineAdr = lineN;
		else
		{
			symbolTable[order] = tableEntry(line, 'L', memOrder + 2);
			flags[order++] = goLine;
		}
		if (strcmp(symb, "==") == 0)
		{
			SML[memOrder++] = LOAD * 1000 + varAdr;
			SML[memOrder++] = SUBSTRACT * 1000 + varAdr2;
			SML[memOrder++] = BRANCHZERO * 1000 + lineAdr;
		}
		else
			if (strcmp(symb, ">") == 0)
			{
				SML[memOrder++] = LOAD * 1000 + varAdr2;
				SML[memOrder++] = SUBSTRACT * 1000 + varAdr;
				SML[memOrder++] = BRANCHNEG * 1000 + lineAdr;
			}
			else
				if (strcmp(symb, "<") == 0)
				{
					SML[memOrder++] = LOAD * 1000 + varAdr;
					SML[memOrder++] = SUBSTRACT * 1000 + varAdr2;
					SML[memOrder++] = BRANCHNEG * 1000 + lineAdr;
				}
				else
					if (strcmp(symb, ">=") == 0)
					{
						SML[memOrder++] = LOAD * 1000 + varAdr2;
						SML[memOrder++] = SUBSTRACT * 1000 + varAdr;
						SML[memOrder++] = BRANCHNEG * 1000 + lineAdr;
						symbolTable[order] = tableEntry(line, 'L', memOrder);
						flags[order++] = goLine;
						SML[memOrder++] = BRANCHZERO * 1000 + lineAdr;
					}
					else
						if (strcmp(symb, "<=") == 0)
						{
							SML[memOrder++] = LOAD * 1000 + varAdr;
							SML[memOrder++] = SUBSTRACT * 1000 + varAdr2;
							SML[memOrder++] = BRANCHNEG * 1000 + lineAdr;
							symbolTable[order] = tableEntry(line, 'L', memOrder);
							flags[order++] = goLine;
							SML[memOrder++] = BRANCHZERO * 1000 + lineAdr;
						}
						else
							if (strcmp(symb, "!=") == 0)
							{
								SML[memOrder++] = LOAD * 1000 + varAdr;
								SML[memOrder++] = SUBSTRACT * 1000 + varAdr2;
								SML[memOrder++] = BRANCHNEG * 1000;
								SML[memOrder++] = LOAD * 1000 + varAdr2;
								SML[memOrder++] = SUBSTRACT * 1000 + varAdr;
								symbolTable[order] = tableEntry(line, 'L', memOrder);
								flags[order++] = goLine;
								SML[memOrder++] = BRANCHNEG * 1000;
							}
	}
}

//Цикл for
void Comp::_for()
{
	symbolTable[order++] = tableEntry(order, 'L', memOrder);
	char varName = strtok(NULL, " ")[0];
	if (strcmp(strtok(NULL, " "), "=") != 0)
		throw exception("Expected '='");
	int varTab = checkChar(varName, symbolTable, 'V', order);
	int varAdr;
	if (varTab == -1)
	{
		varAdr = varOrder;
		symbolTable[order] = tableEntry(varName, 'V', varOrder--);
	}
	else
		varAdr = symbolTable[varTab].getLoc();
	char* val1 = strtok(NULL, " ");
	int val1Tab;
	int val1Adr;
	if (val1[0] >= 'a' && val1[0] <= 'z')
	{
		val1Tab = checkChar(val1[0], symbolTable, 'V', order);
		if (val1Tab == -1)
		{
			val1Adr = varOrder;
			symbolTable[order] = tableEntry(val1[0], 'V', varOrder--);
		}
		else
			val1Adr = symbolTable[val1Tab].getLoc();
		SML[varAdr] = SML[val1Adr];
	}
	else
		if (val1[0] >= '0' && val1[0] <= '9' || val1[0] == '-')
			SML[varAdr] = atoi(val1);
		else
			throw exception("Unrecognized value");

	if (strcmp(strtok(NULL, " "), "to") != 0)
		throw exception("Expected 'to'");

	char* val2 = strtok(NULL, " ");
	int val2Adr;
	int val2Tab;
	if (val2[0] >= 'a' && val2[0] <= 'z')
	{
		val2Tab = checkChar(val2[0], symbolTable, 'V', order);
		if (val2Tab == -1)
		{
			val2Adr = varOrder;
			symbolTable[order] = tableEntry(val2[0], 'V', varOrder--);
		}
		else
			val2Adr = symbolTable[val2Tab].getLoc();
	}
	else
		if (val2[0] >= '0' && val2[0] <= '9' || val2[0] == '-')
		{
			val2Adr = varOrder;
			SML[varOrder--] = atoi(val2);
		}
		else
			throw exception("Unrecognized value");


	char* stepOrDo = strtok(NULL, " ");
	int stepAdr;
	int stepTab;
	if (strcmp(stepOrDo, "do") != 0)
		if (isdigit(stepOrDo[0]) || stepOrDo[0] == '-')
			SML[varOrder--] = atoi(stepOrDo);
		else
			if (islower(stepOrDo[0]))
			{			
				stepTab = checkChar(stepOrDo[0], symbolTable, 'V', order);
				if (stepTab == -1)
				{
					stepAdr = varOrder;
					symbolTable[order] = tableEntry(stepOrDo[0], 'V', varOrder--);
				}
				else
					stepAdr = symbolTable[stepTab].getLoc();
			}
			else
				throw exception("Unrecognized value");
	else
	{
		stepAdr = varOrder;
		SML[varOrder--] = 1;
	}
	loop l;
	l.varAdr = varAdr;
	l.val2Adr = val2Adr;
	l.stepAdr = stepAdr;
	l.varName = varName;
	l.branchAdr = memOrder; //
	loops.push(l);
	SML[memOrder++] = LOAD * 1000 + val2Adr;
	SML[memOrder++] = SUBSTRACT * 1000 + varAdr;
	SML[memOrder++] = BRANCHNEG * 1000;
}

void Comp::next()
{
	symbolTable[order++] = tableEntry(order, 'L', memOrder);
	char varGo = strtok(NULL, " ")[0];
	loop go;
	loops.pop(go);
	if (varGo != go.varName)
		throw exception("Unexpected loop iterator");
	
	SML[memOrder++] = LOAD * 1000 + go.varAdr;
	SML[memOrder++] = ADD * 1000 + go.stepAdr;
	SML[memOrder++] = STORE * 1000 + go.varAdr;
	SML[memOrder++] = BRANCH * 1000 + go.branchAdr;
	SML[go.branchAdr + 2] += memOrder;
}
void Comp::end()
{
	symbolTable[order++] = tableEntry(order, 'L', memOrder);
	SML[memOrder] = HALT * 1000;
}

