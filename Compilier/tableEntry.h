#pragma once
const int N = 500;
class tableEntry
{
public:
	tableEntry(char s = 0, char t = 0, int l = 0);
	~tableEntry();
	char getSymbol() const
	{
		return symbol;
	}

	char getType() const
	{
		return type;
	}

	int getLoc() const
	{
		return location;
	}
private:
	char symbol;
	char type;
	int location;
};


