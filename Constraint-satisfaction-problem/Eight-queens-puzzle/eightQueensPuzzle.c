#include <stdio.h>

#define ROW 8
#define COL 8
#define EMPTY 0
#define MAX_VALUE 8

typedef struct
{
    int x;
    int y;
} Coordinates;

typedef struct
{
    int table[ROW][COL];
} EightQueensPuzzle;

void initPuzzle(EightQueensPuzzle *puzzle)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            puzzle->table[i][j] = EMPTY;
        }
    }
}

int getValueDomain(EightQueensPuzzle puzzle, Coordinates coord)
{
    // Row binding
    for (int i = 0; i < COL; i++)
    {
        if (i != coord.y)
        {
            if (puzzle.table[coord.x][i] != EMPTY)
            {
                return 0;
            }
        }
    }
    // Diagonal line binding
    // Right down
    for (int j = 1; j < COL; j++)
    {
        if ((coord.x < ROW) && (coord.y < COL))
        {
            if ((coord.x + j) < ROW && (coord.y + j) < COL)
            {
                if (puzzle.table[coord.x + j][coord.y + j] != EMPTY)
                {
                    return 0;
                }
            }
        }
    }
    // Right up
    for (int j = 1; j < COL; j++)
    {
        if ((coord.x > 0) && (coord.y < COL))
        {
            if ((coord.x - j) >= 0 && (coord.y + j) < COL)
            {
                if (puzzle.table[coord.x - j][coord.y + j] != EMPTY)
                {
                    return 0;
                }
            }
        }
    }
    // Left up
    for (int j = 1; j < COL; j++)
    {
        if ((coord.x > 0) && (coord.y > 0))
        {
            if ((coord.x - j) >= 0 && (coord.y - j) >= 0)
            {
                if (puzzle.table[coord.x - j][coord.y - j] != EMPTY)
                {
                    return 0;
                }
            }
        }
    }
    // Left down
    for (int j = 1; j < COL; j++)
    {
        if ((coord.x > ROW) && (coord.y > 0))
        {
            if ((coord.x - j) < ROW && (coord.y - j) >= 0)
            {
                if (puzzle.table[coord.x - j][coord.y - j] != EMPTY)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}

void printPuzzle(EightQueensPuzzle puzzle)
{
    for (int i = 0; i < ROW; i++)
    {
        if (i == 0)
        {
            printf("|-------------------------------|\n");
        }
        for (int j = 0; j < COL; j++)
        {
            if (puzzle.table[i][j] == EMPTY)
            {
                printf("|   ");
                if (j == COL - 1)
                {
                    printf("|\n");
                }
            }
            else
            {
                printf("| %d ", puzzle.table[i][j]);
                if (j == COL - 1)
                {
                    printf("|\n");
                }
            }
        }
        printf("|-------------------------------|\n");
    }
}

int isFill(EightQueensPuzzle puzzle)
{
    int check = 0;

    for (int j = 0; j < COL; j++)
    {
        for (int i = 0; i < ROW; i++)
        {
            if (puzzle.table[i][j] != EMPTY)
            {
                check = 1;
                break;
            }
            else
            {
                check = 0;
            }
        }
    }
    return check;
}

int noQueen(EightQueensPuzzle puzzle, int positionY)
{
    for (int i = 0; i < ROW; i++)
    {
        if (puzzle.table[i][positionY] != EMPTY)
        {
            return 0;
        }
    }
    return 1;
}

int exploredCounter = 0;

int eightQueensPuzzle(EightQueensPuzzle *puzzle)
{
    if (isFill(*puzzle))
    {
        return 1;
    }

    exploredCounter++;

    for (int col = 0; col < COL; col++)
    {
        if (noQueen(*puzzle, col))
        {
            for (int i = 0; i < ROW; i++)
            {
                Coordinates position = {i, col};
                if (getValueDomain(*puzzle, position))
                {
                    puzzle->table[position.x][position.y] = 1;
                    if (eightQueensPuzzle(puzzle))
                    {
                        return 1;
                    }
                    puzzle->table[position.x][position.y] = 0;
                }
            }
            return 0;
        }
    }
    return 0;
}

int main()
{
    EightQueensPuzzle puzzle;
    initPuzzle(&puzzle);

    if (eightQueensPuzzle(&puzzle))
    {
        printf("After %d state -> Solved\n", exploredCounter);
        printPuzzle(puzzle);
    }
    else
    {
        printf("Can't find solve!\n");
    }

    return 0;
}