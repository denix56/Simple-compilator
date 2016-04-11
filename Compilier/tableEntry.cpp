#include "tableEntry.h"



tableEntry::tableEntry(char s, char t, int l)
{
	if (l < 0 || l>499 || t!='C' && t!='L' && t!='V' && t!=0)
		throw 1;
	else
	{
		symbol = s;
		type = t;
		location = l;
	}

}


tableEntry::~tableEntry()
{
}
