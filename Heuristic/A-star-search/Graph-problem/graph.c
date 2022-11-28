#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 5
#define MAX_LENGTH 500

typedef struct
{
    int neighbor[MAX_VERTICES];
    int heuristic;
} Vertices;

typedef struct
{
    int numberVertices;
    Vertices vertices[MAX_VERTICES];
} Graph;

void initGraph (Graph *g, int numberVertices)
{
    g->numberVertices = numberVertices;
    for (int i = 0; i < numberVertices; i++)
    {
        for (int j = 0; j < numberVertices; j++)
        {
            g->vertices[i].neighbor[j] = 0;
            g->vertices[i].heuristic = 0;
        }
    }
}

const char name[] = {'A', 'B', 'C', 'D', 'G'};

typedef struct
{
    int vertex;
} State;

void printState (State state)
{
    printf("%c", name[state.vertex]);
}

int compareStates (State state1, State state2)
{
    return state1.vertex == state2.vertex;
}

typedef struct node
{
    State state;
    struct node *parent;
    int numberOrderAction;
    int f;
    int g;
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

node *findNode (list l, node *n, int *position)
{
    for (int i = 0; i <= l.size; i++)
    {
        if (compareStates(elementAt(l, i)->state, n->state))
        {
            *position = i;
            return elementAt(l, i);
        }
    }
    return NULL;
}

void sortList (list *l)
{
    for (int i = 0; i <= l->size - 1; i++)
    {
        for (int j = i + 1; j <= l->size; j++)
        {
            if (l->data[i]->f > l->data[j]->f)
            {
                node *temp = l->data[i];
                l->data[i] = l->data[j];
                l->data[j] = temp;
            }
        }
    }
}

int goalCheck (State current, State goal)
{
    return compareStates(current, goal);
}

node *AStar (Graph G, State startState, State goal)
{
    list open;
    list close;
    initList(&open);
    initList(&close);

    node *root = (node*)malloc(sizeof(node));
    root->state = startState;
    root->parent = NULL;
    root->numberOrderAction = 0;
    root->g = 0;
    root->heuristic = G.vertices[startState.vertex].heuristic;
    root->f = root->g + root->heuristic;
    push(&open, root);

    while (!isEmpty(open))
    {
        node *frontNode = elementAt(open, 0);
        removeList(&open, 0);
        push(&close, frontNode);

        if (goalCheck(frontNode->state, goal))
        {
            return frontNode;
        }

        for (int opt = 0; opt < MAX_VERTICES; opt++)
        {
            if (G.vertices[frontNode->state.vertex].neighbor[opt] > 0)
            {
                node *newNode = (node*)malloc(sizeof(node));
                newNode->state.vertex = opt;
                newNode->parent = frontNode;
                newNode->g = frontNode->g + G.vertices[frontNode->state.vertex].neighbor[opt];
                newNode->heuristic = G.vertices[opt].heuristic;
                newNode->f = newNode->g + newNode->heuristic;

                int positionOpen;
                int positionClose;
                node *nodeFoundOpen = findNode(open, newNode, &positionOpen);
                node *nodeFoundClose = findNode(close, newNode, &positionClose);

                if ((nodeFoundOpen == NULL) && (nodeFoundClose == NULL))
                {
                    push(&open, newNode);
                }
                else if ((nodeFoundOpen != NULL) && (newNode->g < nodeFoundOpen->g))
                {
                    removeList(&open, positionOpen);
                    push(&open, newNode);
                }
                else if ((nodeFoundClose != NULL) && (newNode->g) < nodeFoundClose->g)
                {
                    removeList(&close, positionClose);
                    push(&open, newNode);
                }
            }
        }
        sortList(&open);
    }
    return NULL;
}

void print_WaysToGetGoal (node *goalNode)
{
    if (goalNode == NULL)
    {
        printf("Can't find goal\n");
    }
    else
    {
        list printList;
        initList(&printList);

        while (goalNode != NULL)
        {
            push(&printList, goalNode);
            goalNode = goalNode->parent;
        }

        for (int i = printList.size; i >= 0; i--)
        {
            printState(elementAt(printList, i)->state);
            if (i != 0)
            {
                printf(" -> ");
            }
        }
        printf("\n");
    }
}

int main()
{
    Graph G;
    freopen("data.txt", "r", stdin);
    initGraph(&G, MAX_VERTICES);
    
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        int value;
        scanf("%d", &value);
        G.vertices[i].heuristic = value;

        for (int j = 0; j < MAX_VERTICES; j++)
        {
            scanf("%d", &value);
            G.vertices[i].neighbor[j] = value;
        }
    }

    State startState, goal;
    startState.vertex = 0;
    goal.vertex = 4;

    node *result = AStar(G, startState, goal);
    print_WaysToGetGoal(result);

    return 0;
}