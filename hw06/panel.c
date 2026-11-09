#include "panel.h"
#include "iom361_r4.h"

uint32_t panel_pressed(uint32_t now, uint32_t before)
{
    return now & ~before;
}

uint32_t panel_released(uint32_t now, uint32_t before)
{
    return ~now & before;
}

int panel_led(uint32_t *base, int n, int on)
{
    int rc;
    uint32_t leds;
    uint32_t mask;

    if (n < 0 || n > 31) {
        return -1;
    }

    leds = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    mask = (uint32_t)1 << n;

    if (on) {
        leds |= mask;
    } else {
        leds &= ~mask;
    }

    iom361_writeReg(base, LEDS_REG, leds, &rc);
    return rc;
}

int panel_toggle(uint32_t *base, int n)
{
    int rc;
    uint32_t leds;
    uint32_t mask;

    if (n < 0 || n > 31) {
        return -1;
    }

    leds = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0) {
        return rc;
    }

    mask = (uint32_t)1 << n;
    leds ^= mask;

    iom361_writeReg(base, LEDS_REG, leds, &rc);
    return rc;
}

