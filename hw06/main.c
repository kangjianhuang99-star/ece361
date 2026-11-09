#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "sensor.h"
#include "panel.h"
#include "scenario.h"
#include "iom361_r4.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s scenario.txt\n", argv[0]);
        return 1;
    }

    scenario_t scenario;
    int rc = scenario_load(argv[1], &scenario);

    if (rc != 0) {
        if (rc > 0) {
            fprintf(stderr, "Invalid scenario at line %d\n", rc);
        } else {
            fprintf(stderr, "Cannot load scenario: %s\n", argv[1]);
        }
        return 1;
    }

    int last_tick = scenario.lines[scenario.count - 1].tick;
    intmax_t start = (intmax_t)scenario.start;

    if (start > INTMAX_MAX - last_tick) {
        fprintf(stderr, "Scenario time is out of range\n");
        return 1;
    }

    time_t end = (time_t)(start + last_tick);
    if ((intmax_t)end != start + last_tick) {
        fprintf(stderr, "Scenario time is out of range\n");
        return 1;
    }

    uint32_t *base = iom361_initialize(4, 4, &rc);
    if (base == NULL || rc != 0) {
        fprintf(stderr, "Initialization failed: %d\n", rc);
        return 1;
    }

    iom361_set_display(false);

    uint32_t before = 0;

    for (int tick = 0; ; tick++) {
        const scenario_line_t *line = scenario_at(&scenario, tick);

        if (line != NULL) {
            _iom361_set_sensor1(line->temp_c, line->rh_pct);
            _iom361_set_switches(line->switches);
        }

        time_t now = (time_t)(start + tick);
        reading_t reading;

        rc = sensor_read(base, now, &reading);
        if (rc != 0) {
            fprintf(stderr, "Sensor read failed: %d\n", rc);
            return 1;
        }

        uint32_t switches = iom361_readReg(base, SWITCHES_REG, &rc);
        if (rc != 0) {
            fprintf(stderr, "Switch read failed: %d\n", rc);
            return 1;
        }

        uint32_t pressed = panel_pressed(switches, before);

        for (int k = 0; k < 4; k++) {
            if (pressed & ((uint32_t)1 << k)) {
                rc = panel_toggle(base, k);
                if (rc != 0) {
                    fprintf(stderr, "LED toggle failed: %d\n", rc);
                    return 1;
                }
            }
        }

        uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);
        if (rc != 0) {
            fprintf(stderr, "LED read failed: %d\n", rc);
            return 1;
        }

        struct tm *utc = gmtime(&reading.t);
        char timestamp[32];

        if (utc == NULL ||
            strftime(timestamp, sizeof(timestamp),
                     "%Y-%m-%d %H:%M:%S", utc) == 0) {
            fprintf(stderr, "Time formatting failed\n");
            return 1;
        }

        printf("%s temp=%.1f rh=%.1f switches=0x%x "
               "pressed=0x%x leds=0x%x\n",
               timestamp,
               reading.temp_c,
               reading.rh_pct,
               (unsigned)switches,
               (unsigned)pressed,
               (unsigned)leds);

        before = switches;

        if (tick == last_tick) {
            break;
        }
    }

    return 0;
}
