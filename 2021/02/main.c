#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0
const char *INFILE = "input.test";
#else
const char *INFILE = "input";
#endif

void solve()
{
    FILE *fp;
    char *line = NULL;
    char *space = NULL;
    size_t len = 0;
    ssize_t read;

    if ((fp = fopen(INFILE, "r")) == NULL)
    {
        perror("uWu made a BuBu");
        exit(EXIT_FAILURE);
    }

    int pos = 0;
    int d1 = 0;
    int d2 = 0;
    int aim = 0;
    int n = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        space = strchr(line, ' ');
        n = atoi(space);
        switch (line[0])
        {
        case 'f':
            pos += n;
            d2 += (aim * n);
            break;

        case 'd':
            d1 = aim += n;
            break;

        case 'u':
            d1 = aim -= n;
            break;

        default:
            printf("Welp!\n");
            exit(EXIT_FAILURE);
            break;
        }
    }

    printf("Position: %d - Depth: %d - m1: %d\n", pos, d1, pos * d1);
    printf("Aim: %d - Depth2: %d - m2: %d\n", aim, d2, pos * d2);

    free(line);
    fclose(fp);
}

int main()
{
    printf("** AoC 2021 - Day 02\n");
    solve();
    exit(EXIT_SUCCESS);
}