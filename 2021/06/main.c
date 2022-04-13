#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if 0
const char *INFILE = "input.test";
#else
const char *INFILE = "input";
#endif

typedef u_int64_t u8;

// contains the count of lantern fishes in each state (0..8)
#define STATES 9
u8 LSTATE[STATES] = {0};

void printState()
{
    for (int i = 0; i < STATES; i++)
        printf(" %lu ", LSTATE[i]);

    printf("\n");
}

void nextDay()
{
    u8 next = LSTATE[0];
    LSTATE[0] = LSTATE[1];
    LSTATE[1] = LSTATE[2];
    LSTATE[2] = LSTATE[3];
    LSTATE[3] = LSTATE[4];
    LSTATE[4] = LSTATE[5];
    LSTATE[5] = LSTATE[6];
    LSTATE[6] = LSTATE[7] + next;
    LSTATE[7] = LSTATE[8];
    LSTATE[8] = next;
}

u8 countFishes()
{
    u8 sum = 0;
    for (int i = 0; i < STATES; i++)
        sum += LSTATE[i];

    return sum;
}

void parseLine(char *line)
{
    char *tkn, *rest;
    while ((tkn = strtok_r(line, ",", &rest)) != NULL)
    {
        LSTATE[atoi(tkn)]++;
        line = NULL;
    }
}

void parseInput()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    if ((fp = fopen(INFILE, "r")) == NULL)
    {
        perror("uWu made a BuBu");
        exit(EXIT_FAILURE);
    }

    getline(&line, &len, fp);
    parseLine(line);

    free(line);
    fclose(fp);
}

int main()
{
    printf("** AoC 2021 - Day 06\n");

    parseInput();
    printState();

    int i = 0;
    for (; i < 18; i++)
        nextDay();

    printf("Count after  18 days: %lu\n", countFishes());

    for (; i < 80; i++)
        nextDay();

    printf("Count after  80 days: %lu\n", countFishes());

    for (; i < 256; i++)
        nextDay();

    printf("Count after 256 days: %lu\n", countFishes());

    exit(EXIT_SUCCESS);
}