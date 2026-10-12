#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/*
 * Print a histogram of temperature readings.
 *
 * Parameters:
 *   temps - array containing temperature values
 *   n     - number of readings
 *
 * Returns:
 *   Nothing (void).
 *
 * Preconditions:
 *   temps is a valid array when n > 0.
 *   n >= 0.
 */
void histogram_print(const float temps[], int n);

#endif