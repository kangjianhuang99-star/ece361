#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct bst bst_t;

/* Takes no arguments. Returns a new empty tree, or NULL if allocation fails. */
bst_t *bst_create(void);

/* Frees the tree and all its nodes. Accepts NULL and returns nothing. */
void bst_destroy(bst_t *t);

/* Inserts key, placing equal keys to the right. Returns 0 on success.
   If t is NULL or allocation fails, returns -1 and leaves the tree unchanged. */
int bst_insert(bst_t *t, int key);

/* Returns true if key is in t. If visits is non-NULL, stores the number
   of nodes compared there. A NULL tree returns false with zero visits. */
bool bst_contains(const bst_t *t, int key, size_t *visits);

/* Returns the number of keys, including duplicates. Returns 0 for NULL. */
size_t bst_size(const bst_t *t);

/* Returns the number of nodes on the longest path from the root to a leaf.
   Returns 0 for an empty tree or NULL. */
size_t bst_height(const bst_t *t);

#endif
