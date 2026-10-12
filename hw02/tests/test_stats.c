#include <stdio.h>
#include "../stats.h"

static int passed = 0;
static int failed = 0;

static void check(const char *name, int condition)
{
    if (condition) {
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

int main(void)
{
    /* Test 1: Single element */
    float single[] = {5.0f};
    check("single element",
          stats_max(single, 1) == 5.0f);

    /* Test 2: Maximum at the beginning */
    float first[] = {9.0f, 3.0f, 2.0f};
    check("maximum at beginning",
          stats_max(first, 3) == 9.0f);

    /* Test 3: Maximum at the end */
    float last[] = {2.0f, 3.0f, 9.0f};
    check("maximum at end",
          stats_max(last, 3) == 9.0f);

    /* Test 4: Equality breaks the run */
    float temps[] = {31.0f, 32.0f, 30.0f, 33.0f};
    int start = -1;

    int length = stats_longest_run(temps, 4, 30.0f, &start);

    check("threshold equality breaks run",
          length == 2 && start == 0);

    /* Test 5: Recursive maximum with 10000 elements */
    float large[10000];

    for (int i = 0; i < 10000; i++) {
        large[i] = (float)i;
    }

    check("recursive maximum 10000 elements",
          stats_max(large, 10000) == 9999.0f);

    printf("\nSummary: %d passed, %d failed\n",
           passed, failed);

    return failed == 0 ? 0 : 1;
}