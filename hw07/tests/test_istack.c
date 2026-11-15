#include "istack.h"

#include <stdio.h>
#include <stdlib.h>

static bool test_empty(void)
{
    istack_t *s = istack_create();
    if (s == NULL) {
        return false;
    }

    int value = 42;
    bool ok = istack_count(s) == 0 &&
              istack_is_empty(s) &&
              istack_capacity(s) == ISTACK_START &&
              istack_pop(s, &value) == -1 && value == 42 &&
              istack_peek(s, &value) == -1 && value == 42;

    istack_destroy(s);
    return ok;
}

static bool test_lifo(void)
{
    istack_t *s = istack_create();
    if (s == NULL) {
        return false;
    }

    int value = 0;
    bool ok = istack_push(s, 10) == 0 &&
              istack_push(s, 20) == 0 &&
              istack_peek(s, &value) == 0 && value == 20 &&
              istack_count(s) == 2 &&
              istack_pop(s, &value) == 0 && value == 20 &&
              istack_pop(s, &value) == 0 && value == 10 &&
              istack_is_empty(s);

    istack_destroy(s);
    return ok;
}

static bool test_growth(void)
{
    istack_t *s = istack_create();
    if (s == NULL) {
        return false;
    }

    bool ok = istack_capacity(s) == 8;
    for (int i = 1; i <= 8 && ok; i++) {
        ok = istack_push(s, i) == 0;
    }

    ok = ok && istack_count(s) == 8 && istack_capacity(s) == 8 &&
         istack_push(s, 9) == 0 &&
         istack_count(s) == 9 && istack_capacity(s) == 16;

    for (int i = 9; i >= 1 && ok; i--) {
        int value;
        ok = istack_pop(s, &value) == 0 && value == i;
    }

    ok = ok && istack_is_empty(s) && istack_capacity(s) == 16;
    istack_destroy(s);
    return ok;
}

static bool test_large_stack(void)
{
    istack_t *s = istack_create();
    if (s == NULL) {
        return false;
    }

    bool ok = true;
    for (int i = 0; i < 1000 && ok; i++) {
        ok = istack_push(s, i) == 0;
    }

    ok = ok && istack_count(s) == 1000;

    for (int i = 999; i >= 0 && ok; i--) {
        int value;
        ok = istack_pop(s, &value) == 0 && value == i;
    }

    ok = ok && istack_is_empty(s);
    istack_destroy(s);
    return ok;
}

static bool test_array_copy(void)
{
    istack_t *s = istack_create();
    if (s == NULL) {
        return false;
    }

    size_t count = 99;
    int *empty = istack_to_array(s, &count);
    bool ok = empty == NULL && count == 0;
    free(empty);

    ok = ok && istack_push(s, 7) == 0 &&
         istack_push(s, 8) == 0 && istack_push(s, 9) == 0;

    int *copy = istack_to_array(s, &count);
    ok = ok && copy != NULL && count == 3 &&
         copy[0] == 7 && copy[1] == 8 && copy[2] == 9;

    if (ok) {
        int value;
        copy[2] = 100;
        ok = istack_peek(s, &value) == 0 && value == 9;
    }

    free(copy);
    istack_destroy(s);
    return ok;
}

static bool test_null_arguments(void)
{
    int value = 42;
    size_t count = 99;
    int *copy = istack_to_array(NULL, &count);
    bool ok = copy == NULL && count == 0 &&
              istack_push(NULL, 1) == -1 &&
              istack_pop(NULL, &value) == -1 && value == 42 &&
              istack_peek(NULL, &value) == -1 && value == 42 &&
              istack_count(NULL) == 0 &&
              istack_is_empty(NULL) &&
              istack_capacity(NULL) == 0;

    free(copy);
    istack_destroy(NULL);

    istack_t *s = istack_create();
    if (s == NULL) {
        return false;
    }

    ok = ok && istack_push(s, 7) == 0;
    copy = istack_to_array(s, NULL);
    ok = ok && copy == NULL &&
         istack_peek(s, NULL) == 0 && istack_count(s) == 1 &&
         istack_pop(s, NULL) == 0 && istack_is_empty(s);

    free(copy);
    istack_destroy(s);
    return ok;
}

static bool test_independent_stacks(void)
{
    istack_t *a = istack_create();
    istack_t *b = istack_create();
    int value;

    bool ok = a != NULL && b != NULL &&
              istack_push(a, 11) == 0 && istack_is_empty(b) &&
              istack_push(b, 22) == 0 &&
              istack_pop(a, &value) == 0 && value == 11 &&
              istack_is_empty(a) &&
              istack_peek(b, &value) == 0 && value == 22;

    istack_destroy(a);
    istack_destroy(b);
    return ok;
}

static bool test_destroy_nonempty(void)
{
    istack_t *s = istack_create();
    if (s == NULL) {
        return false;
    }

    bool ok = istack_push(s, 42) == 0;
    istack_destroy(s);
    return ok;
}

int main(void)
{
    const struct {
        const char *name;
        bool (*run)(void);
    } tests[] = {
        {"empty stack", test_empty},
        {"last in, first out", test_lifo},
        {"capacity grows from 8 to 16", test_growth},
        {"1000 pushes and pops", test_large_stack},
        {"bottom-first array copy", test_array_copy},
        {"NULL arguments", test_null_arguments},
        {"independent stacks", test_independent_stacks},
        {"destroy nonempty stack", test_destroy_nonempty}
    };

    size_t passed = 0;
    size_t total = sizeof tests / sizeof tests[0];

    for (size_t i = 0; i < total; i++) {
        bool ok = tests[i].run();
        printf("%s: %s\n", ok ? "PASS" : "FAIL", tests[i].name);
        if (ok) {
            passed++;
        }
    }

    printf("Summary: %zu/%zu passed\n", passed, total);
    return passed == total ? 0 : 1;
}
