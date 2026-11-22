#include "../bst.h"
#include <stdio.h>
#include <stdlib.h>

static int check(bool passed, const char *name)
{
    printf("%s: %s\n", passed ? "PASS" : "FAIL", name);
    return passed ? 0 : 1;
}

static int test_original_keys(void)
{
    const int keys[] = {27, 14, 35, 10, 19, 31, 42};
    bst_t *t = bst_create();
    if (t == NULL) {
        puts("FAIL: create tree");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; i++) {
        if (bst_insert(t, keys[i]) != 0) {
            puts("FAIL: insert original keys");
            bst_destroy(t);
            return EXIT_FAILURE;
        }
    }

    int failures = 0;
    failures += check(bst_size(t) == 7, "size is 7");
    failures += check(bst_height(t) == 3, "height is 3");

    size_t visits = 0;
    bool found = bst_contains(t, 31, &visits);
    failures += check(found && visits == 3, "31 found in 3 visits");

    visits = 0;
    found = bst_contains(t, 15, &visits);
    failures += check(!found && visits == 3, "15 absent after 3 visits");

    bst_destroy(t);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int test_duplicates(void)
{
    bst_t *t = bst_create();
    if (t == NULL) {
        return check(false, "create tree for duplicate test");
    }

    bool passed = bst_insert(t, 14) == 0 &&
                  bst_insert(t, 14) == 0 &&
                  bst_size(t) == 2 &&
                  bst_height(t) == 2;

    bst_destroy(t);
    return check(passed, "duplicate key is kept");
}

static int test_increasing(void)
{
    bst_t *t = bst_create();
    if (t == NULL) {
        return check(false, "create tree for increasing-order test");
    }

    bool passed = true;
    for (int key = 1; key <= 5; key++) {
        if (bst_insert(t, key) != 0) {
            passed = false;
            break;
        }
    }

    passed = passed && bst_height(t) == 5;
    bst_destroy(t);
    return check(passed, "five increasing keys have height 5");
}

static int test_empty_tree(void);
int main(void)
{
    int failures = test_original_keys();
    failures += test_empty_tree();
    failures += test_duplicates();
    failures += test_increasing();
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int test_empty_tree(void)
{
    bst_t *t = bst_create();
    if (t == NULL) {
        return check(false, "create empty tree");
    }

    size_t visits = 99;
    bool found = bst_contains(t, 31, &visits);
    bst_destroy(t);
    return check(!found && visits == 0, "empty search takes zero visits");
}
