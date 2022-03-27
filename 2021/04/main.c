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

typedef u_int8_t U8;
typedef u_int16_t U16;

typedef struct BingoCell
{
    U8 value : 7;
    U8 crossed : 1;
} BingoCell;

typedef union BingoCounter
{
    struct
    {
        U16 i1 : 3;
        U16 i2 : 3;
        U16 i3 : 3;
        U16 i4 : 3;
        U16 i5 : 3;
        U16 won : 1;
    };
    U16 state;
} BingoCounter;

typedef struct BingoBoard
{
    BingoCounter rows;
    BingoCounter cols;
    BingoCell board[25];
} BingoBoard;

int draws_count = 0;
int draws[100] = {0};

int boards_count = 0;
BingoBoard *boards[100] = {0};

int parseDraw(char *line, int *draws)
{
    char *token, *next = NULL;
    int i = 0;
    while ((token = strtok_r(line, ",", &next)) != NULL)
    {
        draws[i] = atoi(token) & 0xff;
        i++;
        line = NULL;
    }
    return i++;
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

    bool first = true;
    int board_pos = 0;
    BingoBoard *cur = NULL;
    while ((getline(&line, &len, fp)) != -1)
    {
        // parse first line
        if (first)
        {
            draws_count = parseDraw(line, &draws[0]);
            first = false;
            continue;
        }
        // Allocate a new board on empty lines
        if (strcmp(line, "\n") == 0)
        {
            cur = (BingoBoard *)calloc(1, sizeof(BingoBoard));
            continue;
        }
        // Fill the board
        char *token, *next;
        while ((token = strtok_r(line, " ", &next)) != NULL)
        {
            cur->board[board_pos].value = atoi(token) & 0x7f;
            board_pos++;
            line = NULL;
        }
        // end  board
        if (board_pos == 25)
        {
            board_pos = 0;
            boards[boards_count] = cur;
            boards_count++;
        }
    }

    free(line);
    fclose(fp);
}

void incrementCounter(BingoCounter *cnt, int idx)
{
    // bitfields generics aka the state in the union
    // set the state and get the modified bit-field value
    // almost easier than haskell lenses
    U16 res = (cnt->state = cnt->state + (1 << (idx * 3))) >> (idx * 3) & 0x7;
    if (res == 5)
        cnt->won = 1;
}

char *reportScore(BingoBoard *bb, uint n)
{
    char *ret = calloc(64, sizeof(char));
    int sum = 0;
    for (int i = 0; i < 25; i++)
        if (bb->board[i].crossed == 0)
            sum += bb->board[i].value;

    snprintf(ret, 64, "sum: %d, last: %2u, score: %u", sum, n, sum * n);
    return ret;
}

void crossNumber(BingoBoard *bb, uint n)
{
    for (int p = 0; p < 25; p++)
        if (bb->board[p].value == n)
        {
            bb->board[p].crossed = 1;
            incrementCounter(&bb->rows, p / 5);
            incrementCounter(&bb->cols, p % 5);
            break;
        }
}

static inline bool hasWon(BingoBoard *bb)
{
    return bb->rows.won || bb->cols.won;
}

void simulateBingo()
{
    int winners = 0;
    BingoBoard *first, *last;
    uint first_i, last_i, first_n, last_n;

    for (int d = 0; d < draws_count; d++)
        for (int b = 0; b < boards_count; b++)
        {
            BingoBoard *bb = boards[b];
            // skip if already won
            if (hasWon(bb))
                continue;

            crossNumber(bb, draws[d]);

            if (hasWon(bb))
            {
                winners++;
                if (winners == 1)
                {
                    first = bb;
                    first_i = b;
                    first_n = draws[d];
                    continue;
                }
                else if (winners == boards_count)
                {
                    last = bb;
                    last_i = b;
                    last_n = draws[d];
                    goto imagine_using_goto_in_2022_lulz;
                }
            }
        }

imagine_using_goto_in_2022_lulz:
    printf("first winner: board[%2u] -> %s\n", first_i, reportScore(first, first_n));
    printf(" last winner: board[%2u] -> %s\n", last_i, reportScore(last, last_n));
}

int main()
{
    printf("** AoC 2021 - Day 04\n");

    parseInput();
    printf("parsed %u draws, %u boards\n", draws_count, boards_count);

    simulateBingo();

    exit(EXIT_SUCCESS);
}
