#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include "scenario.h"

static int parse_integer(const char *text, int base,
                         long long low, long long high,
                         long long *value)
{
    char *end;

    errno = 0;
    long long number = strtoll(text, &end, base);

    if (errno != 0 || end == text || *end != '\0' ||
        number < low || number > high) {
        return 0;
    }

    *value = number;
    return 1;
}

static int parse_real(const char *text, float *value)
{
    char *end;

    errno = 0;
    float number = strtof(text, &end);

    if (errno != 0 || end == text || *end != '\0' ||
        !isfinite(number)) {
        return 0;
    }

    *value = number;
    return 1;
}

int scenario_load(const char *path, scenario_t *s)
{
    if (path == NULL || s == NULL) {
        return -1;
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    char buffer[1024];
    int line_number = 0;
    int have_start = 0;
    int error = 0;

    s->start = 0;
    s->count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_number++;

        if (strchr(buffer, '\n') == NULL &&
            strlen(buffer) == sizeof(buffer) - 1) {
            int next = fgetc(file);
            if (next != EOF) {
                error = line_number;
                break;
            }
        }

        char *comment = strchr(buffer, '#');
        if (comment != NULL) {
            *comment = '\0';
        }

        char *fields[5];
        size_t count = 0;
        char *token = strtok(buffer, " \t\r\n");

        while (token != NULL && count < 5) {
            fields[count++] = token;
            token = strtok(NULL, " \t\r\n");
        }

        if (count == 0) {
            continue;
        }

        if (!have_start) {
            long long stamp;

            if (count != 2 ||
                strcmp(fields[0], "start") != 0 ||
                !parse_integer(fields[1], 10,
                               LLONG_MIN, LLONG_MAX, &stamp)) {
                error = line_number;
                break;
            }

            time_t start = (time_t)stamp;
            if ((long long)start != stamp) {
                error = line_number;
                break;
            }

            s->start = start;
            have_start = 1;
            continue;
        }

        if (count != 4 || s->count >= SCENARIO_MAX_LINES) {
            error = line_number;
            break;
        }

        long long tick;
        long long switches;
        scenario_line_t row;

        if (!parse_integer(fields[0], 10, 0, INT_MAX, &tick) ||
            !parse_real(fields[1], &row.temp_c) ||
            !parse_real(fields[2], &row.rh_pct) ||
            !parse_integer(fields[3], 16, 0, UINT32_MAX,
                           &switches)) {
            error = line_number;
            break;
        }

        row.tick = (int)tick;
        row.switches = (uint32_t)switches;

        if (s->count > 0 &&
            row.tick <= s->lines[s->count - 1].tick) {
            error = line_number;
            break;
        }

        s->lines[s->count++] = row;
    }

    if (error == 0) {
        if (ferror(file)) {
            error = -1;
        } else if (!have_start || s->count == 0) {
            error = line_number + 1;
        }
    }

    if (fclose(file) != 0 && error == 0) {
        error = -1;
    }

    return error;
}

const scenario_line_t *scenario_at(const scenario_t *s, int tick)
{
    if (s == NULL || s->count == 0 || tick < 0) {
        return NULL;
    }

    const scenario_line_t *active = NULL;

    for (size_t i = 0; i < s->count; i++) {
        if (s->lines[i].tick > tick) {
            break;
        }

        active = &s->lines[i];
    }

    return active;
}

