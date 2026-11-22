#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    bst_t *t = bst_create();
    if (t == NULL) {
        return EXIT_FAILURE;
    }

    int key;
    int result;
    while ((result = scanf("%d", &key)) == 1) {
        if (bst_insert(t, key) != 0) {
            bst_destroy(t);
            return EXIT_FAILURE;
        }
    }

    int status = EXIT_SUCCESS;
    size_t n = bst_size(t);

    if (ferror(stdin)) {
        status = EXIT_FAILURE;
    } else if (result == 0) {
        printf("not a number after %zu values\n", n);
        status = EXIT_FAILURE;
    } else if (n == 0) {
        puts("empty tree");
    } else {
        printf("n %zu, height %zu\n", n, bst_height(t));
    }

    bst_destroy(t);
    return status;
}
