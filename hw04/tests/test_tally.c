#include <stdio.h>
#include "tally.h"
#include <string.h>

/* Print one test result; return 1 for failure, 0 for success. */
static int check(const char *label, int condition)
{
    if (condition) {
        printf("PASS: %s\n", label);
        return 0;
    }

    printf("FAIL: %s\n", label);
    return 1;
}

static int test_limits(void)
{
    int failed = 0;
    tally_t *t = tally_create();

    failed += check("create tally for limit tests", t != NULL);

    if (t == NULL) {
        return failed;
    }

    const char name15[] = "abcdefghijklmno";
    const char name16[] = "abcdefghijklmnop";

    failed += check("accept a 15-character name",
                    tally_add(t, name15) == 1);

    failed += check("reject a 16-character name",
                    tally_add(t, name16) == -1);

    failed += check("long name rejection changes nothing",
                    tally_size(t) == 1 &&
                    tally_count(t, name15) == 1 &&
                    tally_count(t, name16) == 0);

    /* One name is already stored; add 15 more. */
    int filled = 1;
    char name[TALLY_LEN + 1];

    for (int i = 0; i < TALLY_NAMES - 1; i++) {
        snprintf(name, sizeof(name), "n%02d", i);

        if (tally_add(t, name) != 1) {
            filled = 0;
        }
    }

    failed += check("fill all available entries",
                    filled && tally_size(t) == TALLY_NAMES);

    failed += check("full tally rejects a new name",
                    tally_add(t, "new") == -1);

    /* Check that every stored count survived the rejection. */
    int unchanged = tally_size(t) == TALLY_NAMES &&
                    tally_count(t, "new") == 0 &&
                    tally_count(t, name15) == 1;

    for (int i = 0; i < TALLY_NAMES - 1; i++) {
        snprintf(name, sizeof(name), "n%02d", i);

        if (tally_count(t, name) != 1) {
            unchanged = 0;
        }
    }

    failed += check("full tally rejection changes nothing",
                    unchanged);

    failed += check("full tally still counts an existing name",
                    tally_add(t, name15) == 2);

    failed += check("existing name changes count but not size",
                    tally_count(t, name15) == 2 &&
                    tally_size(t) == TALLY_NAMES);

    tally_destroy(t);
    return failed;
}

static int test_lifecycle(void)
{
    int failed = 0;
    tally_t *items[TALLY_MAX];
    int all_created = 1;

    for (int i = 0; i < TALLY_MAX; i++) {
        items[i] = tally_create();

        if (items[i] == NULL) {
            all_created = 0;
        }
    }

    failed += check("create TALLY_MAX tallies", all_created);

    if (!all_created) {
        for (int i = 0; i < TALLY_MAX; i++) {
            tally_destroy(items[i]);
        }

        return failed;
    }

    tally_t *extra = tally_create();

    failed += check("one more tally is refused", extra == NULL);
    tally_destroy(extra);

    tally_destroy(NULL);

    extra = tally_create();

    failed += check("destroy NULL does not free active tallies",
                    extra == NULL);

    tally_destroy(extra);

    int independent = tally_add(items[0], "door") == 1 &&
                      tally_add(items[0], "door") == 2 &&
                      tally_add(items[1], "door") == 1;

    failed += check("two tallies are independent",
                    independent &&
                    tally_count(items[0], "door") == 2 &&
                    tally_count(items[1], "door") == 1 &&
                    tally_size(items[0]) == 1 &&
                    tally_size(items[1]) == 1);

    tally_destroy(items[0]);
    items[0] = tally_create();

    failed += check("create after destroy", items[0] != NULL);

    if (items[0] != NULL) {
        failed += check("reused tally starts empty",
                        tally_size(items[0]) == 0 &&
                        tally_count(items[0], "door") == 0);

        char original[] = "sensor";
        int added = tally_add(items[0], original);

        original[0] = 'X';

        failed += check("tally copies the input name",
                        added == 1 &&
                        tally_count(items[0], "sensor") == 1 &&
                        tally_count(items[0], "Xensor") == 0);
    }

    for (int i = 0; i < TALLY_MAX; i++) {
        tally_destroy(items[i]);
    }

    return failed;
}

static int test_print(void)
{
    int failed = 0;
    tally_t *t = tally_create();

    failed += check("create tally for printing", t != NULL);

    if (t == NULL) {
        return failed;
    }

    int added = tally_add(t, "window") == 1 &&
                tally_add(t, "window") == 2 &&
                tally_add(t, "alarm") == 1 &&
                tally_add(t, "door") == 1;

    failed += check("prepare records for printing", added);

    FILE *out = tmpfile();

    failed += check("open temporary output stream", out != NULL);

    if (out == NULL) {
        tally_destroy(t);
        return failed;
    }

    tally_print(t, out);
    rewind(out);

    char actual[128];
    size_t n = fread(actual, 1, sizeof(actual) - 1, out);
    actual[n] = '\0';

    failed += check("print sorted names with correct counts",
                    !ferror(out) &&
                    strcmp(actual,
                           "alarm 1\ndoor 1\nwindow 2\n") == 0);

    failed += check("printing does not change the tally",
                    tally_size(t) == 3 &&
                    tally_count(t, "alarm") == 1 &&
                    tally_count(t, "door") == 1 &&
                    tally_count(t, "window") == 2);

    fclose(out);
    tally_destroy(t);

    return failed;
}

int main(void)
{
    int failed = 0;
    tally_t *t = tally_create();

    failed += check("create a tally", t != NULL);

    if (t != NULL) {
        failed += check("new tally is empty",
                        tally_size(t) == 0);

        failed += check("missing name has count zero",
                        tally_count(t, "door") == 0);

        failed += check("first addition returns one",
                        tally_add(t, "door") == 1);

        failed += check("second addition returns two",
                        tally_add(t, "door") == 2);

        failed += check("query returns the current count",
                        tally_count(t, "door") == 2);

        failed += check("add a different name",
                        tally_add(t, "alarm") == 1);

        failed += check("size counts different names",
                        tally_size(t) == 2);

        failed += check("reject an empty name",
                        tally_add(t, "") == -1);

        failed += check("reject a NULL name",
                        tally_add(t, NULL) == -1);

        failed += check("rejected additions change nothing",
                        tally_size(t) == 2 &&
                        tally_count(t, "door") == 2 &&
                        tally_count(t, "alarm") == 1);

        failed += check("query a NULL name",
                        tally_count(t, NULL) == 0);
    }

    failed += check("reject a NULL tally",
                    tally_add(NULL, "door") == -1);

    failed += check("query a NULL tally",
                    tally_count(NULL, "door") == 0);

    failed += check("size of a NULL tally",
                    tally_size(NULL) == 0);

            tally_destroy(t);

    failed += test_limits();
    failed += test_lifecycle();
    failed += test_print();

    printf("Summary: %d failed\n", failed);

    return failed != 0;
}
