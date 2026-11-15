#include "istack.h"

#include <stdio.h>

static int closing_for(int opening)
{
    switch (opening) {
    case '(':
        return ')';
    case '[':
        return ']';
    case '{':
        return '}';
    default:
        return 0;
    }
}

static int check_balance(istack_t *stack)
{
    size_t line = 1;
    size_t column = 1;
    size_t pairs = 0;
    size_t deepest = 0;
    int ch;

    while ((ch = getchar()) != EOF) {
        if (closing_for(ch) != 0) {
            if (istack_push(stack, ch) == -1) {
                fputs("error: unable to grow the stack\n", stderr);
                return 2;
            }

            size_t depth = istack_count(stack);
            if (depth > deepest) {
                deepest = depth;
            }
        } else if (ch == ')' || ch == ']' || ch == '}') {
            int opening;
            if (istack_pop(stack, &opening) == -1) {
                printf("line %zu, column %zu: found '%c', nothing open\n",
                       line, column, ch);
                return 1;
            }

            int expected = closing_for(opening);
            if (ch != expected) {
                printf("line %zu, column %zu: found '%c', expected '%c'\n",
                       line, column, ch, expected);
                return 1;
            }

            pairs++;
        }

        if (ch == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }

    if (ferror(stdin)) {
        fputs("error: unable to read input\n", stderr);
        return 2;
    }

    int innermost;
    if (istack_peek(stack, &innermost) == 0) {
        printf("end of input: %zu still open, innermost '%c'\n",
               istack_count(stack), innermost);
        return 1;
    }

    printf("balanced: %zu pairs, deepest %zu\n", pairs, deepest);
    return 0;
}

int main(void)
{
    istack_t *stack = istack_create();
    if (stack == NULL) {
        fputs("error: unable to create the stack\n", stderr);
        return 2;
    }

    int status = check_balance(stack);
    istack_destroy(stack);
    return status;
}
