#ifndef PTR_H
#define PTR_H

// Inputs: two valid float pointers. Swaps their values. No return value.
void swap_float(float *a, float *b);

// Inputs: array of n floats and valid output pointers.
// Returns 0 on success, -1 for invalid input without changing outputs.
int minmax(const float *a, int n, float *min, float *max);

// Inputs: readable array of n floats and a threshold.
// Returns the first matching element's address, or NULL.
const float *find_above(const float *a, int n, float threshold);

// Input: writable array of n floats. Reverses it in place.
// Does nothing if a is NULL or n < 2.
void reverse(float *a, int n);

#endif
