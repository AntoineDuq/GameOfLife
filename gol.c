#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define KEY_SPACE ' '
#define DELAY 5000
#define NO 0
#define YES 1
#define SIZEMAX 300

/* Structures */

typedef struct cell{
	int alive;
}Cell;

/* Variables globales */

int max_x, max_y;
int x,y;

typedef struct game{
	Cell cells[SIZEMAX][SIZEMAX];
	int size_x;
	int size_y;
}Game;

Game game_current;
Game game_next;

/* Fonctions */

void cursor_move(int ch)
{
	switch(ch)
	{

		case KEY_UP:
			if(y > 0)
			{
				move(y-1,x);
				y--;
			}
			break;
		case KEY_DOWN:
			if(y < SIZEMAX-1 && y < game_current.size_y-1)
			{
				move(y+1,x);
				y++;
			}
			break;
		case KEY_RIGHT:
			if(x < SIZEMAX-1 && x < game_current.size_x-1)
			{
				move(y,x+1);
				x++;
			}
			break;
		case KEY_LEFT:
			if(x > 0)
			{
				move(y,x-1);
				x--;
			}
			break;
		default:
			break;
	}

}

void cell_select(void)
{
	game_current.cells[y][x].alive = YES;
}

void cell_unselect(void)
{
	if(game_current.cells[y][x].alive == YES)
	{
		game_current.cells[y][x].alive = NO;
	}
}

void size_update(void)
{
	if(max_x > SIZEMAX)
	{
		game_current.size_x = SIZEMAX;
		game_next.size_x = SIZEMAX;
	}
	else
	{
		game_current.size_x = max_x;
		game_next.size_x = max_x;
	}
	if(max_y > SIZEMAX)
	{
		game_current.size_y = SIZEMAX;
		game_next.size_y = SIZEMAX;
	}
	else
	{	
		game_current.size_y = max_y;
		game_next.size_y = max_y;
	}
}

void game_init(void)
{
	int i,j;
	size_update();
	
	for (i=0;i<SIZEMAX;i++)
	{
		for(j=0;j<SIZEMAX;j++)
		{
			game_current.cells[i][j].alive = NO;
			game_next.cells[i][j].alive = NO;
		}
	}
}

void cells_disp(void)
{
	int i,j;
	for (i=0;i<game_current.size_y;i++)
	{
		for(j=0;j<game_current.size_x;j++)
		{
			if(game_current.cells[i][j].alive == YES )
			{
				mvaddch(i,j,'o');
			}
		}
	}
}

int cells_around(int i, int j)
{
	int c = 0;
	if(i > 0 && game_current.cells[i-1][j].alive == YES){c++;}
	if(i < game_current.size_y-1 && game_current.cells[i+1][j].alive == YES){c++;}
	if(j > 0 && game_current.cells[i][j-1].alive == YES){c++;}
	if(j < game_current.size_x-1 && game_current.cells[i][j+1].alive == YES){c++;}
	
	if(i < game_current.size_y-1 && j < game_current.size_x-1 && game_current.cells[i+1][j+1].alive == YES){c++;}
	if(i > 0 && j > 0 && game_current.cells[i-1][j-1].alive == YES){c++;}
	if(i < game_current.size_y-1 && j > 0 && game_current.cells[i+1][j-1].alive == YES){c++;}
	if(i > 0 && j < game_current.size_x-1 && game_current.cells[i-1][j+1].alive == YES){c++;}
	
	return c;
}

void next_turn(void)
{
	int i,j,c;
	for (i=0;i<game_current.size_y;i++)
	{
		for(j=0;j<game_current.size_x;j++)
		{
			c = cells_around(i,j);
			if(game_current.cells[i][j].alive == YES)
			{
				if((c == 2) || (c == 3))
				{
					game_next.cells[i][j].alive = YES;
				}
				else
				{
					game_next.cells[i][j].alive = NO;
				}
			}
			
			else if(game_current.cells[i][j].alive == NO)
			{
				if(c == 3)
				{
					game_next.cells[i][j].alive = YES;
				}
				else
				{
					game_next.cells[i][j].alive = NO;
				}
			}
		}
	}
}

void cells_copy(void)
{
	int i,j;
	for (i=0;i<SIZEMAX;i++)
	{
		for(j=0;j<SIZEMAX;j++)
		{
			game_current.cells[i][j].alive = game_next.cells[i][j].alive;
		}
	}
}


/* Programme principale */


int main(int argc, char *argv[]) 
{
	if(argc != 3){printf("Usage ./gol <interval_µs> <iterations>\n");return 0;}
	int max_ite = atoi(argv[2]);
	int speed = atoi(argv[1]);
	if(speed == 0){speed = 1000000;}
	int ch;
	initscr();
	cbreak();
	curs_set(TRUE);
	keypad(stdscr, TRUE);
	erase();
	x = 0;
	y = 0;
	getmaxyx(stdscr, max_y, max_x);
	game_init();

	while(1)
	{
		getmaxyx(stdscr, max_y, max_x);
		size_update();
		move(y,x);
		
		ch = getch();
		if(ch == KEY_SPACE)
		{
			cell_select();
		}
		else if(ch == KEY_BACKSPACE)
		{
			cell_unselect();
		}
		else if(ch == 10)
		{
			break;
		}
		else
		{
			cursor_move(ch);
		}
		
		clear();
		cells_disp();
		
		refresh();	
		usleep(DELAY);
	}

	clear();
	refresh();
	curs_set(FALSE);
	cells_disp();
	int ite = 1;
	char ite_ch[8];

	while(1)
	{
		ch = getch();
		if(ch = 10)
		{
			while(1)		
			{
				getmaxyx(stdscr, max_y, max_x);
				size_update();
				next_turn();
				cells_copy();
				clear();
				sprintf(ite_ch, "%d", ite);
				mvprintw(0, 0, ite_ch);
				cells_disp();
				refresh();
				usleep(speed);
				if(ite == max_ite){break;}
				ite++;
			}
			if(ite == max_ite){break;}
		}
	}
	
	mvprintw(game_current.size_y-1, 0, "Press any key to quit !");
	getch();
	
	endwin();
	printf("Speed : %d\n",speed);
}
