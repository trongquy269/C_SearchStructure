#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 500

int tankCapacityA = 9;
int tankCapacityB = 4;
int goal = 6;

typedef struct
{
    int a;
    int b;
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
}

typedef struct
{
    node *data[MAX_LENGTH];
    int topIndex;
} stack;

void initStack (stack *s)
{
    s->topIndex = -1;
}

int isEmpty (stack s)
{
    return s.topIndex == -1;
}

int isFull (stack s)
{
    return s.topIndex == MAX_LENGTH - 1;
}

void push (stack *s, node *value)
{
    if (isFull(*s))
    {
        printf("Error! Stack is full\n");
    }
    else
    {
        s->topIndex++;
        s->data[s->topIndex] = value;
    }
}

node *pop (stack *s)
{
    if (isEmpty(*s))
    {
        printf("Error! Stack is empty\n");
    }
    else
    {
        node *data;
        data = s->data[s->topIndex];
        s->topIndex--;
        return data;
    }
}

node *top (stack s)
{
    if (isEmpty(s))
    {
        printf("Error! Stack is empty\n");
    }
    else
    {
        return s.data[s.topIndex];
    }
}

int findStack (stack s, State state)
{
    while (!isEmpty(s))
    {
        State tempState = pop(&s)->state;
        if ((tempState.a == state.a) && (tempState.b == state.b))
        {
            return 1;
        }
    }
    return 0;
}
// End: stack

int goalCheck (State current)
{
    if ((current.a == goal) || (current.b == goal))
    {
        return 1;
    }
    return 0;
}

int fullA (State current, State *water)
{
    if (current.a != tankCapacityA)
    {
        water->a = tankCapacityA;
        water->b = current.b;
        return 1;
    }
    return 0;
}

int fullB (State current, State *water)
{
    if (current.b != tankCapacityB)
    {
        water->b = tankCapacityB;
        water->a = current.a;
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
        return 1;
    }
    return 0;
}

int emptyA (State current, State *water)
{
    if (current.a != 0)
    {
        water->a = 0;
        water->b = current.b;
        return 1;
    }
    return 0;
}

int emptyB (State current, State *water)
{
    if (current.b != 0)
    {
        water->b = 0;
        water->a = current.a;
        return 1;
    }
    return 0;
}

int callOperator (State current, State *water, int opt)
{
    switch (opt)
    {
        case 1: return fullA(current, water);
        case 2: return fullB(current, water);
        case 3: return pourAB(current, water);
        case 4: return pourBA(current, water);
        case 5: return emptyA(current, water);
        case 6: return emptyB(current, water);
        default: printf("Error call operators\n");
        return 0;
    }
}

const char *action[] = {"Start state", "Pour full A", "Pour full B", "Pour from A to B", "Pour from B to A", "Pour empty A", "Pour empty B"};

node *DFS (State startState)
{
    stack open;
    stack close;
    initStack(&open);
    initStack(&close);

    node *root = (node *)malloc(sizeof(node));
    root->state = startState;
    root->parent = NULL;
    root->numberOrderAction = 0;
    push(&open, root);
    while (!isEmpty(open))
    {
        node *nodeTop = pop(&open);
        push(&close, nodeTop);

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
                if (!(findStack(open, newState)) && !(findStack(close, newState)))
                {
                    node *newNode = (node *)malloc(sizeof(node));
                    newNode->state = newState;
                    newNode->parent = nodeTop;
                    newNode->numberOrderAction = opt;
                    
                    push(&open, newNode);
                }
            }
        }
    }
    return NULL;
}

void print_WaysToGetGoal (node *stateNode)
{
    stack stackToPrint;
    initStack(&stackToPrint);

    while (stateNode != NULL)
    {
        push(&stackToPrint, stateNode);
        stateNode = stateNode->parent;
    }

    int numberOrderAction = 0;
    
    while (!isEmpty(stackToPrint))
    {
        printf("\nHanh dong %d: %s ", numberOrderAction, action[top(stackToPrint)->numberOrderAction]);
        node *tempNode = pop(&stackToPrint);
        printf("(%d, %d)", tempNode->state.a, tempNode->state.b);
        numberOrderAction++;
        free (tempNode);
    }
    printf(" Goal state\n");
}

int main()
{
    State current;
    emptyState(&current);
    node *tempNode = DFS(current);
    print_WaysToGetGoal(tempNode);
    free(tempNode);
    return 0;
}