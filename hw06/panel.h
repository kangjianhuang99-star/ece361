#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>

/* Return the switches that changed from off to on. */
uint32_t panel_pressed(uint32_t now, uint32_t before);

/* Return the switches that changed from on to off. */
uint32_t panel_released(uint32_t now, uint32_t before);

/* Set LED n without changing other LEDs; return -1 for an invalid n. */
int panel_led(uint32_t *base, int n, int on);

/* Toggle LED n without changing other LEDs; return -1 for an invalid n. */
int panel_toggle(uint32_t *base, int n);

#endif

