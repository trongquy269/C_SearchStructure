#include <iostream>
#include <math.h>
#include <list>

using namespace std;

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
    cout << "-------------\n";
    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            cout << "| " << state.puzzle[row][col] << " ";
        }
        cout << endl;
        cout << "-------------\n";
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
            cout << "Error call operator\n";
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

int findState (list <node*> l, State state)
{
    while (!l.empty())
    {
        if (compareState(l.front()->state, state))
        {
            return 1;
        }
        l.pop_front();
    }
    return 0;
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
    list <node*> open;
    list <node*> close;

    node *root = new node;
    root->state = startState;
    root->parent = NULL;
    root->numberOrderAction = 0;
    root->heuristic = heuristic(root->state, goal);
    open.push_front(root);

    int count = 0;

    while (!open.empty())
    {
        count++;
        node *tempNode = open.front();
        open.pop_front();
        close.push_front(tempNode);

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
                node *newNode = new node;
                newNode->state = newState;
                newNode->parent = tempNode;
                newNode->numberOrderAction = opt;
                newNode->heuristic = heuristic(newState, goal);

                if (!(findState(open, newState)) && !(findState(close, newState)))
                {
                    if (newNode->heuristic <= open.front()->heuristic)
                    {
                        open.push_front(newNode);
                    }
                    else
                    {
                        open.push_back(newNode);
                    }
                }
            }
        }
    }
    return NULL;
}

void printWaysToGetGoal (node *goalNode)
{
    if (goalNode == NULL)
    {
        cout << "Don't find goal\n";
    }
    else
    {
        list <node*> listPrint;
        while (goalNode != NULL)
        {
            listPrint.push_back(goalNode);
            goalNode = goalNode->parent;
        }
        int numberOrderAction = 0;
        while (!listPrint.empty())
        {
            cout << "Hanh dong " << numberOrderAction << ": " << action[listPrint.back()->numberOrderAction] << endl;
            printState(listPrint.back()->state);
            listPrint.pop_back();
            numberOrderAction++;
        }
        cout << "Goal\n";
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

    return 0;
}
