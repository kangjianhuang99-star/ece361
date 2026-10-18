
#include <stdio.h>
#include "../ptr.h"

static int passed = 0;
static int failed = 0;

static void check(int condition, const char *name)
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
    float x = 3.0f;
    float y = 8.0f;

    swap_float(&x, &y);
    check(x == 8.0f && y == 3.0f, "swap_float");

    float data[] = {3.0f, 7.0f, 2.0f, 9.0f};
    float min = 0.0f;
    float max = 0.0f;

    int result = minmax(data, 4, &min, &max);

    check(result == 0 && min == 2.0f && max == 9.0f,
          "minmax multiple values");

    float single[] = {5.0f};

    result = minmax(single, 1, &min, &max);

    check(result == 0 && min == 5.0f && max == 5.0f,
          "minmax single value");

    min = 100.0f;
    max = 200.0f;

    result = minmax(data, 0, &min, &max);

    check(result == -1 && min == 100.0f && max == 200.0f,
          "minmax n zero");

    result = minmax(NULL, 4, &min, &max);

    check(result == -1 && min == 100.0f && max == 200.0f,
          "minmax NULL");


float values[] = {2.0f, 7.0f, 4.0f, 9.0f};

const float *found = find_above(values, 4, 5.0f);
check(found == &values[1], "find_above first match");

found = find_above(values, 4, 10.0f);
check(found == NULL, "find_above no match");

found = find_above(values, 4, 9.0f);
check(found == NULL, "find_above equal threshold");

float even[] = {1.0f, 2.0f, 3.0f, 4.0f};
reverse(even, 4);
check(even[0] == 4.0f && even[1] == 3.0f &&
      even[2] == 2.0f && even[3] == 1.0f,
      "reverse even length");

float odd[] = {1.0f, 2.0f, 3.0f};
reverse(odd, 3);
check(odd[0] == 3.0f && odd[1] == 2.0f &&
      odd[2] == 1.0f, "reverse odd length");

    printf("\nPassed: %d, Failed: %d\n", passed, failed);

    return failed == 0 ? 0 : 1;
}

