#ifndef ISTACK_H
#define ISTACK_H

#include <stdbool.h>
#include <stddef.h>

#define ISTACK_START 8

typedef struct istack istack_t;

/* Creates an empty stack with room for ISTACK_START values, or returns NULL on allocation failure. */
istack_t *istack_create(void);

/* Frees s and its storage. A NULL s is accepted. */
void istack_destroy(istack_t *s);

/* Pushes v onto s, doubling capacity when full. Returns 0 on success or -1 on failure, leaving s unchanged. */
int istack_push(istack_t *s, int v);

/* Removes the top value, storing it in *v if v is non-NULL. Returns 0, or -1 for a NULL or empty s, leaving *v unchanged. */
int istack_pop(istack_t *s, int *v);

/* Reads the top value into *v if v is non-NULL. Returns 0, or -1 for a NULL or empty s, leaving *v unchanged. */
int istack_peek(const istack_t *s, int *v);

/* Returns the number of values in s, or 0 if s is NULL. */
size_t istack_count(const istack_t *s);

/* Returns true if s is NULL or has no values. */
bool istack_is_empty(const istack_t *s);

/* Returns the current capacity of s, or 0 if s is NULL. */
size_t istack_capacity(const istack_t *s);

/* Returns a bottom-first copy that the caller must free and stores its length in *n.
 * Returns NULL if n is NULL, or if s is NULL, empty, or allocation fails.
 * When n is non-NULL, a failure sets *n to 0.
 */
int *istack_to_array(const istack_t *s, size_t *n);

#endif
