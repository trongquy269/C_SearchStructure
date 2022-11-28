#include <iostream>
#include <queue>

using namespace std;

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

int findQueue (queue <node*> q, State state)
{
    while (!q.empty())
    {
        State tempState = q.front()->state;
        q.pop();
        if ((tempState.human == state.human) && (tempState.devil == state.devil) && (tempState.boat == state.boat))
        {
            return 1;
        }
    }
    return 0;
}

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
    queue <node*> open;
    queue <node*> close;

    node *root = new node;
    root->state = startState;
    root->parent = NULL;
    root->numberOrderAction = 0;
    open.push(root);
    while (!open.empty())
    {
        node *nodeTop = open.front();
        open.pop();
        close.push(nodeTop);

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
                    node *newNode = new node;
                    newNode->state = newState;
                    newNode->parent = nodeTop;
                    newNode->numberOrderAction = opt;
                    
                    open.push(newNode);
                }
            }
        }
    }
    return NULL;
}

void print_WaysToGetGoal(node* stateNode)
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
        cout << "\nHanh dong " << numberOrderAction << " : " << action[state[key]->numberOrderAction];
        cout << " (" << state[key]->state.human << " ," << state[key]->state.devil << " ," << state[key]->state.boat << ")";
        numberOrderAction++;
        key--;
    }
    cout << " Goal state\n";
}

int main()
{
    State current;
    current.human = numberOfPeople;
    current.devil = numberOfDevil;
    current.boat = 1;
    node *tempNode = BFS(current);
    print_WaysToGetGoal(tempNode);
    delete tempNode;
    return 0;
}