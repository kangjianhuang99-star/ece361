#ifndef SCENARIO_H
#define SCENARIO_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#define SCENARIO_MAX_LINES 1000

typedef struct {
    int tick;
    float temp_c;
    float rh_pct;
    uint32_t switches;
} scenario_line_t;

typedef struct {
    time_t start;
    size_t count;
    scenario_line_t lines[SCENARIO_MAX_LINES];
} scenario_t;

/* Load a scenario: return 0, the first invalid line number, or -1 on I/O error. */
int scenario_load(const char *path, scenario_t *s);

/* Return the line in force at tick, or NULL if no line applies. */
const scenario_line_t *scenario_at(const scenario_t *s, int tick);

#endif
