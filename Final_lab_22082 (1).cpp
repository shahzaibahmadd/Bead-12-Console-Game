#include <iostream>

#include <iomanip>
#include<conio.h>
#include <fstream>
#include<Windows.h>


using namespace std;
struct position
{
	int ri;
	int ci;
};
enum { GREEN, YELLOW };

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
void init(char**& b, int& dim, string pnames[], int& turn)
{
	ifstream rdr("hello.txt");
	rdr >> dim;

	cout << "player 1 name" << endl;
	cin >> pnames[0];
	cout << "player 2 name" << endl;
	cin >> pnames[1];


	b = new char* [dim];
	for (int ri = 0; ri < dim; ri++)
	{
		b[ri] = new char[dim];
	}
	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{

			rdr >> b[ri][ci];
		}
	}
	turn = 0;
}
void squarefill(int sr, int sc, int dr, int dc, int color, char sym)
{
	SetClr(color);
	for (int i = sr; i <= dr; i++)
	{
		for (int j = sc; j <= dc; j++)
		{
			gotoRowCol(i, j);
			cout << sym;

		}
	}
}

void square(int sr, int sc, int dr, int dc, int color, char sym)
{
	SetClr(color);
	for (int i = sr; i <= dr; i++)
	{
		for (int j = sc; j <= dc; j++)
		{
			gotoRowCol(i, sc);
			cout << sym;
			gotoRowCol(i, dc);
			cout << sym;
			gotoRowCol(sr, j);
			cout << sym;
			gotoRowCol(dr, j);
			cout << sym;
		}
	}
}
void xplus(int sr, int sc, int dr, int dc, int color, char sym)
{
	SetClr(color);
	for (int i = sr; i < dr; i++)
	{
		for (int j = sc; j < dc; j++)
		{
			gotoRowCol(i, (sc + dc) / 2);
			cout << sym;
			gotoRowCol((sr + dr) / 2, j);
			cout << sym;
		}
	}
}
void cross(int sr, int sc, int dr, int dc, int color, char sym)
{
	SetClr(color);
	for (int i = sr; i <= dr; i++)
	{
		for (int s = sc; s <= dc; s++)
		{
			if (dr - i == dc - s)
			{
				gotoRowCol(i, s);
				cout << sym;

			}
		}

	}
	int x = dc;
	for (int i = sr; i <= dr; i++)
	{
		gotoRowCol(i, x);
		cout << sym;
		x--;
	}

}

void selectposition(position& p, int dim)
{

	int a, b;

	getRowColbyLeftClick(p.ri, p.ci);
	a = p.ri;
	b = p.ci;
	int A = a / dim;
	int B = b / dim;
	p.ri = A;
	p.ci = B;
	
}
bool isgreenpiece(char sym)
{
	
	return  sym == '@';
	 
}
bool isyellowpiece(char sym)
{
	return sym == '$';
	
}
bool ismypiece(char sym, int& turn)
{

	if (turn == GREEN)
	{
		return isgreenpiece(sym);
	}
	if (turn == YELLOW)
	{
		return isyellowpiece(sym);
	}
	return false;

}

bool isvalidsrc(char**& b, int& dim, position sc, int& turn)
{
	if (sc.ri < 0 || sc.ri >= dim || sc.ci < 0 || sc.ci >= dim)
	{		return false;
}

	return ismypiece(b[sc.ri][sc.ci], turn);


}
bool isvaliddst(char**& b, int& dim, position dc, int& turn)
{
	if (dc.ri < 0 || dc.ri >= dim || dc.ci < 0 || dc.ci >= dim)
		return false;
	if (ismypiece(b[dc.ri][dc.ci], turn) == true)
	{
		return false;
	}
	return true;


}
void turnf(int& turn)
{
	turn = (turn + 1) % 2;
	
}
void  turnmsg(string pnames)
{
	cout << pnames << "'s Turn.." << endl;
}
void updateboard(char**& b, position sc, position dc)
{
	
	/*char c = b[sc.ri][sc.ci];
	b[dc.ri][dc.ci] = c;
	b[sc.ri][sc.ci] = '-';*/
	swap(b[dc.ri][dc.ci], b[sc.ri][sc.ci]);
	

}
bool isHoriMove(position Sc, position Dc)
{
	if (Sc.ri == Dc.ri)
		return true;
	return false;
}
bool isDiaMove(position Sc, position Dc)
{
	if (abs(Sc.ri - Dc.ri) == abs(Sc.ci - Dc.ci))
		return true;
	return false;
}
bool isVertMove(position Sc, position Dc)
{
	if (Sc.ci == Dc.ci)
		return true;
	return false;

}
bool legality(char**& B, position P, int& Turn)
{
	if (Turn == GREEN)
	{
		if (B[P.ri][P.ci] == '@')
		{
			return true;
		}
		return false;
	}
	if (Turn == YELLOW)
	{
		if (B[P.ri][P.ci] == '$')
		{
			return true;
		}
		return false;
	}
}

bool isLegalMove(char**& B, position Sc, position Dc, int& Turn)
{
	position P;
	bool r = true;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			P.ri = i;
			P.ci = j;
			if ((isDiaMove(Sc, P) || isHoriMove(Sc, P) || isVertMove(Sc, P)) && (abs(Sc.ri - P.ri) == 1 || abs(Sc.ci - P.ci) == 1) && legality(B, P, Turn))
				r = false;
		}
	}
	if (r == true)
	{
		if ((isDiaMove(Sc, Dc) || isHoriMove(Sc, Dc) || isVertMove(Sc, Dc)) && (abs(Sc.ri - Dc.ri) == 1 || abs(Sc.ci - Dc.ci) == 1) && B[Dc.ri][Dc.ci] == '-')
			return true;
		return false;
	}
	if (r == false)
	{
		if ((isDiaMove(Sc, Dc) || isHoriMove(Sc, Dc) || isVertMove(Sc, Dc)) && (abs(Sc.ri - Dc.ri) == 2 || abs(Sc.ci - Dc.ci) == 2) && B[Dc.ri][Dc.ci] == '-' || ((abs(Sc.ri - Dc.ri) ==1 || abs(Sc.ci - Dc.ci) == 1) && B[Dc.ri][Dc.ci] == '-'))
		{
			if (isDiaMove(Sc, Dc) && (abs(Sc.ri - Dc.ri) == 2 || abs(Sc.ci - Dc.ci) == 2))
			{
				if (Sc.ri - Dc.ri > 0 && Sc.ci - Dc.ci > 0)
				{
					B[Dc.ri + 1][Dc.ci + 1] = '-';
				}
				if (Sc.ri - Dc.ri > 0 && Sc.ci - Dc.ci < 0)
				{
					B[Dc.ri + 1][Dc.ci - 1] = '-';
				}
				if (Sc.ri - Dc.ri < 0 && Sc.ci - Dc.ci>0)
				{
					B[Dc.ri - 1][Dc.ci + 1] = '-';
				}
				if (Sc.ri - Dc.ri < 0 && Sc.ci - Dc.ci < 0)
				{
					B[Dc.ri - 1][Dc.ci - 1] = '-';
				}
			}
			if (isHoriMove(Sc, Dc) && (abs(Sc.ri - Dc.ri) == 2 || abs(Sc.ci - Dc.ci) == 2))
			{
				if (Sc.ci - Dc.ci < 0)
				{
					B[Dc.ri][Dc.ci - 1] = '-';
				}
				if (Sc.ci - Dc.ci > 0)
				{
					B[Dc.ri][Dc.ci + 1] = '-';
				}
			}
			if (isVertMove(Sc, Dc) && (abs(Sc.ri - Dc.ri) == 2 || abs(Sc.ci - Dc.ci) == 2))
			{
				if (Sc.ri - Dc.ri < 0)
				{
					B[Dc.ri - 1][Dc.ci] = '-';
				}
				if (Sc.ri - Dc.ri > 0)
				{
					B[Dc.ri + 1][Dc.ci] = '-';
				}
			}
			return true;
		}
		return false;
	}
}
bool kill(char**& B)
{



	return true;
}
bool WinCheck(char** B, int& bc, int& rc)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (B[i][j] == '@')
				rc++;
			if (B[i][j] == '$')
				bc++;
		}
	}
	if (bc == 0 || rc == 0)
		return true;
	return false;
}
void printboard(char**& b, int& dim)
{
	int r = 0;

	for (int ri = 0; ri < dim; ri++)
	{
		int c = 0;
		for (int ci = 0; ci < dim; ci++)
		{

			gotoRowCol(r, c);
			cout << b[ri][ci];

			c += 7;
		}
		r += 7;
		cout << endl;

	}


}
void board()
{
	squarefill(0, 0, 14, 14, 9, -37);
	squarefill(0, 14, 14, 28, 9, -37);
	squarefill(14, 0, 28, 14, 9, -37);
	squarefill(14, 14, 28, 28, 9, -37);

	square(0, 0, 14, 14, 12, -37);
	square(0, 14, 14, 28, 12, -37);
	square(14, 0, 28, 14, 12, -37);
	square(14, 14, 28, 28, 12, -37);

	xplus(0, 0, 14, 14, 12, -37);
	xplus(0, 14, 14, 28, 12, -37);
	xplus(14, 0, 28, 14, 12, -37);
	xplus(14, 14, 28, 28, 12, -37);

	cross(0, 0, 14, 14, 6, '*');
	cross(0, 14, 14, 28, 6, '*');
	cross(14, 0, 28, 14, 6, '*');
	cross(14, 14, 28, 28, 6, '*');

}

int main()
{
	char** b;
	int dim;
	string pnames[2];
	int turn;

	position sp, ds;


	init(b, dim, pnames, turn);

	board();
	printboard(b, dim);
	while (true)
	{
		
		turnmsg(pnames[turn]);
		do
		{
			cout << "Select from" << endl;
			selectposition(sp, dim);
			
		} while (!isvalidsrc(b, dim, sp, turn));
		do
		{
			cout << "Select to" << endl;
			selectposition(ds, dim);
			isLegalMove(b, sp,ds, turn);
		} while (!isvaliddst(b, dim, ds, turn));
		updateboard(b, sp, ds);
		board();
		printboard(b, dim);
		turnf(turn);

	}




	return 0;
}
