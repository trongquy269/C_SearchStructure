#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 500

int tankCapacityA = 10;
int tankCapacityB = 5;
int tankCapacityC = 6;
int goal = 8;

typedef struct
{
    int a;
    int b;
    int c;
} State;

typedef struct node
{
    State state;
    struct node *parent;
    int numberOrderAction;
} node;

void emptyState (State *current)
{
    current->a = 0;
    current->b = 0;
    current->c = 0;
}

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
        printf("Error enqueue! Queue is full\n");
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
        printf("Error dequeue! Queue is empty\n");
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
        printf("Error peek! Queue is empty\n");
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
        State tempState =deQueue(&q)->state;
        if ((tempState.a == state.a) && (tempState.b == state.b) && (tempState.c == state.c))
        {
            return 1;
        }
    }
    return 0;
}
// End: queue

int goalCheck (State current)
{
    if ((current.a == goal) || (current.b == goal) || (current.c == goal))
    {
        return 1;
    }
    return 0;
}

int pourAB (State current, State *water)
{
    if ((current.a != 0) && (current.b != tankCapacityB))
    {
        if (current.a <= tankCapacityB - current.b)
        {
            water->b = current.b + current.a;
            water->a = 0;
        }
        else
        {
            water->b = tankCapacityB;
            water->a = current.a - (tankCapacityB - current.b);
        }
        water->c = current.c;
        return 1;
    }
    return 0;
}

int pourBA (State current, State *water)
{
    if ((current.b != 0) && (current.a != tankCapacityA))
    {
        if (current.b <= tankCapacityA - current.a)
        {
            water->a = current.a + current.b;
            water->b = 0;
        }
        else
        {
            water->a = tankCapacityA;
            water->b = current.b - (tankCapacityA - current.a);
        }
        water->c = current.c;
        return 1;
    }
    return 0;
}

int pourBC (State current, State *water)
{
    if ((current.b != 0) && (current.c != tankCapacityC))
    {
        if (current.b <= tankCapacityC - current.c)
        {
            water->c = current.c + current.b;
            water->b = 0;
        }
        else
        {
            water->c = tankCapacityC;
            water->b = current.b - (tankCapacityC - current.c);
        }
        water->a = current.a;
        return 1;
    }
    return 0;
}

int pourCB (State current, State *water)
{
    if ((current.c != 0) && (current.b != tankCapacityB))
    {
        if (current.c <= tankCapacityB - current.b)
        {
            water->b = current.b + current.c;
            water->c = 0;
        }
        else
        {
            water->b = tankCapacityB;
            water->c = current.c - (tankCapacityB - current.b);
        }
        water->a = current.a;
        return 1;
    }
    return 0;
}

int pourAC (State current, State *water)
{
    if ((current.a != 0) && (current.c != tankCapacityC))
    {
        if (current.a <= tankCapacityC - current.c)
        {
            water->c = current.c + current.a;
            water->a = 0;
        }
        else
        {
            water->c = tankCapacityC;
            water->a = current.a - (tankCapacityC - current.c);
        }
        water->b = current.b;
        return 1;
    }
    return 0;
}

int pourCA (State current, State *water)
{
    if ((current.c != 0) && (current.a != tankCapacityA))
    {
        if (current.c <= tankCapacityA - current.a)
        {
            water->a = current.a + current.c;
            water->c = 0;
        }
        else
        {
            water->a = tankCapacityA;
            water->c = current.c - (tankCapacityA - current.a);
        }
        water->b = current.b;
        return 1;
    }
    return 0;
}

int callOperator (State current, State *water, int opt)
{
    switch (opt)
    {
        case 1: return pourAB(current, water);
        case 2: return pourBA(current, water);
        case 3: return pourBC(current, water);
        case 4: return pourCB(current, water);
        case 5: return pourAC(current, water);
        case 6: return pourCA(current, water);
        default: printf("Error call operators\n");
        return 0;
    }
}

const char *action[] = {"Start state", "Pour from A to B", "Pour from B to A", "Pour from B to C", "Pour from C to B", "Pour from A to C", "Pour from C to A"};

node *BFS (State startState)
{
    queue open;
    queue close;
    initQueue(&open);
    initQueue(&close);

    node *root = (node *)malloc(sizeof(node));
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

        int opt;

        for (opt = 1; opt <= 6; opt++)
        {
            State newState;
            emptyState(&newState);
            if (callOperator(nodeTop->state, &newState, opt))
            {
                if (!(findQueue(open, newState)) && !(findQueue(close, newState)))
                {
                    node *newNode = (node *)malloc(sizeof(node));
                    newNode->state = newState;
                    newNode->parent = nodeTop;
                    newNode->numberOrderAction = opt;
                    
                    enQueue(&open, newNode);
                }
            }
        }
    }
    return NULL;
}

void print_WaysToGetGoal (node *stateNode)
{
    node *state[MAX_LENGTH];
    int key = -1;

    while (stateNode != NULL)
    {
        key++;
        state[key] = stateNode;
        stateNode = stateNode->parent;
    }

    int numberOrderAction = 0;
    
    while (key >= 0)
    {
        printf("\nHanh dong %d: %s ", numberOrderAction, action[state[key]->numberOrderAction]);
        printf("(%d, %d, %d)", state[key]->state.a, state[key]->state.b, state[key]->state.c);
        numberOrderAction++;
        key--;
    }
    printf(" Goal state\n");
}

int main()
{
    State current;
    current.a = 10;
    current.b = 0;
    current.c = 0;
    node *tempNode = BFS(current);
    print_WaysToGetGoal(tempNode);
    free(tempNode);
    return 0;
}