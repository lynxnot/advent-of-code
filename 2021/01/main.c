#include <stdio.h>
#include <stdlib.h>

#if 0
const char *INFILE = "input.test";
#define MAX_MEASUREMENTS 10
#else
const char *INFILE = "input";
#define MAX_MEASUREMENTS 2000
#endif

int depths[MAX_MEASUREMENTS] = {0};

void parseMeasurements(const char *path)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    if ((fp = fopen(INFILE, "r")) == NULL)
    {
        perror("uWu made a BuBu");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (getline(&line, &len, fp) != -1)
    {
        depths[i] = atoi(line);
        i++;
    }

    free(line);
    fclose(fp);
}

int main()
{
    printf("** AoC 2021 - Day 01\n");

    parseMeasurements(INFILE);

    // part 1
    int increased = 0;
    for (int i = 0; i < MAX_MEASUREMENTS - 1; i++)
        if (depths[i + 1] > depths[i])
            increased++;

    printf("Depth has increased %d times.\n", increased);

    // part 2
    int sums[MAX_MEASUREMENTS - 2] = {0};
    for (int i = 0; i < MAX_MEASUREMENTS - 2; i++)
        sums[i] = depths[i] + depths[i + 1] + depths[i + 2];

    int trincreased = 0;
    for (int i = 0; i < MAX_MEASUREMENTS - 3; i++)
        if (sums[i + 1] > sums[i])
            trincreased++;

    printf("Tri-Depth has increased %d times.\n", trincreased);

    exit(EXIT_SUCCESS);
}
