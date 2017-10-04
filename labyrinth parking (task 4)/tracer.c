#include <stdio.h>

#define ORIENTATION_LEFT 101
#define ORIENTATION_UP 102
#define ORIENTATION_RIGHT 103
#define ORIENTATION_BOTTOM 104



#define ROWS 5
#define COLS 4


struct cell
{
	int haveUpperBorder;
	int haveBottomBorder;
	int haveLeftBorder;
	int haveRightBorder;

	int waveTraceFactor;
	int proceed;
};

struct cell field[ROWS][COLS];
int orientation = ORIENTATION_UP;

void setCellParameters(struct cell* c, const char* params)
{
	if (params == NULL)
	{
		c->haveBottomBorder = 0;
		c->haveLeftBorder = 0;
		c->haveRightBorder = 0;
		c->haveUpperBorder = 0;
		return;
	}

	//UBLR

	//upper border
	if (params[0] == 'U') c->haveUpperBorder = 1; else c->haveUpperBorder = 0;
	//bottom border
	if (params[1] == 'B') c->haveBottomBorder = 1; else c->haveBottomBorder = 0;
	//left border
	if (params[2] == 'L') c->haveLeftBorder = 1; else c->haveLeftBorder = 0;
	//right border
	if (params[3] == 'R') c->haveRightBorder = 1; else c->haveRightBorder = 0;
}

void setCellWavefactor(struct cell* c, int wavefactor, int procstatus)
{
	c->waveTraceFactor = wavefactor;
	c->proceed = procstatus;
}



//displays field in console
//TODO: rewrite for NXT's screen
void displayLabyrinth(int showfactor)
{
	for (int i = 0; i < ROWS; i++)
	{
		//str 1
		for (int j = 0; j < COLS; j++)
		{
			if (field[i][j].haveUpperBorder)
			{
				printf("###");
				continue;
			}

			if (field[i][j].haveLeftBorder)
				printf("# ");
			else
				printf("  ");

			if (field[i][j].haveRightBorder)
				printf("#");
			else
				printf(" ");
		}

		printf("\n");

		//str 2
		for (int j = 0; j < COLS; j++)
		{
			if (field[i][j].haveLeftBorder)
				printf("#");
			else
				printf(" ");

			if (showfactor == 0) printf(" ");
			if (showfactor == 1) printf("%d", field[i][j].waveTraceFactor);
			if (showfactor == 2) {
				if (field[i][j].proceed == 3)
					printf("+");
				else
					printf(" ");
			}



			if (field[i][j].haveRightBorder)
				printf("#");
			else
				printf(" ");
		}

		printf("\n");

		//str 3
		for (int j = 0; j < COLS; j++)
		{
			if (field[i][j].haveBottomBorder)
			{
				printf("###");
				continue;
			}

			if (field[i][j].haveLeftBorder)
				printf("# ");
			else
				printf("  ");

			if (field[i][j].haveRightBorder)
				printf("#");
			else
				printf(" ");
		}

		printf("\n");
	}

	printf("\n\n\n");
}


void waveTrace(int i, int j, int current)
{
	if (current > 1024)
	{
		printf("TOO MANY CALLS!");
		return;
	}

	int cr = current;
	field[i][j].waveTraceFactor = cr;
	field[i][j].proceed = 1;

	if (!field[i][j].haveLeftBorder)
	{
		if (field[i][j - 1].proceed == 0)
		{
			waveTrace(i, j - 1, cr + 1);
		}
	}

	if (!field[i][j].haveUpperBorder)
	{
		if (field[i - 1][j].proceed == 0)
		{

			waveTrace(i - 1, j, cr + 1);
		}
	}

	if (!field[i][j].haveRightBorder)
	{
		if (field[i][j + 1].proceed == 0)
		{
			waveTrace(i, j + 1, cr + 1);
		}
	}

	if (!field[i][j].haveBottomBorder)
	{
		if (field[i + 1][j].proceed == 0)
		{
			waveTrace(i + 1, j, cr + 1);
		}
	}

}


void backTrace(i, j)
{
	field[i][j].proceed = 3;
	int target = field[i][j].waveTraceFactor - 1;

	if (field[i][j].waveTraceFactor != 0)
	{
		if (!field[i][j].haveLeftBorder && field[i][j - 1].waveTraceFactor == target)
		{
			backTrace(i, j - 1);
			return;
		}

		if (!field[i][j].haveUpperBorder && field[i - 1][j].waveTraceFactor == target)
		{
			backTrace(i - 1, j);
			return;
		}

		if (!field[i][j].haveRightBorder && field[i][j + 1].waveTraceFactor == target)
		{
			backTrace(i, j + 1);
			return;
		}

		if (!field[i][j].haveBottomBorder && field[i + 1][j].waveTraceFactor == target)
		{
			backTrace(i + 1, j);
			return;
		}
	}
}

void buildPath(int x1, int y1, int x2, int y2)
{
	//clean-up previous tracing
	for (int x = 0; x < ROWS; x++)
		for (int y = 0; y < COLS; y++)
		{
			setCellWavefactor(&field[x][y], 0, 0);
		}

	//Tracing
	waveTrace(x1, y1, 0);

	//output field with wavefactors
	displayLabyrinth(1);

	//finding and marking path
	backTrace(x2, y2);

	//output field with path
	displayLabyrinth(2);
}


void go(int i, int j)
{
	field[i][j].proceed = 4;

	if (!field[i][j].haveLeftBorder && field[i][j - 1].proceed == 3)
	{
		if (orientation == ORIENTATION_LEFT)
		{
			printf("+forward\n");
		}
		else
		{
			printf("+make orientation -> LEFT\n");
			orientation = ORIENTATION_LEFT;
			printf("+forward\n");
		}

		go(i, j - 1);
		return;
	}

	if (!field[i][j].haveUpperBorder && field[i - 1][j].proceed == 3)
	{
		if (orientation == ORIENTATION_UP)
		{
			printf("+forward\n");
		}
		else
		{
			printf("+make orientation -> UP\n");
			orientation = ORIENTATION_UP;
			printf("+forward\n");
		}

		go(i - 1, j);
		return;
	}

	if (!field[i][j].haveRightBorder && field[i][j + 1].proceed == 3)
	{
		if (orientation == ORIENTATION_RIGHT)
		{
			printf("+forward\n");
		}
		else
		{
			printf("+make orientation -> RIGHT\n");
			orientation = ORIENTATION_RIGHT;
			printf("+forward\n");
		}

		go(i, j + 1);
		return;
	}

	if (!field[i][j].haveBottomBorder && field[i + 1][j].proceed == 3)
	{
		if (orientation == ORIENTATION_BOTTOM)
		{
			printf("+forward\n");
		}
		else
		{
			printf("+make orientation -> BOTTOM\n");
			orientation = ORIENTATION_BOTTOM;
			printf("+forward\n");
		}

		go(i + 1, j);
		return;
	}
}

int main()
{
	//
	// FIELD DEFINITION
	//

	//setting no border by default
	for (int x = 0; x < ROWS; x++)
		for (int y = 0; y < COLS; y++)
		{
			setCellParameters(&field[x][y], NULL);
			setCellWavefactor(&field[x][y], 0, 0);
		}

	//setting borders (for individual labyrinth)
	//row 0 (upper row)
	setCellParameters(&field[0][0], "UxLR");
	setCellParameters(&field[0][1], "UxLR");
	setCellParameters(&field[0][2], "UxLR");
	setCellParameters(&field[0][3], "UxLR");
	//row 1
	setCellParameters(&field[1][0], "xxLR");
	setCellParameters(&field[1][1], "xxLR");
	setCellParameters(&field[1][2], "xxLx");
	setCellParameters(&field[1][3], "xxxR");
	//row 2
	setCellParameters(&field[2][0], "xxLR");
	setCellParameters(&field[2][1], "xxLx");
	setCellParameters(&field[2][2], "xxxx");
	setCellParameters(&field[2][3], "xxxR");
	//row 3
	setCellParameters(&field[3][0], "xBLx");
	setCellParameters(&field[3][1], "xBxx");
	setCellParameters(&field[3][2], "xxxR");
	setCellParameters(&field[3][3], "xxLR");
	//row 4 (bottom row)
	setCellParameters(&field[4][0], "UBLx");
	setCellParameters(&field[4][1], "UBxx");
	setCellParameters(&field[4][2], "xBxR");
	setCellParameters(&field[4][3], "xBLR");

	//output EMPTY field
	displayLabyrinth(0);

	buildPath(0, 0, 0, 1);
	go(0, 0);

	getchar();
	return 0;
}
