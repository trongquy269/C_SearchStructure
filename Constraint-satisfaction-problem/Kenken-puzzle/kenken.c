#include <stdio.h>
#include <string.h>

#define ROW 4
#define COL 4
#define EMPTY 0
#define MAX_VALUE 4

typedef struct
{
    int x;
    int y;
} Coordinates;

typedef struct
{
    int data[MAX_VALUE + 1];
    int size;
} List;

void initList(List *list)
{
    list->size = -1;
}

int isEmpty(List list)
{
    return list.size == -1;
}

void pushList(List *list, int value)
{
    list->size++;
    list->data[list->size] = value;
}

void sortMaxList(List *list)
{
    for (int i = 0; i < list->size; i++)
    {
        for (int j = i; j <= list->size; j++)
        {
            if (list->data[i] < list->data[j])
            {
                int temp = list->data[i];
                list->data[i] = list->data[j];
                list->data[j] = temp;
            }
        }
    }
}

typedef struct
{
    int value;
    char operation;
    Coordinates block[ROW * COL];
    int numOfEmptyCells;
} Frame;

typedef struct
{
    Frame blocks[ROW * COL];
    int numOfBlock;
    int table[ROW][COL];
} KenkenPuzzle;

void initKenkenPuzzle(KenkenPuzzle *kenken)
{
    kenken->numOfBlock = -1;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            kenken->table[i][j] = EMPTY;
        }
    }
}

int isFill(KenkenPuzzle kenken)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (kenken.table[i][j] == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

void readData(KenkenPuzzle *kenken)
{
    freopen("data.txt", "r", stdin);
    scanf("%d", &kenken->numOfBlock);
    for (int i = 0; i < kenken->numOfBlock; i++)
    {
        Frame frame;
        scanf("%d", &frame.numOfEmptyCells);
        for (int j = 0; j < frame.numOfEmptyCells; j++)
        {
            Coordinates coord;
            scanf("%d", &coord.x);
            scanf("%d", &coord.y);
            frame.block[j] = coord;
        }
        scanf("%d", &frame.value);
        if (frame.numOfEmptyCells > 1)
        {
            char str[1];
            scanf("%s", str);
            frame.operation = str[0];
        }
        else
        {
            frame.operation = '\0';
        }
        kenken->blocks[i] = frame;
    }
}

void printKenkenPuzzle(KenkenPuzzle kenken)
{
    for (int i = 0; i < ROW; i++)
    {
        if (i == 0)
        {
            printf("|---------------|\n");
        }
        for (int j = 0; j < COL; j++)
        {
            if (j < COL - 1)
            {
                printf("| %d ", kenken.table[i][j]);
            }
            else
            {
                printf("| %d |\n", kenken.table[i][j]);
            }
        }
        printf("|---------------|\n");
    }
}

List getValueDomain(KenkenPuzzle kenken, Coordinates coord)
{
    int valueDomain[MAX_VALUE + 1];
    for (int i = 1; i <= MAX_VALUE; i++)
    {
        valueDomain[i] = 1;
    }
    // Constraint follow row
    for (int i = 0; i < ROW; i++)
    {
        if (i != coord.x)
        {
            if (kenken.table[i][coord.y] != EMPTY)
            {
                valueDomain[kenken.table[i][coord.y]] = 0;
            }
        }
    }
    // Constraint follow col
    for (int i = 0; i < COL; i++)
    {
        if (i != coord.y)
        {
            if (kenken.table[coord.x][i] != EMPTY)
            {
                valueDomain[kenken.table[coord.x][i]] = 0;
            }
        }
    }

    List result;
    initList(&result);

    for (int i = 1; i <= MAX_VALUE; i++)
    {
        if (valueDomain[i] == 1)
        {
            pushList(&result, i);
        }
    }

    return result;
}

int isResult(int result, char operation, List list)
{
    for (int i = 0; i <= list.size; i++)
    {
        if (list.data[i] == EMPTY)
        {
            return 0;
        }
    }
    if (operation == '\0')
    {
        return result == list.data[0];
    }
    else if (operation == '+')
    {
        int sum = 0;
        for (int i = 0; i <= list.size; i++)
        {
            sum += list.data[i];
        }
        return result == sum;
    }
    else if (operation == '-')
    {
        sortMaxList(&list);
        int subtract = list.data[0];
        for (int i = 1; i <= list.size; i++)
        {
            subtract -= list.data[i];
        }
        return result == subtract;
    }
    else if (operation == '*')
    {
        int multiply = 1;
        for (int i = 0; i <= list.size; i++)
        {
            multiply *= list.data[i];
        }
        return result == multiply;
    }
    else if (operation == '/')
    {
        sortMaxList(&list);
        int division = list.data[0];
        for (int i = 1; i <= list.size; i++)
        {
            division /= list.data[i];
        }
        return result == division;
    }
    else
    {
        return 0;
    }
}

List getValueConstraintInBlock(KenkenPuzzle kenken, Frame block)
{
    List blockList;
    initList(&blockList);

    for (int i = 0; i < block.numOfEmptyCells; i++)
    {
        Coordinates position = block.block[i];
        pushList(&blockList, kenken.table[position.x][position.y]);
    }

    return blockList;
}

int isGoal(KenkenPuzzle kenken)
{
    if (isFill(kenken))
    {
        for (int i = 0; i < kenken.numOfBlock; i++)
        {
            List blockList = getValueConstraintInBlock(kenken, kenken.blocks[i]);
            if (!isResult(kenken.blocks[i].value, kenken.blocks[i].operation, blockList))
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int exploredCounter = 0;
int kenkenSolve(KenkenPuzzle *kenken)
{
    if (isGoal(*kenken))
    {
        return 1;
    }
    if (isFill(*kenken))
    {
        return 0;
    }
    exploredCounter++;
    printf("%d\n", exploredCounter);
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            Coordinates position = {i, j};
            if (kenken->table[position.x][position.y] == EMPTY)
            {
                List valueDomain = getValueDomain(*kenken, position);
                if (isEmpty(valueDomain))
                {
                    return 0;
                }
                for (int k = 0; k <= valueDomain.size; k++)
                {
                    kenken->table[position.x][position.y] = valueDomain.data[k];
                    if (kenkenSolve(kenken))
                    {
                        return 1;
                    }
                    kenken->table[position.x][position.y] = EMPTY;
                }
            }
        }
    }
    return 0;
}

int main()
{
    KenkenPuzzle kenken;
    initKenkenPuzzle(&kenken);
    readData(&kenken);
    if (kenkenSolve(&kenken))
    {
        printf("Explored: %d -> Solved: \n", exploredCounter);
        printKenkenPuzzle(kenken);
    }
    else
    {
        printf("Can't find solve!\n");
    }
    return 0;
}