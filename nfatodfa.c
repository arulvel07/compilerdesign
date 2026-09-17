#include <stdio.h>
#include <string.h>

#define MAX 20
#define MAXDFA 100

int n, m;  //no of nfa states, m alpbahets
char alphabet[MAX];

int trans[MAX][MAX][MAX];
int eps[MAX][MAX];

int dfa[MAXDFA][MAX];
int dfaCount = 0;
int dfaTrans[MAXDFA][MAX];

int final[MAX];

/* Find epsilon closure of a set */
void epsilonClosure(int set[])
{
    int changed = 1;

    while (changed)
    {
        changed = 0;

        for (int i = 0; i < n; i++)
        {
            if (set[i])
            {
                for (int j = 0; j < n; j++)
                {
                    if (eps[i][j] && !set[j])
                    {
                        set[j] = 1;
                        changed = 1;
                    }
                }
            }
        }
    }
}

/* Move from a set using a symbol */
void move(int set[], int next[], int symbol)
{
    for (int i = 0; i < n; i++)
    {
        if (set[i])
        {
            for (int j = 0; j < n; j++)
            {
                if (trans[i][symbol][j])
                    next[j] = 1;
            }
        }
    }
}

/* Check whether subset already exists */
int findState(int set[])
{
    for (int i = 0; i < dfaCount; i++)
    {
        int same = 1;

        for (int j = 0; j < n; j++)
        {
            if (dfa[i][j] != set[j])
            {
                same = 0;
                break;
            }
        }

        if (same)
            return i;
    }

    return -1;
}

/* Add a new DFA state */
int addState(int set[])
{
    int id = dfaCount++;

    for (int i = 0; i < n; i++)
        dfa[id][i] = set[i];

    return id;
}

/* Subset construction */
void subsetConstruction(int start)
{
    int set[MAX] = {0};

    set[start] = 1;
    epsilonClosure(set);

    addState(set);

    for (int current = 0; current < dfaCount; current++)
    {
        for (int s = 0; s < m; s++)
        {
            int next[MAX] = {0};

            move(dfa[current], next, s);
            epsilonClosure(next);

            int id = findState(next);

            if (id == -1)
                id = addState(next);

            dfaTrans[current][s] = id;
        }
    }
}

/* Print DFA */
void printDFA()
{
    printf("\nDFA TRANSITION TABLE\n");

    for (int i = 0; i < dfaCount; i++)
    {
        printf("D%d = { ", i);

        for (int j = 0; j < n; j++)
        {
            if (dfa[i][j])
                printf("%d ", j);
        }

        printf("}\n");

        for (int s = 0; s < m; s++)
            printf("  on %c -> D%d\n",
                   alphabet[s], dfaTrans[i][s]);
    }

    printf("\nDFA Final States: ");

    for (int i = 0; i < dfaCount; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (dfa[i][j] && final[j])
            {
                printf("D%d ", i);
                break;
            }
        }
    }

    printf("\n");
}

int main()
{
    int start, nf, f;

    printf("Number of NFA states: ");
    scanf("%d", &n);

    printf("Number of alphabet symbols: ");
    scanf("%d", &m);

    printf("Alphabet: ");
    for (int i = 0; i < m; i++)
        scanf(" %c", &alphabet[i]);

    printf("Number of transitions: ");
    scanf("%d", &nf);

    printf("Enter transitions (from symbol to):\n");

    for (int i = 0; i < nf; i++)
    {
        int from, to;
        char ch;

        scanf("%d %c %d", &from, &ch, &to);

        if (ch == 'e')
        {
            eps[from][to] = 1;
        }
        else
        {
            for (int j = 0; j < m; j++)
            {
                if (alphabet[j] == ch)
                    trans[from][j][to] = 1;
            }
        }
    }

    printf("Initial state: ");
    scanf("%d", &start);

    printf("Number of final states: ");
    scanf("%d", &f);

    printf("Final states: ");
    for (int i = 0; i < f; i++)
    {
        int x;
        scanf("%d", &x);
        final[x] = 1;
    }

    subsetConstruction(start);
    printDFA();

    return 0;
}   