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

typedef struct TrieNode
{
    struct TrieNode *childs[2];
    uint val[2];
    uint hi_c : 10;
    uint lo_c : 10;
    uint isLast : 1;
} TrieNode;

void printNode(TrieNode *node)
{
    printf("hi_c: %d, lo_c: %d, hi: %d, lo: %d, last: %d, c0: %p, c1: %p\n",
           node->hi_c, node->lo_c, node->val[1], node->val[0], node->isLast,
           (void *)&node->childs[0], (void *)&node->childs[1]);
}

void insertTrieVal(TrieNode *node, const uint val)
{
    for (int pos = BIT_COUNT - 1; pos >= 0; pos--)
    {
        int bit = (val >> pos) & 1;
        (bit == 0) ? node->lo_c++ : node->hi_c++;

        if (node->childs[bit] == NULL)
            node->childs[bit] = calloc(1, sizeof(TrieNode));

        if (pos != 0)
            node = node->childs[bit];
        else
        {
            node->val[val & 1] = val;
            node->isLast = 1;
        }
    }
}

uint selectOxy(TrieNode *node)
{
    for (int d = 0; d < BIT_COUNT - 1; d++)
        node = (node->hi_c >= node->lo_c) ? node->childs[1] : node->childs[0];

    return (node->hi_c >= node->lo_c) ? node->val[1] : node->val[0];
}

uint selectCarbonDiOxy(TrieNode *node)
{
    // this seems a bit messy
    while (!node->isLast)
    {
        if (node->lo_c == 0 || node->hi_c == 0)
        {
            node = (node->hi_c == 0) ? node->childs[0] : node->childs[1];
            continue;
        }
        node = (node->lo_c <= node->hi_c) ? node->childs[0] : node->childs[1];
    }

    return (node->hi_c == 0) ? node->val[0] : node->val[1];
}

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

    uint bit_counter[BIT_COUNT] = {0};
    int line_count = 0;
    TrieNode *root = calloc(1, sizeof(TrieNode));
    while ((getline(&line, &len, fp)) != -1)
    {
        line_count++;
        for (int pos = 0; pos < BIT_COUNT; pos++)
            if (line[pos] == '1')
                bit_counter[pos]++;

        // part 2: fill the trie
        uint val = strtoul(line, NULL, 2);
        insertTrieVal(root, val);
    }

    free(line);
    fclose(fp);

    // part 1
    int gamma = 0;
    int epsilon = 0;
    int p = BIT_COUNT - 1;
    int halfln = line_count / 2;
    for (int pos = 0; pos < BIT_COUNT; pos++)
    {
        if (bit_counter[pos] > halfln)
            gamma += 1 << p;

        p--;
    }

    epsilon = ~gamma & ((1 << BIT_COUNT) - 1);
    printf("gamma: %d - epsilon: %d - power: %d\n", gamma, epsilon, gamma * epsilon);

    // part 2
    uint oxy = selectOxy(root);
    uint co2 = selectCarbonDiOxy(root);
    printf("oxygen: %d, co2: %d, rating: %d\n", oxy, co2, oxy * co2);
}

int main()
{
    printf("** AoC 2021 - Day 03\n");
    solve();
    exit(EXIT_SUCCESS);
}