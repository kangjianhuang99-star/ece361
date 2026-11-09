#include <stdio.h>
#include <math.h>
#include "sensor.h"
#include "panel.h"
#include "scenario.h"
#include "iom361_r4.h"

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

static int leds_are(uint32_t *base, uint32_t expected)
{
    int rc;
    uint32_t value = iom361_readReg(base, LEDS_REG, &rc);

    return rc == 0 && value == expected;
}

static int load_text(const char *text, scenario_t *s)
{
    const char *path = "tests/scenario_test_input.tmp";
    FILE *file = fopen(path, "w");

    if (file == NULL) {
        return -1;
    }

    int written = fputs(text, file);
    int closed = fclose(file);

    if (written == EOF || closed != 0) {
        return -1;
    }

    return scenario_load(path, s);
}

static void test_sensor(uint32_t *base)
{
    reading_t r = {0, 0.0f, 0.0f};

    _iom361_set_sensor1(91.0f, 40.0f);
    int rc = sensor_read(base, (time_t)1791273600, &r);

    check(rc == 0 && fabsf(r.temp_c - 91.0f) < 0.001f,
          "sensor reads 91.0 C");

    check(rc == 0 && fabsf(r.rh_pct - 40.0f) < 0.001f,
          "sensor reads humidity");

    check(rc == 0 && r.t == (time_t)1791273600,
          "sensor stores timestamp");

    r.t = (time_t)123;
    r.temp_c = 12.3f;
    r.rh_pct = 45.6f;

    rc = sensor_read(NULL, (time_t)999, &r);

    check(rc == 1,
          "sensor returns iom361 error code");

    check(r.t == (time_t)123 &&
          r.temp_c == 12.3f && r.rh_pct == 45.6f,
          "failed sensor read changes nothing");
}

static void test_switches(void)
{
    check(panel_pressed(0x1u, 0x0u) == 0x1u,
          "pressed detects a press");

    check(panel_released(0x1u, 0x0u) == 0x0u,
          "released ignores a press");

    check(panel_pressed(0x0u, 0x1u) == 0x0u,
          "pressed ignores a release");

    check(panel_released(0x0u, 0x1u) == 0x1u,
          "released detects a release");

    check(panel_pressed(0x1u, 0x1u) == 0x0u,
          "pressed ignores a held switch");

    check(panel_released(0x1u, 0x1u) == 0x0u,
          "released ignores a held switch");

    check(panel_pressed(0x6u, 0x3u) == 0x4u &&
          panel_released(0x6u, 0x3u) == 0x1u,
          "mixed switch changes");
}

static void test_leds(uint32_t *base)
{
    int rc;
    iom361_writeReg(base, LEDS_REG, 0xA5A55A5Au, &rc);

    check(rc == 0, "set initial LED pattern");

    rc = panel_led(base, 0, 1);
    check(rc == 0 && leds_are(base, 0xA5A55A5Bu),
          "LED on preserves other LEDs");

    rc = panel_led(base, 0, 0);
    check(rc == 0 && leds_are(base, 0xA5A55A5Au),
          "LED off preserves other LEDs");

    rc = panel_toggle(base, 31);
    check(rc == 0 && leds_are(base, 0x25A55A5Au),
          "toggle LED 31 preserves other LEDs");

    rc = panel_toggle(base, 31);
    check(rc == 0 && leds_are(base, 0xA5A55A5Au),
          "toggle twice restores original pattern");

    check(panel_led(base, -1, 1) == -1,
          "panel_led rejects negative index");

    check(panel_led(base, 32, 1) == -1,
          "panel_led rejects index 32");

    check(panel_toggle(base, -1) == -1,
          "panel_toggle rejects negative index");

    check(panel_toggle(base, 32) == -1,
          "panel_toggle rejects index 32");

    check(leds_are(base, 0xA5A55A5Au),
          "invalid LED indices change nothing");
}

static void test_scenarios(void)
{
    scenario_t s = {0};

    int rc = load_text(
        "# Example scenario\n"
        "\n"
        "start 1791273600 # UTC start\n"
        "\n"
        "0 21.5 40.0 0x0\n"
        "3 91.0 55.0 0x1 # press\n"
        "6 5.0 60.0 0x0\n", &s);

    check(rc == 0 && s.count == 3 &&
          s.start == (time_t)1791273600,
          "load comments and blank lines");

    const scenario_line_t *line = scenario_at(&s, 0);
    check(line != NULL && line->tick == 0 &&
          fabsf(line->temp_c - 21.5f) < 0.001f,
          "scenario initial values");

    line = scenario_at(&s, 2);
    check(line != NULL && line->tick == 0 &&
          line->switches == 0x0u,
          "values hold before next line");

    line = scenario_at(&s, 3);
    check(line != NULL && line->tick == 3 &&
          line->switches == 0x1u,
          "values change at specified tick");

    line = scenario_at(&s, 5);
    check(line != NULL && line->tick == 3 &&
          fabsf(line->temp_c - 91.0f) < 0.001f &&
          fabsf(line->rh_pct - 55.0f) < 0.001f &&
          line->switches == 0x1u,
          "all values hold between lines");

    line = scenario_at(&s, 10);
    check(line != NULL && line->tick == 6,
          "last values remain in force");

    check(scenario_at(&s, -1) == NULL,
          "negative tick has no active line");

    rc = load_text(
        "# Missing start\n"
        "0 21.5 40.0 0x0\n", &s);
    check(rc == 2, "reject missing start at line 2");

    rc = load_text(
        "start 1791273600\n"
        "0 21.5 40.0 0x0\n"
        "1 21.5 40.0\n", &s);
    check(rc == 3, "reject missing field at line 3");

    rc = load_text(
        "start 1791273600\n"
        "0 21.5 40.0 0x0\n"
        "0 22.0 45.0 0x1\n", &s);
    check(rc == 3, "reject repeated tick at line 3");

    rc = load_text(
        "start 1791273600\n"
        "2 21.5 40.0 0x0\n"
        "1 22.0 45.0 0x1\n", &s);
    check(rc == 3, "reject decreasing tick at line 3");

    rc = load_text(
        "start 1791273600\n"
        "0 nan 40.0 0x0\n", &s);
    check(rc == 2, "reject invalid sensor value");

    remove("tests/scenario_test_input.tmp");
}

int main(void)
{
    int rc;
    uint32_t *base = iom361_initialize(32, 32, &rc);

    if (base == NULL || rc != 0) {
        fprintf(stderr, "Test initialization failed\n");
        return 1;
    }

    iom361_set_display(false);

    test_sensor(base);
    test_switches();
    test_leds(base);
    test_scenarios();

    printf("Unit tests: %d passed, %d failed\n", passed, failed);

    return failed != 0;
}
