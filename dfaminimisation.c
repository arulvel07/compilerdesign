#include <stdio.h>

#define MAX 20

int main() {
    int n, m, i, j, k;
    int trans[MAX][MAX], final[MAX] = {0};
    int marked[MAX][MAX] = {0};

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter number of symbols: ");
    scanf("%d", &m);

    printf("Enter transition table:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &trans[i][j]);

    int nf;
    printf("Enter number of final states: ");
    scanf("%d", &nf);

    printf("Enter final states: ");
    for (i = 0; i < nf; i++) {
        int x;
        scanf("%d", &x);
        final[x] = 1;
    }

    /* Step 1: Mark final/non-final pairs */
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (final[i] != final[j])
                marked[i][j] = 1;

    /* Step 2: Mark distinguishable pairs */
    int changed = 1;

    while (changed) {
        changed = 0;

        for (i = 0; i < n; i++) {
            for (j = i + 1; j < n; j++) {

                if (marked[i][j])
                    continue;

                for (k = 0; k < m; k++) {
                    int a = trans[i][k];
                    int b = trans[j][k];

                    if (a > b) {
                        int temp = a;
                        a = b;
                        b = temp;
                    }

                    if (marked[a][b]) {
                        marked[i][j] = 1;
                        changed = 1;
                        break;
                    }
                }
            }
        }
    }

    /* Step 3: Print equivalent states */
    printf("\nEquivalent states:\n");

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (!marked[i][j])
                printf("q%d = q%d\n", i, j);

    return 0;
}