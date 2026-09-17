#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    int from, to;
    char symbol;
} Transition;

typedef struct {
    int start, end;
} Fragment;

Transition t[MAX];
Fragment stack[MAX];
int nt = 0, state = 0, top = -1;

/* Add explicit concatenation */
void addDot(char *re, char *out) {
    int k = 0;

    for (int i = 0; re[i]; i++) {
        char c = re[i];
        out[k++] = c;

        if (c == '(' || c == '|')
            continue;

        if (re[i + 1] && re[i + 1] != ')' &&
            re[i + 1] != '|' && re[i + 1] != '*' &&
            re[i + 1] != '+')
            out[k++] = '.';
    }

    out[k] = '\0';
}

/* Operator precedence */
int prec(char c) {
    if (c == '|') return 1;
    if (c == '.') return 2;
    return 3;
}

/* Infix -> Postfix */
void postfix(char *in, char *out) {
    char op[MAX];
    int top = -1, k = 0;

    for (int i = 0; in[i]; i++) {
        char c = in[i];

        if (isalnum(c))
            out[k++] = c;

        else if (c == '(')
            op[++top] = c;

        else if (c == ')') {
            while (op[top] != '(')
                out[k++] = op[top--];
            top--;
        }

        else {
            while (top >= 0 && op[top] != '(' &&
                   prec(op[top]) >= prec(c))
                out[k++] = op[top--];

            op[++top] = c;
        }
    }

    while (top >= 0)
        out[k++] = op[top--];

    out[k] = '\0';
}

void add(int from, char symbol, int to) {
    t[nt++] = (Transition){from, to, symbol};
}

/* Thompson construction */
void thompson(char *re) {
    for (int i = 0; re[i]; i++) {
        char c = re[i];

        if (isalnum(c)) {
            int s = state++, e = state++;
            add(s, c, e);
            stack[++top] = (Fragment){s, e};
        }

        else if (c == '.') {
            Fragment b = stack[top--];
            Fragment a = stack[top--];

            add(a.end, 'e', b.start);
            stack[++top] = (Fragment){a.start, b.end};
        }

        else if (c == '|') {
            Fragment b = stack[top--];
            Fragment a = stack[top--];

            int s = state++, e = state++;

            add(s, 'e', a.start);
            add(s, 'e', b.start);
            add(a.end, 'e', e);
            add(b.end, 'e', e);

            stack[++top] = (Fragment){s, e};
        }

        else if (c == '*') {
            Fragment a = stack[top--];

            int s = state++, e = state++;

            add(s, 'e', a.start);
            add(s, 'e', e);
            add(a.end, 'e', a.start);
            add(a.end, 'e', e);

            stack[++top] = (Fragment){s, e};
        }

        else if (c == '+') {
            Fragment a = stack[top--];

            int s = state++, e = state++;

            add(s, 'e', a.start);
            add(a.end, 'e', a.start);
            add(a.end, 'e', e);

            stack[++top] = (Fragment){s, e};
        }
    }
}

int main() {
    char re[MAX], dotted[MAX], post[MAX];

    printf("Enter regular expression: ");
    scanf("%s", re);

    addDot(re, dotted);
    postfix(dotted, post);
    thompson(post);

    printf("\nWith concatenation: %s", dotted);
    printf("\nPostfix: %s\n", post);

    printf("\nNFA Transitions:\n");
    for (int i = 0; i < nt; i++)
        printf("q%d --%c--> q%d\n",
               t[i].from, t[i].symbol, t[i].to);

    printf("Initial state: q0\n");
    printf("Accepting state: q%d\n", stack[top].end);

    return 0;
}