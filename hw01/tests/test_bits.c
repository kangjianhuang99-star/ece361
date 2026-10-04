#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "bits.h"
#include "status.h"

static int tests_run = 0;
static int tests_failed = 0;

static void check(const char *name, int passed)
{
    tests_run++;
    if (passed) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

static void check_print_binary(const char *name, uint32_t value, int width,
                               const char *expected)
{
    FILE *capture = tmpfile();
    char actual[64] = "";

    if (capture == NULL) {
        check(name, 0);
        return;
    }

    fflush(stdout);
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0) {
        fclose(capture);
        check(name, 0);
        return;
    }

    if (dup2(fileno(capture), STDOUT_FILENO) < 0) {
        close(saved_stdout);
        fclose(capture);
        check(name, 0);
        return;
    }

    print_binary(value, width);
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    rewind(capture);
    fgets(actual, sizeof actual, capture);
    fclose(capture);

    check(name, strcmp(actual, expected) == 0);
}
int main(void)
{
    check_print_binary("print_binary example", 0x2Cu, 8, "0010 1100\n");
    check_print_binary("print_binary width 1", 1u, 1, "1\n");
    check_print_binary(
        "print_binary width 32", 0x80000001u, 32,
        "1000 0000 0000 0000 0000 0000 0000 0001\n"
    );

    check("get_field width 1 at pos 31",
          get_field(0x80000000u, 31, 1) == 1u);
    check("get_field width 32",
          get_field(0x89ABCDEFu, 0, 32) == 0x89ABCDEFu);
    check("get_field invalid range returns 0",
          get_field(0xFFFFFFFFu, 31, 2) == 0u);

    check("set_field width 1 at pos 31",
          set_field(0u, 31, 1, 1u) == 0x80000000u);
    check("set_field width 32",
          set_field(0u, 0, 32, 0x89ABCDEFu) == 0x89ABCDEFu);
    check("set_field truncates a too-wide value",
          set_field(0u, 4, 3, 0xFu) == 0x70u);
    check("set_field invalid range preserves word",
          set_field(0x12345678u, 31, 2, 3u) == 0x12345678u);
    check("sign_extend width 1 positive", sign_extend(0u, 1) == 0);
    check("sign_extend width 1 negative", sign_extend(1u, 1) == -1);
    check("sign_extend width 8 most negative",
          sign_extend(0x80u, 8) == -128);
    check("sign_extend width 32 most negative",
          sign_extend(0x80000000u, 32) == INT32_MIN);
    check("sign_extend width 32 maximum positive",
          sign_extend(0x7FFFFFFFu, 32) == INT32_MAX);
    check("sign_extend assignment example",
          sign_extend(0xF8u, 8) == -8);

    status_t example = status_unpack(0x1631u);
    check("status_unpack assignment example",
          example.setpoint == 22 && example.mode == 3 &&
          example.heat == 1 && example.cool == 0 &&
          example.fan == 0 && example.fault == 0 &&
          example.reserved == 0);

    status_t invalid_mode = status_unpack(0xFF50u);
    check("status_unpack preserves invalid mode 5",
          invalid_mode.mode == 5 && invalid_mode.setpoint == -1);

    status_t boundary = status_unpack(0x804Fu);
    check("status_unpack negative setpoint and mode 4",
          boundary.setpoint == -128 && boundary.mode == 4 &&
          boundary.heat == 1 && boundary.cool == 1 &&
          boundary.fan == 1 && boundary.fault == 1 &&
          boundary.reserved == 0);

    printf("\nSummary: %d tests, %d failed\n", tests_run, tests_failed);
    return tests_failed == 0 ? 0 : 1;
}
