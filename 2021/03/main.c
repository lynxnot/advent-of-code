#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0
#define BIT_COUNT 5
const char *INFILE = "input.test";
#else
#define BIT_COUNT 12
const char *INFILE = "input";
#endif

void solve()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    if ((fp = fopen(INFILE, "r")) == NULL)
    {
        perror("uWu made a BuBu");
        exit(EXIT_FAILURE);
    }

    int bit_counter[BIT_COUNT] = {0};
    int line_count = 0;
    while ((getline(&line, &len, fp)) != -1)
    {
        line_count++;
        for (int bit = 0; bit < BIT_COUNT; bit++)
            if (line[bit] == '1')
                bit_counter[bit]++;
    }

    int gamma = 0;
    int epsilon = 0;
    int p = BIT_COUNT - 1;
    int halfln = line_count / 2;
    for (int bit = 0; bit < BIT_COUNT; bit++)
    {
        if (bit_counter[bit] > halfln)
            gamma += 1 << p;

        p--;
    }

    epsilon = ~gamma & ((1 << BIT_COUNT) - 1);
    printf("gamma: %d - epsilon: %d - power: %d\n", gamma, epsilon, gamma * epsilon);

    free(line);
    fclose(fp);
}

int main()
{
    printf("** AoC 2021 - Day 03\n");
    solve();
    exit(EXIT_SUCCESS);
}