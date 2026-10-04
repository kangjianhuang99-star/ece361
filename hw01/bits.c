#include "bits.h"
#include <stdio.h>

void print_binary(uint32_t x, int width)
{
    for (int bit = width - 1; bit >= 0; bit--) {
        putchar(((x >> bit) & 1u) ? '1' : '0');
        if (bit > 0 && bit % 4 == 0) {
            putchar(' ');
        }
}
    putchar('\n');
}

uint32_t get_field(uint32_t word, int pos, int width)
{
    if (pos < 0 || pos > 31 || width < 1 || width > 32 ||
        pos + width > 32) {
        return 0;
    }
    if (width == 32) {
        return word;
    }

    uint32_t mask = (1u << width) - 1u;
    return (word >> pos) & mask;
}

uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    if (pos < 0 || pos > 31 || width < 1 || width > 32 ||
        pos + width > 32) {
        return word;
    }
    if (width == 32) {
        return value;
    }

    uint32_t mask = (1u << width) - 1u;
    uint32_t shifted_mask = mask << pos;
    word = word & ~shifted_mask;
    word = word | ((value & mask) << pos);
    return word;
}
int32_t sign_extend(uint32_t value, int width)
{
    uint32_t mask;
    if (width == 32) {
        mask = UINT32_MAX;
    } else {
        mask = (1u << width) - 1u;
    }
    uint32_t sign_bit = 1u << (width - 1);
    value = value & mask;

    if ((value & sign_bit) == 0) {
        return (int32_t)value;
    }
    return -1 - (int32_t)((~value) & mask);
}
