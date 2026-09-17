#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 1000

const char *keywords[] = {
    "int", "if", "else", "float", "void",
    "double", "while", "char", "for", "return"
};

const int keywords_count =
    sizeof(keywords) / sizeof(keywords[0]);

int is_keyword(const char *str)
{
    for (int i = 0; i < keywords_count; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int is_operator_char(char c)
{
    return c == '+' || c == '-' || c == '*' ||
           c == '/' || c == '=' || c == '<' ||
           c == '>' || c == '|' || c == '!' ||
           c == '&' || c == '%';
}

int is_punctuation_char(char c)
{
    return c == '{' || c == '}' || c == ';' ||
           c == ',' || c == '(' || c == ')' ||
           c == '[' || c == ']';
}

int main()
{
    char c;
    char token[MAX_TOKEN_LEN];
    int idx = 0;

    printf("Enter your code (Ctrl+D to end):\n");

    while ((c = getchar()) != EOF)
    {
        /* Ignore whitespace */
        if (isspace(c))
            continue;

        /* Keyword / Identifier */
        if (isalpha(c) || c == '_')
        {
            idx = 0;
            token[idx++] = c;

            while ((c = getchar()) != EOF &&
                   (isalnum(c) || c == '_'))
            {
                if (idx < MAX_TOKEN_LEN - 1)
                    token[idx++] = c;
            }

            token[idx] = '\0';

            if (c != EOF)
                ungetc(c, stdin);

            if (is_keyword(token))
                printf("Keyword      : %s\n", token);
            else
                printf("Identifier   : %s\n", token);

            continue;
        }

        /* Integer literal */
        if (isdigit(c))
        {
            idx = 0;
            token[idx++] = c;

            while ((c = getchar()) != EOF && isdigit(c))
            {
                if (idx < MAX_TOKEN_LEN - 1)
                    token[idx++] = c;
            }

            token[idx] = '\0';

            if (c != EOF)
                ungetc(c, stdin);

            printf("Literal      : %s\n", token);
            continue;
        }

        /* Character literal */
        if (c == '\'')
        {
            idx = 0;
            token[idx++] = c;

            char next = getchar();

            if (next == EOF)
                break;

            token[idx++] = next;

            char closing = getchar();

            if (closing == EOF)
                break;

            token[idx++] = closing;
            token[idx] = '\0';

            if (closing == '\'')
                printf("Literal      : %s\n", token);
            else
                printf("Unknown      : %s\n", token);

            continue;
        }

        if (c == '"')
        {
                idx = 0;
                token[idx++] = c;

                while ((c = getchar()) != EOF && c != '"')
                {
                    if (idx < MAX_TOKEN_LEN - 1)
                        token[idx++] = c;
                }

                if (c == '"')
                    token[idx++] = c;

                token[idx] = '\0';

                printf("Literal      : %s\n", token);
                continue;
        }
        /* Operators */
        if (is_operator_char(c))
        {
            idx = 0;
            token[idx++] = c;

            char next = getchar();

            if (next != EOF)
            {
                if ((c == '=' && next == '=') ||
                    (c == '!' && next == '=') ||
                    (c == '>' && next == '=') ||
                    (c == '<' && next == '=') ||
                    (c == '&' && next == '&') ||
                    (c == '|' && next == '|') ||
                    (c == '+' && (next == '+' || next == '=')) ||
                    (c == '-' && (next == '-' || next == '=')) ||
                    (c == '*' && next == '=') ||
                    (c == '/' && next == '='))
                {
                    token[idx++] = next;
                }
                else
                {
                    ungetc(next, stdin);
                }
            }

            token[idx] = '\0';
            printf("Operator     : %s\n", token);
            continue;
        }

        /* Punctuation */
        if (is_punctuation_char(c))
        {
            printf("Punctuation  : %c\n", c);
            continue;
        }

        /* Unknown character */
        printf("Unknown      : %c\n", c);
    }

    return 0;
}