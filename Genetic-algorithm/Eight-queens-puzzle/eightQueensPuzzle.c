#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ROW 8
#define COL 8
#define CROSS_OVER_LENGTH 3
#define NUM_OF_IND 4

typedef struct
{
    int Gene[COL];
    int Fitness;
} Chromosome;

typedef struct
{
    Chromosome individual[NUM_OF_IND];
} Population;

int fitnessCalculation(Chromosome individual)
{
    int result = 0;
    int fitness;
    for (int i = 0; i < COL - 1; i++)
    {
        fitness = COL - (i + 1);

        // Kiem tra theo hang
        for (int j = i + 1; j < COL; j++)
        {
            if (individual.Gene[i] == individual.Gene[j])
            {
                fitness--;
            }
        }

        // Kiem tra theo duong cheo
        for (int j = 1; j < COL; j++)
        {
            if (i + j < COL)
            {
                // Down
                if (individual.Gene[i] + j < COL)
                {
                    if (individual.Gene[i] + j == individual.Gene[i + j])
                    {
                        fitness--;
                    }
                }
                // Up
                if (individual.Gene[i] - j >= 0)
                {
                    if (individual.Gene[i] - j == individual.Gene[i + j])
                    {
                        fitness--;
                    }
                }
            }
        }
        result += fitness;
    }
    return result;
}

void crossOver(Chromosome *individualA, Chromosome *individualB, int firstIndex)
{
    if (COL - firstIndex >= CROSS_OVER_LENGTH)
    {
        for (int i = firstIndex; i < CROSS_OVER_LENGTH; i++)
        {
            int temp = individualA->Gene[i];
            individualA->Gene[i] = individualB->Gene[i];
            individualB->Gene[i] = temp;
        }
    }
    else
    {
        for (int i = firstIndex; i > firstIndex - CROSS_OVER_LENGTH; i--)
        {
            int temp = individualA->Gene[i];
            individualA->Gene[i] = individualB->Gene[i];
            individualB->Gene[i] = temp;
        }
    }
}

void mutation(Chromosome *individual)
{
    int mutationIndex = (rand() % (COL - 0)) + 0;
    int mutationIndexValue = (rand() % (COL - 0)) + 0;
    while (mutationIndexValue == individual->Gene[mutationIndex]) {
        mutationIndexValue = (rand() % (COL - 0)) + 0;
    }
    individual->Gene[mutationIndex] = mutationIndexValue;
}

void selection(Population *eQueens)
{
    for (int i = 0; i < NUM_OF_IND - 1; i++)
    {
        for (int j = i + 1; j < NUM_OF_IND; j++)
        {
            if (eQueens->individual[i].Fitness < eQueens->individual[j].Fitness)
            {
                Chromosome temp = eQueens->individual[i];
                eQueens->individual[i] = eQueens->individual[j];
                eQueens->individual[j] = temp;
            }
        }
    }
}

void readData(Population *eQueens)
{
    freopen("data.txt", "r", stdin);
    for (int i = 0; i < NUM_OF_IND; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            scanf("%d", &eQueens->individual[i].Gene[j]);
        }
        eQueens->individual[i].Fitness = fitnessCalculation(eQueens->individual[i]);
    }
}

void printTable(Chromosome individual)
{
    for (int i = 0; i < ROW; i++)
    {
        if (i == 0)
        {
            printf("|-------------------------------|\n");
        }
        for (int j = 0; j < COL; j++)
        {
            if (i == individual.Gene[j])
            {
                printf("| 1 ");
                if (j == COL - 1)
                {
                    printf("|\n");
                }
            }
            else
            {
                printf("|   ");
                if (j == COL - 1)
                {
                    printf("|\n");
                }
            }
        }
        printf("|-------------------------------|\n");
    }
}

int geneticTimes = 0;

void geneticAlgorithm(Population *eQueens)
{
    while (1)
    {
        geneticTimes++;

        // Fitness calculation
        for (int i = 0; i < NUM_OF_IND; i++)
        {
            eQueens->individual[i].Fitness = fitnessCalculation(eQueens->individual[i]);
        }

        // Stop?
        if (eQueens->individual[0].Fitness == 28)
        {
            return;
        }

        // Selection
        selection(eQueens);

        // Cross-over
        int check = 1;
        for (int i = 0; i < NUM_OF_IND; i += 2)
        {
            if (check == 1)
            {
                crossOver(&eQueens->individual[i], &eQueens->individual[i + 1], 0);
                check = 2;
            }
            else
            {
                crossOver(&eQueens->individual[i], &eQueens->individual[i + 1], COL - 1);
                check = 1;
            }
        }

        // Mutation
        for (int i = 0; i < NUM_OF_IND; i++)
        {
            mutation(&eQueens->individual[i]);
        }
    }
}

int main()
{
    srand(time(NULL));
    Population eQueens;
    readData(&eQueens);
    geneticAlgorithm(&eQueens);
    printf("The best individual: ");
    for (int i = 0; i < COL; i++)
    {
        printf("%d ", eQueens.individual[0].Gene[i]);
    }
    printf("\nFitness: %d\n", eQueens.individual[0].Fitness);
    printf("Genetic times: %d\n", geneticTimes);
    printTable(eQueens.individual[0]);
    return 0;
}