// ConsoleApplication34.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;


int main(int argc, char* argv[])
{
	const int READ = 10,
		WRITE = 11,
		LOAD = 20,
		STORE = 21,
		ADD = 30,
		SUBSTRACT = 31,
		DIVIDE = 32,
		MULTIPLY = 33,
		BRANCH = 40,
		BRANCHNEG = 41,
		BRANCHZERO = 42,
		HALT = 43;
	const int N = 100;
	long memory[N] = { 0 };
	long accumulator = 0,
		instructionRegister = 0,
		instructionCounter = 0,
		operationCode = 0,
		operand = 0;
	setlocale(LC_ALL, "Rus");
	cout << "*** ���������� ������������ ���! ***" << endl << endl;
		/*<< "*** ����������, ������� ���� ���������, �� ����� �������  ***" << endl
		<< "*** (��� ����� ������) �� ���. � ���� �������� � �������� ***" << endl
		<< "*** ��������� ������� ����� � ���� ������� (?). ��������� ***" << endl
		<< "*** ���� ����� ����� ��������� �� ��������� ������. ���   ***" << endl
		<< "*** ����������� ����� ��������� ������� ����� -99999      ***" << endl << endl
		<< "������ ������:"<< endl << "READ = 10" << endl << "WRITE = 11" << endl <<
		"LOAD = 20" << endl << "STORE = 21" << endl << "ADD = 30" << endl
		<< "SUBSTRACT = 31" << endl << "DIVIDE = 32" << endl
		<< "MULTIPLY = 33" << endl << "BRANCH = 40" << endl << "BRANCHNEG = 41" << endl
		<< "BRANCHZERO = 42" << endl << "HALT = 43" << endl << endl;
	cout << "������ �������: " << " +1007     - ���������� ����� � ������ 07" << endl << endl;*/
	if (argc == 1)
	{
		cerr << "Not enough arguments" << endl;
		return 1;
	}
	else
	{
		ifstream in(argv[1]);
		for (int i = 0; i < N; i++)
			do
			{
				in.read((char*)&memory[i], sizeof memory[i]);
				if (memory[i] == -99999)
				{
					i = N;
					break;
				}
			} while (memory[i] > 9999 || memory[i] < -9999);
			cout << "*** �������� ��������� ��������� ***" << endl
				<< "*** ������� ���������� ��������� ***" << endl;
			for (; instructionCounter < N; ++instructionCounter)
			{
				bool endIt = false;
				instructionRegister = memory[instructionCounter];
				operationCode = instructionRegister / 100;
				operand = instructionRegister % 100;
				switch (operationCode)
				{
				case 0:
					break;
				case READ:
					cout << setfill('0') << setw(2) << operand << " -> ";
					cin >> memory[operand];
					break;
				case WRITE:
					cout << memory[operand] << endl;
					break;
				case LOAD:
					accumulator = memory[operand];
					break;
				case STORE:
					memory[operand] = accumulator;
					break;
				case ADD:
					accumulator += memory[operand];
					break;
				case SUBSTRACT:
					accumulator -= memory[operand];
					break;
				case DIVIDE:
					if (memory[operand] == 0)
					{
						cout << "*** ������� ������� �� 0 ***" << endl
							<< "*** ���������� �������� �������� ���������� ��������� ***" << endl;
						operationCode = HALT;
						continue;
					}
					accumulator /= memory[operand];
					break;
				case MULTIPLY:
					accumulator *= memory[operand];
					break;
				case BRANCH:
					instructionCounter = operand - 1;
					break;
				case BRANCHNEG:
					if (accumulator < 0)
						instructionCounter = operand - 1;
					break;
				case BRANCHZERO:
					if (accumulator == 0)
						instructionCounter = operand - 1;
					break;
				case HALT:
					endIt = true;
					break;
				default:
					if (accumulator > 9999 || accumulator < -9999)
						cout << "*** ������������ ������������ ***" << endl
						<< "*** ���������� �������� �������� ���������� ��������� ***" << endl;
					else
					{
						cout << "*** �������� ��� �������� ***" << endl
							<< "*** ���������� �������� �������� ���������� ��������� ***" << endl;
					}
					endIt = true;
					break;
				}
				if (endIt)
					break;
			}
			cout.fill(' ');
			cout << "��������:" << endl;
			cout << setw(30) << setiosflags(ios::left) << "accumulator"
				<< setw(5) << setiosflags(ios::right) << accumulator << endl;
			cout << setw(30) << resetiosflags(ios::right) << setiosflags(ios::left) << "instructionCounter"
				<< setw(5) << setiosflags(ios::right) << instructionCounter << endl;
			cout << setw(30) << resetiosflags(ios::right) << setiosflags(ios::left) << "instructionRegister"
				<< setw(5) << setiosflags(ios::right) << instructionRegister << endl;
			cout << setw(30) << resetiosflags(ios::right) << setiosflags(ios::left) << "operationCode"
				<< setw(5) << setiosflags(ios::right) << operationCode << endl;
			cout << setw(30) << resetiosflags(ios::right) << setiosflags(ios::left) << "operand"
				<< setw(5) << setiosflags(ios::right) << operand << endl;
			cout << endl << "������:" << endl;
			cout << setw(3) << "";
			for (int i = 0; i < N / 10; i++)
				cout << setw(6) << i;
			cout << endl;
			for (int i = 0; i < N / 10; i++)
			{
				cout << setfill(' ') << setw(3) << i * 10;
				for (int j = 0; j < N / 10; j++)
				{
					cout << " " << ((memory[i*j] >= 0) ? '+' : '-');
					cout << setfill('0') << setw(4) << abs(memory[i*j]);
				}
				cout << endl;
			}
	}
	system("pause");
	return 0;
}

