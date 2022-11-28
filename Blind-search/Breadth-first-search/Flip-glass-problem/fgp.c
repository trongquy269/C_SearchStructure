#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 500
#define numberOfGlass 6

typedef struct
{
    int glass[numberOfGlass];
} State;

typedef struct node
{
    State state;
    struct node *parent;
    int numberOrderAction;
} node;

typedef struct
{
    node *data[MAX_LENGTH];
    int front, rear;
} queue;

void initQueue (queue *q)
{
    q->front = q->rear = -1;
}

int isEmpty (queue q)
{
    return q.front == -1;
}

int isFull (queue q)
{
    return (q.rear - q.front + 1) % MAX_LENGTH == 0;
}

void enQueue (queue *q, node *value)
{
    if (isFull(*q))
    {
        printf("Error! Queue is full\n");
    }
    else
    {
        if (isEmpty(*q))
        {
            q->front = 0;
        }
        q->rear = (q->rear + 1) % MAX_LENGTH;
        q->data[q->rear] = value;
    }
}

node *deQueue (queue *q)
{
    if (isEmpty(*q))
    {
        printf("Error! Queue is\n");
    }
    else
    {
        node *data = q->data[q->front];
        if (q->front == q->rear)
        {
            initQueue(q);
        }
        else
        {
            q->front = (q->front + 1) % MAX_LENGTH;
        }
        return data;
    }
}

node *peek (queue q)
{
    if (isEmpty(q))
    {
        printf("Error! Queue is empty\n");
    }
    else
    {
        return q.data[q.front];
    }
}

int findQueue (queue q, State state)
{
    while (!isEmpty(q))
    {
        int check = 0;
        State tempState = deQueue(&q)->state;
        for (int i = 0; i < numberOfGlass; i++)
        {
            if (tempState.glass[i] == state.glass[i])
            {
                check = 1;
            }
            else
            {
                check = 0;
                break;
            }
        }
        if (check == 1)
        {
            return 1;
        }
    }
    return 0;
}

int goalCheck (State current)
{
    for (int i = 0; i < numberOfGlass; i++)
    {
        if (current.glass[i] != 1)
        {
            return 0;
        }
    }
    return 1;
}

int flipGlass (int value)
{
    if (value == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void printGlass (State state)
{
    for (int i = 0; i < numberOfGlass; i++)
    {
        printf("%d ", state.glass[i]);
    }
    printf("\n");
}

void flip123Glass (State current, State *result)
{
    result->glass[0] = flipGlass(current.glass[0]);
    result->glass[1] = flipGlass(current.glass[1]);
    result->glass[2] = flipGlass(current.glass[2]);
}

void flip234Glass (State current, State *result)
{
    result->glass[1] = flipGlass(current.glass[1]);
    result->glass[2] = flipGlass(current.glass[2]);
    result->glass[3] = flipGlass(current.glass[3]);
}

void flip345Glass (State current, State *result)
{
    result->glass[2] = flipGlass(current.glass[2]);
    result->glass[3] = flipGlass(current.glass[3]);
    result->glass[4] = flipGlass(current.glass[4]);
}

void flip456Glass (State current, State *result)
{
    result->glass[3] = flipGlass(current.glass[3]);
    result->glass[4] = flipGlass(current.glass[4]);
    result->glass[5] = flipGlass(current.glass[5]);
}

void callOperator (State current, State *result, int opt)
{
    switch (opt)
    {
        case 1:
        {
            flip123Glass(current, result);
            break;
        }
        case 2:
        {
            flip234Glass(current, result);
            break;
        }
        case 3:
        {
            flip345Glass(current, result);
            break;
        }
        case 4:
        {
            flip456Glass(current, result);
            break;
        }
        default:
        {
            printf("Error call operator!\n");
            break;
        }
    }
}

const char *action[] = {"Start state", "Flip 1, 2, 3 glass", "Flip 2, 3, 4 glass", "Flip 3, 4, 5 glass", "Flip 4, 5, 6 glass"};

node *BFS (State startState)
{
    queue open;
    queue close;
    initQueue(&open);
    initQueue(&close);

    node *root = (node*)malloc(sizeof(node));
    root->state = startState;
    root->parent = NULL;
    root->numberOrderAction = 0;
    enQueue(&open, root);

    while (!isEmpty(open))
    {
        node *nodeTop = deQueue(&open);
        enQueue(&close, nodeTop);

        if (goalCheck(nodeTop->state))
        {
            return nodeTop;
        }

        for (int opt = 1; opt <= 4; opt++)
        {
            State newState = nodeTop->state;
            callOperator(nodeTop->state, &newState, opt);
            if (!findQueue(open, newState) && !findQueue(close, newState))
            {
                node *newNode = (node*)malloc(sizeof(node));
                newNode->state = newState;
                newNode->parent = nodeTop;
                newNode->numberOrderAction = opt;
                enQueue(&open, newNode);
            }
        }
    }
    return NULL;
}

void printWaysToGetGoal (node *startNode)
{
    node *state[MAX_LENGTH];
    int key = -1;

    while (startNode != NULL)
    {
        key++;
        state[key] = startNode;
        startNode = startNode->parent;
    }

    int count = 0;

    while (key >= 0)
    {
        printf("Hanh dong %d: %s ", count, action[state[key]->numberOrderAction]);
        printGlass(state[key]->state);
        count++;
        key--;
    }
}

int main()
{
    State startState;
    startState.glass[0] = 1;
    startState.glass[1] = 0;
    startState.glass[2] = 1;
    startState.glass[3] = 0;
    startState.glass[4] = 1;
    startState.glass[5] = 0;

    node *tempNode = BFS(startState);
    printWaysToGetGoal(tempNode);
    free(tempNode);
    
    return 0;
}