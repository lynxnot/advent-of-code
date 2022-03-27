#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

#define TEST 0

#if TEST
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

void printCounter(BingoCounter *cnt)
{
    printf("i1: %u, i2: %u, i3: %u, i4: %u, i5: %u, won: %u\n",
           cnt->i1, cnt->i2, cnt->i3, cnt->i4, cnt->i5, cnt->won);
}

char *vtRenderCell(BingoCell c)
{
    char *ret = malloc(32 * sizeof(char));
    if (c.crossed)
        snprintf(ret, 32, "\033[35;1m%3u\033[0m", c.value);
    else
        snprintf(ret, 32, "%3u", c.value);

    return ret;
}

void printBoard(BingoBoard *bb)
{
    printf("--------------------\n");
    for (int j = 0; j < 5; j++)
    {
        printf("%s %s %s %s %s\n",
               vtRenderCell(bb->board[0 + j * 5]),
               vtRenderCell(bb->board[1 + j * 5]),
               vtRenderCell(bb->board[2 + j * 5]),
               vtRenderCell(bb->board[3 + j * 5]),
               vtRenderCell(bb->board[4 + j * 5]));
    }
    printf("\n");
}

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

void calculateScore(BingoBoard *bb, int b, uint n)
{
    int sum = 0;
    for (int i = 0; i < 25; i++)
        if (bb->board[i].crossed == 0)
            sum += bb->board[i].value;

    printf("winner: board[%d], sum: %d, last: %u, score: %u\n", b, sum, n, sum * n);
}

bool crossBoards(uint n)
{
    for (int b = 0; b < boards_count; b++)
    {
        BingoBoard *bb = boards[b];
        for (int p = 0; p < 25; p++)
        {
            if (bb->board[p].value == n)
            {
                bb->board[p].crossed = 1;
                incrementCounter(&bb->rows, p / 5);
                incrementCounter(&bb->cols, p % 5);
                if (bb->rows.won || bb->cols.won)
                {
                    calculateScore(bb, b, n);
                    return true;
                }
                break;
            }
        }
    }
    return false;
}

int main()
{
    // mess with the terminal
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // disable waiting for '\n' and don't echo input
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("\033[0;0H\033[2J");
    printf("** AoC 2021 - Day 04\n");

    parseInput();
    printf("parsed %u draws, %u boards\n", draws_count, boards_count);

    bool last = false;
    int draw = -1;
    while (!last)
    {
        printf("\033[3;0H");

        printBoard(boards[60]);
        printBoard(boards[61]);
        printBoard(boards[62]);

        if (draw == -1)
            printf("\n");
        else
            printf("number drawn: %u\n", draws[draw]);

        printf("press <space> for next number, <q> to quit\n");

        int c = getchar();
        switch (c)
        {
        case ' ':
            draw++;
            bool win = crossBoards(draws[draw]);
            if (win || draw == draws_count)
                last = true;
            break;

        case 'q':
            last = true;
            break;

        default:
            break;
        }
    }

    // unmess the terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    exit(EXIT_SUCCESS);
}
