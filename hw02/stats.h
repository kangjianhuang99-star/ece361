#ifndef STATS_H
#define STATS_H

/*
 * Return the minimum value in an array.
 * Preconditions: a is valid and n > 0.
 */
float stats_min(const float a[], int n);

/*
 * Return the maximum value using recursion.
 * Preconditions: a is valid and n > 0.
 */
float stats_max(const float a[], int n);

/*
 * Return the arithmetic mean of an array.
 * Preconditions: a is valid and n > 0.
 */
float stats_mean(const float a[], int n);

/*
 * Find the longest consecutive run strictly above threshold.
 *
 * Parameters:
 *   temps       - temperature array
 *   n           - number of readings
 *   threshold   - temperature limit
 *   start_index - receives the starting index of the longest run
 *
 * Returns:
 *   Length of the longest run, or 0 if none exists.
 *
 * Preconditions:
 *   temps and start_index are valid pointers, n >= 0.
 */
int stats_longest_run(const float temps[],
                      int n,
                      float threshold,
                      int *start_index);
#endif