
#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <conio.h>

using namespace std;

enum Cell
{
	CROSS = 'X',
	ZERO = '0',
	EMPTY = '_'
};

enum Progress
{
	IN_PROGRESS,
	WIN_HUMAN,
	WIN_AI,
	DRAW
};


int32_t getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

struct Game
{
	Cell** ppField = nullptr;
	const size_t SIZE = 3;
	Progress progress = IN_PROGRESS;
	size_t turn;
	Cell human, ai;
};

struct Coord
{
	size_t x = 0;
	size_t y = 0;

};

void clsScr()
{
	system("cls");
}


void initGame(Game& g)
{
	g.ppField = new Cell * [g.SIZE];

	for (size_t y = 0; y < g.SIZE; y++)
		g.ppField[y] = new Cell[g.SIZE];

	for (size_t y = 0; y < g.SIZE; y++)
		for (size_t x = 0; x < g.SIZE; x++)
			g.ppField[y][x] = EMPTY;

	if (getRandomNum(0, 1000) > 500)
	{
		g.human = CROSS;
		g.ai = ZERO;
		g.turn = 0;
	}
	else
	{
		g.human = ZERO;
		g.ai = CROSS;
		g.turn = 1;
	}
}


void deinitGame(Game& g)
{
	for (size_t y = 0; y < g.SIZE; y++)
		delete [] g.ppField[y];
	delete [] g.ppField;

}

void draw(const Game& g)
{
	cout << "    ";
	for (size_t x = 0; x < g.SIZE; x++)
		cout << "  " << x + 1 << "   ";
	cout << "x";
	cout << endl;

	for (size_t y = 0; y < g.SIZE; y++)
	{
		cout << " " << y+1  << " |  ";
		for (size_t x = 0; x < g.SIZE; x++)
			cout << (char)g.ppField[y][x] << "  |  ";
		cout << endl;
	}


	cout << endl << "Human: " << (char)g.human << endl;
	cout << "AI: " << (char)g.ai << endl;
}

Coord getHumanCoord(Game& g)
{
	Coord c;

	do
	{
		cout << "Coord x: ";
		cin >> c.x;
		cout << "Coord y: ";
		cin >> c.y;

	} 	while (c.x > 3 || c.y > 3 || c.x == 0 || c.y == 0 || g.ppField[c.y-1][c.x-1] != EMPTY);

	c.x--;
	c.y--;

	return c;
}

Progress isWon(const Game& g)
{
	for (size_t y = 0; y < g.SIZE; y++)
	{
		if (g.ppField[y][0] == g.ppField[y][1] && g.ppField[y][0] == g.ppField[y][2])
		{
			if (g.ppField[y][0] == g.human)
				return WIN_HUMAN;

			if (g.ppField[y][0] == g.ai)
				return WIN_AI;
		}
	}

	for (size_t x = 0; x < g.SIZE; x++)
	{
		if (g.ppField[0][x] == g.ppField[1][x] && g.ppField[0][x] == g.ppField[2][x])
		{
			if (g.ppField[0][x] == g.human)
				return WIN_HUMAN;

			if (g.ppField[0][x] == g.ai)
				return WIN_AI;
		}
	}

	if (g.ppField[0][0] == g.ppField[1][1] && g.ppField[0][0] == g.ppField[2][2])
	{
		if (g.ppField[0][0] == g.human)
			return WIN_HUMAN;

		if (g.ppField[0][0] == g.ai)
			return WIN_AI;
	}

	if (g.ppField[0][2] == g.ppField[1][1] && g.ppField[0][2] == g.ppField[2][0])
	{
		if (g.ppField[1][1] == g.human)
			return WIN_HUMAN;

		if (g.ppField[1][1] == g.ai)
			return WIN_AI;
	}

	bool isDraw = true;
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				isDraw = false;
			}
		}
	}

	if (isDraw)
		return DRAW;
}


Coord getAICoord(Game& g)
{
	// PRE WIN 
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.ai;
				if (isWon(g) == WIN_AI)
				{
					g.ppField[y][x] = EMPTY;
					return { x, y };
				}
				// PRE FAIL
				g.ppField[y][x] = g.human;
				if (isWon(g) == WIN_HUMAN)
				{
					g.ppField[y][x] = EMPTY;
					return { x, y };
				}

				g.ppField[y][x] = EMPTY;
			}
		}
	}

	if (g.ppField[1][1] == EMPTY)
		return { 1, 1 };

	Coord arr[4];
	size_t num = 0;
	if (g.ppField[0][0] == EMPTY)
	{
		arr[num++] = { 0, 0 };
	}
	if (g.ppField[2][2] == EMPTY)
	{
		arr[num++] = { 2, 2 };
	}
	if (g.ppField[2][0] == EMPTY)
	{
		arr[num++] = { 0, 2 };
	}
	if (g.ppField[0][2] == EMPTY)
	{
		arr[num++] = { 2, 0 };
	}
	if (num > 0)
	{
		return arr[getRandomNum(0, 1000) % num];
	}


	num = 0;
	if (g.ppField[0][1] == EMPTY)
	{
		arr[num++] = { 1, 0 };
	}
	if (g.ppField[2][1] == EMPTY)
	{
		arr[num++] = { 1, 2 };
	}
	if (g.ppField[1][0] == EMPTY)
	{
		arr[num++] = { 0, 1 };
	}
	if (g.ppField[1][2] == EMPTY)
	{
		arr[num++] = { 2, 1 };
	}
	if (num > 0)
	{
		return arr[getRandomNum(0, 1000) % num];
	}

}


int main()
{
	Game g;
	initGame(g);
	clsScr();
	draw(g);

	do {
		if (g.turn % 2 == 0)
		{
			Coord c = getHumanCoord(g);
			g.ppField[c.y][c.x] = g.human;
		}
		else
		{
			Coord c = getAICoord(g);
			g.ppField[c.y][c.x] = g.ai;
		}

		clsScr();
		draw(g);

		g.turn++;
		g.progress = isWon(g);

	} 	while (g.progress == IN_PROGRESS);

	if (g.progress == WIN_HUMAN)
		cout << "You win smart Human! Grats! " << endl;
	else if (g.progress == WIN_AI)
		cout << "You lose! Win Smart AI " << endl;
	else
		cout << "It is Draw, Try Again! :) " << endl;

	deinitGame(g);
	_getch();
	return 0;
}
