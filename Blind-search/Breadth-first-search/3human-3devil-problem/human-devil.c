#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 500

int numberOfPeople = 3;
int numberOfDevil = 3;

typedef struct
{
    int human;
    int devil;
    int boat;
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
        State tempState = deQueue(&q)->state;
        if ((tempState.human == state.human) && (tempState.devil == state.devil) && (tempState.boat == state.boat))
        {
            return 1;
        }
    }
    return 0;
}
// End: queue

int goalCheck (State current)
{
    if ((current.human == 0) && (current.devil == 0) && (current.boat == 1))
    {
        return 1;
    }
    return 0;
}

int move1Human (State current, State *newState)
{
    if ((current.human != 0))
    {
        if (current.boat == 1)
        {
            newState->human = current.human - 1;
            if (current.devil <= newState->human)
            {
                newState->devil = current.devil;
                newState->boat = 2;
                return 1;
            }
            return 0;
        }
        else
        {
            if (current.human < numberOfPeople)
            {
                newState->human = current.human + 1;
                if (((numberOfDevil - current.devil) >= (numberOfPeople - newState->human)) || (newState->human == 0))
                {
                    newState->devil = current.devil;     
                    return 1;      
                }
            }
            newState->boat = 1;
            return 0;
        }
    }
    return 0;
}

int move1Devil (State current, State *newState)
{
    if ((current.devil != 0))
    {
        if (current.boat == 1)
        {
            newState->devil = current.devil - 1;
            if (newState->devil <= current.human)
            {
                newState->human = current.human;
                newState->boat = 2;
                return 1;
            }
            return 0;
        }
        else
        {
            if (current.devil < numberOfDevil)
            {
                newState->devil = current.devil + 1;
                if (((numberOfDevil - newState->devil) >= (numberOfPeople - current.human)) || (current.human == 0))
                {
                    newState->human = current.human;     
                    return 1;      
                }
            }
            newState->boat = 1;
            return 0;
        }
    }
    return 0;
}

int move2Human (State current, State *newState)
{
    if ((current.human > 1))
    {
        if (current.boat == 1)
        {
            newState->human = current.human - 2;
            if (current.devil <= newState->human)
            {
                newState->devil = current.devil;
                newState->boat = 2;
                return 1;
            }
            return 0;
        }
        else
        {
            if (current.human < numberOfPeople - 1)
            {
                newState->human = current.human + 2;
                if (((numberOfDevil - current.devil) >= (numberOfPeople - newState->human)) || (newState->human == 0))
                {
                    newState->devil = current.devil;     
                    return 1;      
                }
            }
            newState->boat = 1;
            return 0;
        }
    }
    return 0;
}

int move2Devil (State current, State *newState)
{
    if ((current.devil > 1))
    {
        if (current.boat == 1)
        {
            newState->devil = current.devil - 2;
            if (newState->devil <= current.human)
            {
                newState->human = current.human;
                newState->boat = 2;
                return 1;
            }
            return 0;
        }
        else
        {
            if (current.devil < numberOfDevil - 1)
            {
                newState->devil = current.devil + 2;
                if (((numberOfDevil - newState->devil) >= (numberOfPeople - current.human)) || (current.human == 0))
                {
                    newState->human = current.human;     
                    return 1;      
                }
            }
            newState->boat = 1;
            return 0;
        }
    }
    return 0;
}

int move1Human1Devil (State current, State *newState)
{
    if ((current.human != 0) && (current.devil != 0))
    {
        if (current.boat == 1)
        {
            newState->human = current.human - 1;
            newState->devil = current.devil - 1;
            if (newState->devil <= newState->human)
            {
                newState->boat = 2;
                return 1;
            }
            return 0;
        }
        else
        {
            if ((current.human < numberOfPeople) && (current.devil < numberOfDevil))
            {
                newState->human = current.human + 1;
                newState->devil = current.devil + 1;
                if (((numberOfDevil - newState->devil) >= (numberOfPeople - newState->human)) || (newState->human == 0))
                {   
                    return 1;
                }
            }
            newState->boat = 1;
            return 0;
        }
    }
    return 0;
}

int moveNoThing (State current, State *newState)
{
    if (current.boat == 2)
    {
        if (((numberOfDevil - current.devil) >= (numberOfPeople - current.human)) || (current.human == 0))
        {   
            newState->human = current.human;
            newState->devil = current.devil;
            newState->boat = 1;
            return 1;
        }
    }
    return 0;
}

int callOperator (State current, State *newState, int opt)
{
    switch (opt)
    {
        case 1: return move1Human(current, newState);
        case 2: return move1Devil(current, newState);
        case 3: return move2Human(current, newState);
        case 4: return move2Devil(current, newState);
        case 5: return move1Human1Devil(current, newState);
        case 6: return moveNoThing(current, newState);
        default: printf("Error call operators\n");
        return 0;
    }
}

const char *action[] = {"Start state", "Move 1 human", "Move 1 devil", "Move 2 human", "Move 2 devil", "Move 1 human 1 devil", "Move no thing"};

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
        printf("(%d, %d, %d)", state[key]->state.human, state[key]->state.devil, state[key]->state.boat);
        numberOrderAction++;
        key--;
    }
    printf(" Goal state\n");
}

int main()
{
    State current;
    current.human = numberOfPeople;
    current.devil = numberOfDevil;
    current.boat = 1;
    node *tempNode = BFS(current);
    print_WaysToGetGoal(tempNode);
    free(tempNode);
    return 0;
}