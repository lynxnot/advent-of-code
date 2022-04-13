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
    uint grid_p1[GRID_POINTS][GRID_POINTS] = {0};
    uint grid_p2[GRID_POINTS][GRID_POINTS] = {0};

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
                grid_p1[x][s->p1.y]++;
                grid_p2[x][s->p1.y]++;
            }
        }
        else if (isVertical(s))
        {
            int start = min(s->p1.y, s->p2.y);
            int end = max(s->p1.y, s->p2.y);
            for (int y = start; y <= end; y++)
            {
                grid_p1[s->p1.x][y]++;
                grid_p2[s->p1.x][y]++;
            }
        }
        else
        {
            int xdir = (s->p1.x < s->p2.x) ? 1 : -1;
            int ydir = (s->p1.y < s->p2.y) ? 1 : -1;

            for (int x = s->p1.x, y = s->p1.y;;)
            {
                // printf("x: %u, y: %u\n", x, y);
                grid_p2[x][y]++;

                if (x == s->p2.x)
                    break;

                x += xdir;
                y += ydir;
            }
        }
    }

    int overlappers_p1 = 0;
    int overlappers_p2 = 0;
    for (int y = 0; y < GRID_POINTS; y++)
    {
        for (int x = 0; x < GRID_POINTS; x++)
        {
            if (grid_p1[x][y] >= 2)
            {
                overlappers_p1++;
            }
            if (grid_p2[x][y] >= 2)
            {
                overlappers_p2++;
            }
        }
    }

    printf("Overlappers p1: %d\n", overlappers_p1);
    printf("Overlappers p2: %d\n", overlappers_p2);

    free(segs);
    exit(EXIT_SUCCESS);
}