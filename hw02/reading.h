#ifndef READING_H
#define READING_H

#include <stdio.h>

#define MAX_READINGS 1000

/*
 * Read sensor data from an input stream.
 *
 * Parameters:
 *   input   - input file stream
 *   ticks   - array storing time ticks
 *   temps   - array storing temperatures
 *   hums    - array storing humidity values
 *   skipped - pointer to skipped-line count
 *
 * Returns:
 *   Number of valid readings stored.
 *
 * Preconditions:
 *   Arrays have capacity for MAX_READINGS elements.
 *   input and skipped are valid pointers.
 */
int reading_load(FILE *input,
                 int ticks[],
                 float temps[],
                 float hums[],
                 int *skipped);

#endif