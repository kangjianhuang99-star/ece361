#include "bst.h"
#include <stdlib.h>

struct node {
    int data;
    struct node *leftChild;
    struct node *rightChild;
};

struct bst {
    struct node *root;
    size_t size;
};

/* Replaces the original global root with a separately allocated tree. */
bst_t *bst_create(void)
{
    bst_t *t = malloc(sizeof *t);
    if (t == NULL) {
        return NULL;
    }

    t->root = NULL;
    t->size = 0;
    return t;
}

/* Adapted from insert() in BinarySearchTree.c. */
int bst_insert(bst_t *t, int key)
{
    if (t == NULL) {
        return -1;
    }

    struct node *tempNode = malloc(sizeof *tempNode);
    if (tempNode == NULL) {
        return -1;
    }

    tempNode->data = key;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    struct node *current = t->root;
    struct node *parent = NULL;

    while (current != NULL) {
        parent = current;
        if (key < current->data) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    if (parent == NULL) {
        t->root = tempNode;
    } else if (key < parent->data) {
        parent->leftChild = tempNode;
    } else {
        parent->rightChild = tempNode;
    }

    t->size++;
    return 0;
}

/* New function. Returns the key count maintained by bst_insert(). */
size_t bst_size(const bst_t *t)
{
    if (t == NULL) {
        return 0;
    }
    return t->size;
}

/* New helper for bst_height(). */
static size_t node_height(const struct node *current)
{
    if (current == NULL) {
        return 0;
    }

    size_t left = node_height(current->leftChild);
    size_t right = node_height(current->rightChild);

    if (left > right) {
        return 1 + left;
    }
    return 1 + right;
}

/* New function. The original example did not measure height. */
size_t bst_height(const bst_t *t)
{
    if (t == NULL) {
        return 0;
    }
    return node_height(t->root);
}

/* New helper for bst_destroy(). */
static void destroy_nodes(struct node *current)
{
    if (current == NULL) {
        return;
    }

    destroy_nodes(current->leftChild);
    destroy_nodes(current->rightChild);
    free(current);
}

/* New function. The original example did not free its tree. */
void bst_destroy(bst_t *t)
{
    if (t == NULL) {
        return;
    }

    destroy_nodes(t->root);
    free(t);
}

/* Adapted from search() in BinarySearchTree.c. */
bool bst_contains(const bst_t *t, int key, size_t *visits)
{
    if (visits != NULL) {
        *visits = 0;
    }
    if (t == NULL) {
        return false;
    }

    const struct node *current = t->root;
    while (current != NULL) {
        if (visits != NULL) {
            (*visits)++;
        }

        if (current->data == key) {
            return true;
        }
        if (key < current->data) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }
    return false;
}
