#include <stdio.h>

#define MAX 20

int n, m;
char alphabet[MAX];

int trans[MAX][MAX][MAX];
int eps[MAX][MAX];

int newTrans[MAX][MAX][MAX];
int final[MAX];

void epsilonClosure(int state, int closure[])
{
    closure[state] = 1;

    for (int i = 0; i < n; i++)
    {
        if (closure[i])
        {
            for (int j = 0; j < n; j++)
            {
                if (eps[i][j])
                    closure[j] = 1;
            }
        }
    }
}

void convert()
{
    for (int state = 0; state < n; state++)
    {
        int closure[MAX] = {0};

        epsilonClosure(state, closure);

        for (int s = 0; s < m; s++)
        {
            int next[MAX] = {0};

            for (int i = 0; i < n; i++)
            {
                if (closure[i])
                {
                    for (int j = 0; j < n; j++)
                    {
                        if (trans[i][s][j])
                            next[j] = 1;
                    }
                }
            }

            int result[MAX] = {0};

            for (int i = 0; i < n; i++)
            {
                if (next[i])
                {
                    int temp[MAX] = {0};
                    epsilonClosure(i, temp);

                    for (int j = 0; j < n; j++)
                        if (temp[j])
                            result[j] = 1;
                }
            }

            for (int j = 0; j < n; j++)
                newTrans[state][s][j] = result[j];
        }
    }
}

void printNFA()
{
    printf("\nNFA TRANSITION TABLE\n");

    for (int i = 0; i < n; i++)
    {
        for (int s = 0; s < m; s++)
        {
            printf("q%d --%c--> { ", i, alphabet[s]);

            for (int j = 0; j < n; j++)
            {
                if (newTrans[i][s][j])
                    printf("q%d ", j);
            }

            printf("}\n");
        }
    }

    printf("\nFinal States: ");
    for (int i = 0; i < n; i++)
    {
        int closure[MAX] = {0};
        epsilonClosure(i, closure);

        for (int j = 0; j < n; j++)
        {
            if (closure[j] && final[j])
            {
                printf("q%d ", i);
                break;
            }
        }
    }

    printf("\n");
}

int main()
{
    int t, start, f;

    printf("Number of states: ");
    scanf("%d", &n);

    printf("Number of alphabet symbols: ");
    scanf("%d", &m);

    printf("Alphabet: ");
    for (int i = 0; i < m; i++)
        scanf(" %c", &alphabet[i]);

    printf("Number of transitions: ");
    scanf("%d", &t);

    printf("Enter transitions (from symbol to):\n");

    for (int i = 0; i < t; i++)
    {
        int from, to;
        char ch;

        scanf("%d %c %d", &from, &ch, &to);

        if (ch == 'e')
            eps[from][to] = 1;
        else
        {
            for (int s = 0; s < m; s++)
            {
                if (alphabet[s] == ch)
                    trans[from][s][to] = 1;
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

    convert();
    printNFA();

    return 0;
}