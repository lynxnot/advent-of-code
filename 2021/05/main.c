#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if 0
const char *INFILE = "input.test";
#define SEGS_COUNT 10
#define GRID_POINTS 10
#else
const char *INFILE = "input";
#define SEGS_COUNT 500
#define GRID_POINTS 999
#endif

typedef struct Point
{
    uint x;
    uint y;
} Point;

typedef struct Segment
{
    Point p1;
    Point p2;
} Segment;

static inline bool isHorizontal(Segment *seg)
{
    return seg->p1.y == seg->p2.y;
}

static inline bool isVertical(Segment *seg)
{
    return seg->p1.x == seg->p2.x;
}

void parseLine(Segment *s, char *line)
{
    char *tkn, *rest;
    uint coords[4];
    int i = 0;
    while ((tkn = strtok_r(line, ",-> ", &rest)) != NULL)
    {
        coords[i] = atoi(tkn);
        i++;
        line = NULL;
    }
    s->p1.x = coords[0];
    s->p1.y = coords[1];
    s->p2.x = coords[2];
    s->p2.y = coords[3];
}

void parseInput(Segment *segs)
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
    while ((getline(&line, &len, fp)) != -1)
    {
        parseLine(&segs[i], line);
        i++;
    }

    free(line);
    fclose(fp);
}

static inline uint min(uint a, uint b)
{
    return (a < b) ? a : b;
}

static inline uint max(uint a, uint b)
{
    return (a >= b) ? a : b;
}

int main()
{
    printf("** AoC 2021 - Day 05\n");

    Segment *segs = (Segment *)calloc(SEGS_COUNT, sizeof(Segment));
    uint grid[GRID_POINTS][GRID_POINTS] = {0};

    parseInput(segs);

    for (int i = 0; i < SEGS_COUNT; i++)
    {
        Segment *s = &segs[i];
        if (isHorizontal(s))
        {
            int start = min(s->p1.x, s->p2.x);
            int end = max(s->p1.x, s->p2.x);
            for (int x = start; x <= end; x++)
            {
                grid[x][s->p1.y]++;
            }
        }
        if (isVertical(s))
        {
            int start = min(s->p1.y, s->p2.y);
            int end = max(s->p1.y, s->p2.y);
            for (int y = start; y <= end; y++)
            {
                grid[s->p1.x][y]++;
            }
        }
    }

    int overlappers = 0;
    for (int y = 0; y < GRID_POINTS; y++)
    {
        for (int x = 0; x < GRID_POINTS; x++)
        {
            if (grid[x][y] >= 2)
                overlappers++;
        }
    }

    printf("Overlappers: %d\n", overlappers);

    free(segs);
    exit(EXIT_SUCCESS);
}