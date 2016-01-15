// ConsoleApplication34.cpp: определяет точку входа для консольного приложения.
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
	cout << "*** Симплетрон приветствует вас! ***" << endl << endl;
		/*<< "*** Пожалуйста, введите вашу программу, по одной команде  ***" << endl
		<< "*** (или слову данных) за раз. Я буду выводить в качестве ***" << endl
		<< "*** подсказки текущий адрес и знак вопроса (?). Введенное ***" << endl
		<< "*** вами слово будет размещено по укзанному адресу. Для   ***" << endl
		<< "*** прекращения ввода программы введите число -99999      ***" << endl << endl
		<< "СПИСОК КОМАНД:"<< endl << "READ = 10" << endl << "WRITE = 11" << endl <<
		"LOAD = 20" << endl << "STORE = 21" << endl << "ADD = 30" << endl
		<< "SUBSTRACT = 31" << endl << "DIVIDE = 32" << endl
		<< "MULTIPLY = 33" << endl << "BRANCH = 40" << endl << "BRANCHNEG = 41" << endl
		<< "BRANCHZERO = 42" << endl << "HALT = 43" << endl << endl;
	cout << "ПРИМЕР КОМАНДЫ: " << " +1007     - считывание слова в ячейку 07" << endl << endl;*/
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
			cout << "*** Загрузка программы завершена ***" << endl
				<< "*** Начинаю выполнение программы ***" << endl;
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
						cout << "*** Попытка деления на 0 ***" << endl
							<< "*** Симплетрон аварийно завершил выполнение программы ***" << endl;
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
						cout << "*** Переполнение аккумулятора ***" << endl
						<< "*** Симплетрон аварийно завершил выполнение программы ***" << endl;
					else
					{
						cout << "*** Неверный код операции ***" << endl
							<< "*** Симплетрон аварийно завершил выполнение программы ***" << endl;
					}
					endIt = true;
					break;
				}
				if (endIt)
					break;
			}
			cout.fill(' ');
			cout << "РЕГИСТРЫ:" << endl;
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
			cout << endl << "ПАМЯТЬ:" << endl;
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

