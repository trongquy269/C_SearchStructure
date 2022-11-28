#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LENGTH 500
#define ROW 3
#define COL 3
#define EMPTY 0

typedef struct
{
    int puzzle[ROW][COL];
    int emptyRow;
    int emptyCol;
} State;

void printState (State state)
{
    printf("-------------\n");
    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            printf("| %d ", state.puzzle[row][col]);
        }
        printf("|\n");
        printf("-------------\n");
    }
}

int compareState (State state1, State state2)
{
    if ((state1.emptyRow != state2.emptyRow) || (state1.emptyCol != state2.emptyCol))
    {
        return 0;
    }
    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            if (state1.puzzle[row][col] != state2.puzzle[row][col])
            {
                return 0;
            }
        }
    }
    return 1;
}

int goalCheck (State state, State goal)
{
    return compareState(state, goal); 
}

int upOperator (State current, State *result)
{
    if (current.emptyRow > 0)
    {
        result->emptyRow = current.emptyRow - 1;
        result->puzzle[current.emptyRow][current.emptyCol] = current.puzzle[result->emptyRow][result->emptyCol];
        result->puzzle[result->emptyRow][result->emptyCol] = EMPTY;
        return 1;
    }
    return 0;
}

int downOperator (State current, State *result)
{
    if (current.emptyRow < ROW - 1)
    {
        result->emptyRow = current.emptyRow + 1;
        result->puzzle[current.emptyRow][current.emptyCol] = current.puzzle[result->emptyRow][result->emptyCol];
        result->puzzle[result->emptyRow][result->emptyCol] = EMPTY;
        return 1;
    }
    return 0;
}

int leftOperator (State current, State *result)
{
    if (current.emptyCol > 0)
    {
        result->emptyCol = current.emptyCol - 1;
        result->puzzle[current.emptyRow][current.emptyCol] = current.puzzle[result->emptyRow][result->emptyCol];
        result->puzzle[result->emptyRow][result->emptyCol] = EMPTY;
        return 1;
    }
    return 0;
}

int rightOperator (State current, State *result)
{
    if (current.emptyCol < COL - 1)
    {
        result->emptyCol = current.emptyCol + 1;
        result->puzzle[current.emptyRow][current.emptyCol] = current.puzzle[result->emptyRow][result->emptyCol];
        result->puzzle[result->emptyRow][result->emptyCol] = EMPTY;
        return 1;
    }
    return 0;
}

int callOperator (State current, State *result, int opt)
{
    switch (opt)
    {
        case 1: return upOperator(current, result);
        case 2: return downOperator(current, result);
        case 3: return leftOperator(current, result);
        case 4: return rightOperator(current, result);
        default:
        {
            printf("Error call operator\n");
            return 0;
        }
    }
}

const char *action[] = {"Start state", "Up operator", "Down operator", "Left operator", "Right operator"};

typedef struct node
{
    State state;
    struct node *parent;
    int numberOrderAction;
    int heuristic;
} node;

typedef struct
{
    node *data[MAX_LENGTH];
    int size;
} list;

void initList (list *l)
{
    l->size = -1;
}

int isEmpty (list l)
{
    return l.size == -1;
}

int isFull (list l)
{
    return l.size == MAX_LENGTH;
}

node *elementAt (list l, int position)
{
    return l.data[position];
}

void push (list *l, node *value)
{
    if (isFull(*l))
    {
        printf("Error! List is full\n");
    }
    else
    {
        l->size++;
        l->data[l->size] = value;
    }
}

void removeList (list *l, int position)
{
    if (isEmpty(*l))
    {
        printf("Error! List is empty\n");
    }
    else
    {
        for (int i = position; i <= l->size - 1; i++)
        {
            l->data[i] = l->data[i + 1];
        }
        l->size--;
    }
}

int findState (list l, State state)
{
    for (int i = 0; i <= l.size; i++)
    {
        if (compareState(elementAt(l, i)->state, state))
        {
            return 1;
        }
    }
    return 0;
}

void sortList (list *l)
{
    for (int i = 0; i <= l->size - 1; i++)
    {
        for (int j = i + 1; j <= l->size; j++)
        {
            if (l->data[i]->heuristic > l->data[j]->heuristic)
            {
                node *temp = l->data[i];
                l->data[i] = l->data[j];
                l->data[j] = temp;
            }
        }
    }
}

int heuristic (State state, State goal)
{
    int count = 0;
    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            if (state.puzzle[row][col] != goal.puzzle[row][col])
            {
                count++;
            }
        }
    }
    return count;
}

node *bestFirstSearch (State startState, State goal)
{
    list open;
    list close;
    initList(&open);
    initList(&close);

    node *root = (node*)malloc(sizeof(node));
    root->state = startState;
    root->parent = NULL;
    root->numberOrderAction = 0;
    root->heuristic = heuristic(root->state, goal);
    push(&open, root);

    while (!isEmpty(open))
    {
        node *tempNode = elementAt(open, 0);
        removeList(&open, 0);
        push(&close, tempNode);

        if (goalCheck(tempNode->state, goal))
        {
            return tempNode;
        }

        for (int opt = 1; opt <= 4; opt++)
        {
            State newState;
            newState = tempNode->state;
            if (callOperator(tempNode->state, &newState, opt))
            {
                node *newNode = (node*)malloc(sizeof(node));
                newNode->state = newState;
                newNode->parent = tempNode;
                newNode->numberOrderAction = opt;
                newNode->heuristic = heuristic(newState, goal);

                if (!(findState(open, newState)) && !(findState(close, newState)))
                {
                    push(&open, newNode);
                }
                sortList(&open);
            }
        }
    }
    return NULL;
}

void printWaysToGetGoal (node *goalNode)
{
    if (goalNode == NULL)
    {
        printf("Don't find goal\n");
    }
    else
    {
        list listPrint;
        initList(&listPrint);
        while (goalNode != NULL)
        {
            push(&listPrint, goalNode);
            goalNode = goalNode->parent;
        }
        int numberOrderAction = 0;
        for (int i = listPrint.size; i >=0; i--)
        {
            printf("\nHanh dong %d: %s\n", numberOrderAction, action[elementAt(listPrint, i)->numberOrderAction]);
            printState(elementAt(listPrint, i)->state);
            numberOrderAction++;
        }
    }
}

int main()
{
    State starState;
    starState.puzzle[0][0] = 3;
    starState.puzzle[0][1] = 4;
    starState.puzzle[0][2] = 5;
    starState.puzzle[1][0] = 1;
    starState.puzzle[1][1] = EMPTY;
    starState.puzzle[1][2] = 2;
    starState.puzzle[2][0] = 6;
    starState.puzzle[2][1] = 7;
    starState.puzzle[2][2] = 8;
    starState.emptyRow = 1;
    starState.emptyCol = 1;

    State goal;
    goal.puzzle[0][0] = EMPTY;
    goal.puzzle[0][1] = 1;
    goal.puzzle[0][2] = 2;
    goal.puzzle[1][0] = 3;
    goal.puzzle[1][1] = 4;
    goal.puzzle[1][2] = 5;
    goal.puzzle[2][0] = 6;
    goal.puzzle[2][1] = 7;
    goal.puzzle[2][2] = 8;
    goal.emptyRow = 0;
    goal.emptyCol = 0;

    node *tempNode = bestFirstSearch(starState, goal);
    printWaysToGetGoal(tempNode);
    free(tempNode);
    return 0;
}