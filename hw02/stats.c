#include "stats.h"

/* Find the minimum value */
float stats_min(const float a[], int n)
{
    float min = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] < min)
            min = a[i];
    }

    return min;
}

/* Find the maximum value recursively */
float stats_max(const float a[], int n)
{
    if (n == 1)
        return a[0];

    float rest = stats_max(a + 1, n - 1);

    return a[0] > rest ? a[0] : rest;
}

/* Calculate the arithmetic mean */
float stats_mean(const float a[], int n)
{
    float sum = 0.0f;

    for (int i = 0; i < n; i++) {
        sum += a[i];
    }

    return sum / n;
}

/* Find the longest consecutive run above threshold */
int stats_longest_run(const float temps[],
                      int n,
                      float threshold,
                      int *start_index)
{
    int longest = 0;
    int current = 0;
    int current_start = 0;

    *start_index = -1;

    for (int i = 0; i < n; i++) {
        if (temps[i] > threshold) {
            if (current == 0) {
                current_start = i;
            }

            current++;

            if (current > longest) {
                longest = current;
                *start_index = current_start;
            }
        } else {
            current = 0;
        }
    }

    return longest;
}