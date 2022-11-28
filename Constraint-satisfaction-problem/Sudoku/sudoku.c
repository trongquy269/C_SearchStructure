#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 30
#define ROW 9
#define COL 9
#define AREA_SQUARE_SIZE 3
#define MAX_VALUE 10
#define EMPTY 0

// int valueInput[ROW][COL] = 
// {
//     {5, 3, 0, 0, 7, 0, 0, 0, 0},
//     {6, 0, 0, 1, 9, 5, 0, 0, 0},
//     {0, 9, 8, 0, 0, 0, 0, 6, 0},
//     {8, 0, 0, 0, 6, 0, 0, 0, 3},
//     {4, 0, 0, 8, 0, 3, 0, 0, 1},
//     {7, 0, 0, 0, 2, 0, 0, 0, 6},
//     {0, 6, 0, 0, 0, 0, 2, 8, 0},
//     {0, 0, 0, 4, 1, 9, 0, 0, 5},
//     {0, 0, 0, 0, 8, 0, 0, 7, 9},
// };
int valueInput[ROW][COL] = 
{
    {0, 6, 1, 0, 0, 7, 0, 0, 3},
    {0, 9, 2, 0, 0, 3, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 8, 5, 3, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 5, 0, 4},
    {5, 0, 0, 0, 0, 8, 0, 0, 0},
    {0, 4, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 6, 0, 8, 0, 0},
    {6, 0, 0, 0, 0, 0, 0, 0, 0},
};

typedef struct 
{
    int x;
    int y;
} Coordinates;

typedef struct
{
    Coordinates data[MAX_LENGTH];
    int size;
} ListConstrain;

void initListConstrain (ListConstrain *list)
{
    list->size = -1;
}

int isEmptyListConstrain (ListConstrain list)
{
    return list.size == -1;
}

void pushListConstrain (ListConstrain *list, Coordinates coord)
{
    list->size++;
    list->data[list->size] = coord;
}

typedef struct
{
    int data[MAX_VALUE];
    int size;
} List;

void initList (List *list)
{
    list->size = -1;
}

int isEmptyList (List list)
{
    return list.size == -1;
}

void pushList (List *list, int value)
{
    list->size++;
    list->data[list->size] = value;
}

int indexOfCoord (Coordinates coord)
{
    return (ROW * coord.x + coord.y);
}

Coordinates positionOfVertex (int vertex)
{
    Coordinates coord;
    coord.x = vertex / ROW;
    coord.x = vertex % ROW;
    return coord;
}

typedef struct
{
    int table[ROW][COL];
} Sudoku;

void initSudoku (Sudoku *sudoku, int valueInput[ROW][COL])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            sudoku->table[i][j] = valueInput[i][j];
        }
    }
}

void printSudoku (Sudoku sudoku)
{
    for (int i = 0; i < ROW; i++)
    {
        if (i % AREA_SQUARE_SIZE == 0)
        {
            printf("|-----------------------|\n");
        }
        for (int j = 0; j < COL; j++)
        {
            if (j % AREA_SQUARE_SIZE == 0)
            {
                printf("| ");
            }
            printf("%d ", sudoku.table[i][j]);
        }
        printf("|\n");
    }
    printf("|-----------------------|\n");
}

int isFill (Sudoku sudoku)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (sudoku.table[i][j] == EMPTY)
            {
                return 0;
            }
        }
    }
    return 1;
}

void createConstrainsOfCoord (Coordinates coord, ListConstrain *list)
{
    // Constrain follow row
    for (int i = 0; i < COL; i++)
    {
        if (i != coord.y)
        {
            Coordinates position;
            position.x = coord.x;
            position.y = i;
            pushListConstrain(list, position);
        }
    }

    // Constrain follow col
    for (int i = 0; i < ROW; i++)
    {
        if (i != coord.x)
        {
            Coordinates position;
            position.x = i;
            position.y = coord.y;
            pushListConstrain(list, position);
        }
    }

    // Constrain follow area square
    int x = (coord.x / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
    int y = (coord.y / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;

    for (int i = x; i < x + 3; i++)
    {
        for (int j = y; j < y + 3; j++)
        {
            if ((i != coord.x) && (j != coord.y))
            {
                Coordinates position;
                position.x = i;
                position.y = j;
                pushListConstrain(list, position);
            }
        }
    }
}

List getValueDomain (Sudoku sudoku, Coordinates coord)
{
    // initialization value domain
    int list[MAX_VALUE];
    for (int i = 1; i < MAX_VALUE; i++)
    {
        list[i] = 1;
    }

    ListConstrain lConstrain;
    initListConstrain(&lConstrain);
    createConstrainsOfCoord(coord, &lConstrain);

    for (int i = 0; i <= lConstrain.size; i++)
    {
        Coordinates position = lConstrain.data[i];
        if (sudoku.table[position.x][position.y] != EMPTY)
        {
            list[sudoku.table[position.x][position.y]] = 0;
        }
    }
    
    List result;
    initList(&result);
    for (int i = 1; i < MAX_VALUE; i++)
    {
        if (list[i])
        {
            pushList(&result, i);
        }
    }
    return result;
}

Coordinates getNextMinDomainCell (Sudoku sudoku)
{
    Coordinates result;
    int minLength = 10;

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (sudoku.table[i][j] == EMPTY)
            {
                Coordinates position;
                position.x = i;
                position.y = j;
                int length = getValueDomain(sudoku, position).size;
                if (length < minLength)
                {
                    minLength = length;
                    result = position;
                }
            }
        }
    }
    return result;
}

int exploredCounter = 0;

int sudokuBacktracking (Sudoku *sudoku)
{
    if (isFill(*sudoku))
    {
        return 1;
    }

    exploredCounter++;

    Coordinates position = getNextMinDomainCell(*sudoku);
    List listConstrain;
    initList(&listConstrain);
    listConstrain = getValueDomain(*sudoku, position);

    if (isEmptyList(listConstrain))
    {
        return 0;
    }

    for (int i = 0; i <= listConstrain.size; i++)
    {
        sudoku->table[position.x][position.y] = listConstrain.data[i];
        if (sudokuBacktracking(sudoku))
        {
            return 1;
        }
        sudoku->table[position.x][position.y] = EMPTY;
    }
    return 0;
}

int main()
{
    Sudoku sudoku;
    initSudoku(&sudoku, valueInput);
    printSudoku(sudoku);
    if (sudokuBacktracking(&sudoku))
    {
        printf("Solved it!\n");
        printf("Explored: %d state\n", exploredCounter);
        printSudoku(sudoku);
    }
    else
    {
        printf("Can't solve!\n");
    }
    return 0;
}