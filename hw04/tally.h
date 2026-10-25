#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX    4
#define TALLY_NAMES 16
#define TALLY_LEN   15

typedef struct tally tally_t;

/*
 * Takes no arguments.
 * Returns a new empty tally, or NULL if all TALLY_MAX slots are in use.
 * No preconditions.
 */
tally_t *tally_create(void);

/*
 * Takes a live tally returned by tally_create, or NULL.
 * Releases the tally for reuse; NULL does nothing.
 * Returns nothing. A non-NULL tally must not already be destroyed.
 */
void tally_destroy(tally_t *t);

/*
 * Takes a live tally or NULL, and a name or NULL.
 * Returns the new count after adding one occurrence.
 * Returns -1 without changing the tally if either argument is NULL,
 * the name is empty or longer than TALLY_LEN, or a new name cannot fit.
 * A non-NULL name must be a null-terminated string.
 */
int tally_add(tally_t *t, const char *name);

/*
 * Takes a live tally or NULL, and a name or NULL.
 * Returns the name's count, or 0 if absent or either argument is NULL.
 * A non-NULL name must be a null-terminated string.
 */
int tally_count(const tally_t *t, const char *name);

/*
 * Takes a live tally or NULL.
 * Returns the number of different names, or 0 for NULL.
 * A non-NULL tally must still be in use.
 */
int tally_size(const tally_t *t);

/*
 * Takes a live tally and a writable output stream.
 * Prints one "name count" line per name, in strcmp order.
 * Returns nothing. Both arguments must be valid and non-NULL.
 */
void tally_print(const tally_t *t, FILE *out);

#endif
