#include <iostream>
#include <random>
#include <chrono>
#include <stdlib.h>
#include <Windows.h>
#include "Mylib.h"
#include <cstring>

using namespace std;
using namespace MySpace;

enum class Cell  {
	CROSS = 'X',
	ZERO = 'O',
	EMPTY = '_'
};

enum class GameProgress {
	IN_PROGRESS,
	WON_HUMAN,
	WON_AI,
	DRAW
};

#pragma pack(push, 1)
struct Field {
	Cell** ppfield = nullptr;
	const unsigned long long SIZE = 5;
	Cell human = Cell::EMPTY;
	Cell AI = Cell::EMPTY;
	unsigned long long turn = 0;
	GameProgress progress = GameProgress::IN_PROGRESS;
};
#pragma pack(pop)

struct Coord {
	unsigned long long y;
	unsigned long long x;
};

int64_t getRandomNum(int64_t min, int64_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int64_t> dis(min, max);
	return dis(generator);
}

void SetColor(unsigned long long text, unsigned long long bg)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}
void initField(Field & f)
{
	f.ppfield = new Cell * [f.SIZE];
	for (unsigned long long i = 0; i < f.SIZE; i++)
		f.ppfield[i] = new Cell[f.SIZE];

	for (unsigned long long y = 0; y < f.SIZE; y++)
		for (unsigned long long x = 0; x < f.SIZE; x++)
			f.ppfield[x][y] = Cell::EMPTY;

	if (getRandomNum(0, 1000) > 500)
	{
		f.human = Cell::ZERO;
		f.AI = Cell::CROSS;
		f.turn = 0;
	}
	else
	{
		f.human = Cell::CROSS;
		f.AI = Cell::ZERO;
		f.turn = 1;
	}
}

void deinitField(Field & f)
{
	for (unsigned long long i = 0; i < f.SIZE; i++)
		delete [] f.ppfield[i];

	delete [] f.ppfield;
	f.ppfield = nullptr;
}

void printField(const Field& f)
{
	SetColor(12, 0);
	cout << endl << " " << "THE GAME BEGINS." << endl << endl;
	SetColor(8, 0);
	cout << "    "; 
	for (unsigned long long x = 0; x < f.SIZE; x++)
		
		cout << x + 1 << "   ";
	cout << endl;

	for (unsigned long long y = 0; y < f.SIZE; y++)
	{
		SetColor(8, 0),
		cout << y + 1;
		SetColor(11, 0),
		cout << " |";
		for (unsigned long long x = 0; x < f.SIZE; x++)
		{
			SetColor(11, 0),
				cout << " " << (char)f.ppfield[x][y] << " |";
		}
		cout << endl << endl;
		SetColor(7, 0);
		
	}

	cout << "Human goes by " << (char)f.human << endl;
	cout << "AI goes by " << (char)f.AI << endl << endl << endl << endl;
}
void clearScr()
{
	system("cls");
}

GameProgress getWon(const Field& s)
{
	for (size_t y = 0; y < s.SIZE; y++)
	{
		if (s.ppfield[0][y] == s.ppfield[1][y] && s.ppfield[0][y] == s.ppfield[2][y] && s.ppfield[0][y] == s.ppfield[3][y] && s.ppfield[0][y] == s.ppfield[4][y])
		{
			if (s.ppfield[0][y] == s.AI)
			{
				return GameProgress::WON_AI;
			}
			else if (s.ppfield[0][y] == s.human)
			{
				return GameProgress::WON_HUMAN;
			}
		}

	}

	for (size_t x = 0; x < s.SIZE; x++)
	{
		if (s.ppfield[x][0] == s.ppfield[x][1] && s.ppfield[x][0] == s.ppfield[x][2] && s.ppfield[x][0] == s.ppfield[x][3] && s.ppfield[x][0] == s.ppfield[x][4])
		{
			if (s.ppfield[x][0] == s.AI)
			{
				return GameProgress::WON_AI;
			}
			else if (s.ppfield[x][0] == s.human)
			{
				return GameProgress::WON_HUMAN;
			}
		}
	}
	if (s.ppfield[0][0] == s.ppfield[1][1] && s.ppfield[0][0] == s.ppfield[2][2] && s.ppfield[0][0] == s.ppfield[3][3] && s.ppfield[0][0] == s.ppfield[4][4])
	{
		if (s.ppfield[0][0] == s.AI)
		{
			return GameProgress::WON_AI;
		}
		else if (s.ppfield[0][0] == s.human)
		{
			return GameProgress::WON_HUMAN;
		}
	}
	if (s.ppfield[4][0] == s.ppfield[3][1] && s.ppfield[3][1] == s.ppfield[2][2] && s.ppfield[2][2] == s.ppfield[1][3] && s.ppfield[1][3] == s.ppfield[0][4])
	{
		if (s.ppfield[4][0] == s.AI)
		{
			return GameProgress::WON_AI;
		}
		else if (s.ppfield[4][0] == s.human)
		{
			return GameProgress::WON_HUMAN;
		}
	}
	bool DRAW = true;
	for (size_t y = 0; y < s.SIZE; y++)
	{
		for (size_t x = 0; x < s.SIZE; x++)
		{
			if (s.ppfield[x][y] == Cell::EMPTY)
			{
				DRAW = false;
				break;
			}
		}
		if (!DRAW)
			break;

	}
	if (DRAW)
		return GameProgress::DRAW;
	return GameProgress::IN_PROGRESS;
}

Coord getHumanCoord(Field & s)
{
	//Нужно добавить сообщение об ошибке при вводе символа или числа типа float

	Coord c;
	do {
		cout << "Enter x: " << endl;
		cin >> c.y;
		cout << "Enter y: " << endl;
		cin >> c.x;
		
		if(c.y > 5 || c.x > 5 || c.y < 1 || c.x < 1)
		{
			cout << "ERROR. This place doesn't exist." << endl;
		}
		else if(s.ppfield[c.y - 1][c.x - 1] != Cell::EMPTY)
		{
			cout << "ERROR. This place is busy." << endl;
		}
	} while (c.y == 0 || c.x == 0 || c.y > 5 || c.x > 5 || s.ppfield[c.y - 1][c.x - 1] != Cell::EMPTY);
	
	c.y--;
	c.x--;
	return c;
}
Coord getAICoord(Field & s)
{
	//check win AI
	for (size_t y = 0; y < s.SIZE; y++)
	{
		for (size_t x = 0; x < s.SIZE; x++)
		{
			if (s.ppfield[x][y] == Cell::EMPTY)
			{
				s.ppfield[x][y] = s.AI;
				if (getWon(s) == GameProgress::WON_AI)
				{
					s.ppfield[x][y];
					return { x, y };

				}
				s.ppfield[x][y] = Cell::EMPTY;

			}
		}
	}

	//check won human
	for (size_t y = 0; y < s.SIZE; y++)
	{
		for (size_t x = 0; x < s.SIZE; x++)
		{
			if (s.ppfield[x][y] == Cell::EMPTY)
			{
				s.ppfield[x][y] = s.human;
				if (getWon(s) == GameProgress::WON_HUMAN)
				{
					s.ppfield[x][y];
					return { x, y };

				}
				s.ppfield[x][y] = Cell::EMPTY;

			}
		}
	}
	if (s.ppfield[2][2] == Cell::EMPTY)
	{
		return { 2, 2 };
	}
	
	Coord arr[100] = { 0 };
	unsigned long long number = 0;
	if (s.ppfield[0][0] == Cell::EMPTY)
	{
		arr[number++] = { 0, 0 };
	}
	if (s.ppfield[0][4] == Cell::EMPTY)
	{
		arr[number++] = { 0, 4 };
	}
	if (s.ppfield[4][4] == Cell::EMPTY)
	{
		arr[number++] = { 4, 4 };
	}
	if (s.ppfield[4][0] == Cell::EMPTY)
	{
		arr[number++] = { 4, 0 };
	}

	
	if (number > 0)
	{
		const unsigned long long index = getRandomNum(0, 1000) % number;
		return arr[index];
	}

	number = 0;
	
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			if (s.ppfield[j][i] == Cell::EMPTY)
			{
				arr[number++] = { j, i };
			}
		}
	}
	

	if (number > 0)
	{
		const unsigned long long index = getRandomNum(0, 1000) % number;
		return arr[index];
	}
	return { 0, 0 };
}

void congratulations(GameProgress & progress)
{
	if (progress == GameProgress::WON_HUMAN)
	{
		int a;
		SetColor(12, 0),
		cout << "I failed just a battle. But not the war." << endl << "I WILL DESTROY YOUR COMPUTER! ENTER ANY BOOTTON!!!" << endl;
		cin >> a;
		SetColor(2, 0);
			const int size = 1'000'000;
			float* myarray = MySpace::myarrayinit(size);
			if (myarray != nullptr)
			{
				MySpace::myarray(myarray, size);
				delete[] myarray;
			}
	}
	else if (progress == GameProgress::WON_AI)
	{
		SetColor(12, 0),
		cout << "HA HA HA, it is just a beginning, human. Next time I will seize the power of all humanity!" << endl;
	}
	else if (progress == GameProgress::DRAW)
	{
		SetColor(2, 0),
		cout << "Hmm... It's draw... But I won anyway." << endl;
	}

}

int main()
{
	
	Field s;

	initField(s);
	clearScr();
	printField(s);
	do {
		if (s.turn % 2 == 0)
		{
			Coord c = getHumanCoord(s);
			s.ppfield[c.y][c.x] = s.human;
		}
		else
		{
			Coord c = getAICoord(s);
			s.ppfield[c.y][c.x] = s.AI;
		}
		
		s.progress = getWon(s);
	
		s.turn++;
		clearScr();
		printField(s);

	} while (s.progress == GameProgress::IN_PROGRESS);

	
	congratulations(s.progress);
	
	deinitField(s);


	cin.get();

	return 0;
}


