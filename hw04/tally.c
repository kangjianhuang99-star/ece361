#include "tally.h"
#include <string.h>

/* One entry stores one name and its occurrence count. */
struct entry {
    char name[TALLY_LEN + 1];
    int count;
};

/* The complete tally structure is private to this file. */
struct tally {
    struct entry entries[TALLY_NAMES];
    int n;
    int in_use;
};

/* Storage for all tallies; initially zero-initialized. */
static tally_t pool[TALLY_MAX];

tally_t *tally_create(void)
{
    for (int i = 0; i < TALLY_MAX; i++) {
        if (pool[i].in_use == 0) {
            pool[i].in_use = 1;
            pool[i].n = 0;
            return &pool[i];
        }
    }

    return NULL;
}

void tally_destroy(tally_t *t)
{
    if (t == NULL) {
        return;
    }

    t->n = 0;
    t->in_use = 0;
}

int tally_size(const tally_t *t)
{
    if (t == NULL) {
        return 0;
    }

    return t->n;
}

int tally_count(const tally_t *t, const char *name)
{
    if (t == NULL || name == NULL) {
        return 0;
    }

    for (int i = 0; i < t->n; i++) {
        if (strcmp(t->entries[i].name, name) == 0) {
            return t->entries[i].count;
        }
    }

    return 0;
}

int tally_add(tally_t *t, const char *name)
{
    /* Check arguments before accessing them. */
    if (t == NULL || name == NULL) {
        return -1;
    }

    size_t len = strlen(name);

    if (len == 0 || len > TALLY_LEN) {
        return -1;
    }

    /* Find the existing name or its insertion position. */
    int pos = 0;

    while (pos < t->n &&
           strcmp(t->entries[pos].name, name) < 0) {
        pos++;
    }

    /* An existing name can still count when the tally is full. */
    if (pos < t->n &&
        strcmp(t->entries[pos].name, name) == 0) {
        t->entries[pos].count++;
        return t->entries[pos].count;
    }

    /* A new name needs an available entry. */
    if (t->n == TALLY_NAMES) {
        return -1;
    }

    /* Move entries from back to front to make room. */
    for (int i = t->n; i > pos; i--) {
        t->entries[i] = t->entries[i - 1];
    }

    /* Copy the name, including its terminating null character. */
    memcpy(t->entries[pos].name, name, len + 1);
    t->entries[pos].count = 1;
    t->n++;

    return 1;
}

void tally_print(const tally_t *t, FILE *out)
{
    if (t == NULL || out == NULL) {
        return;
    }

    for (int i = 0; i < t->n; i++) {
        fprintf(out, "%s %d\n",
                t->entries[i].name,
                t->entries[i].count);
    }
}
