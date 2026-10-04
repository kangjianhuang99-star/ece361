# ECE 361 HW1

This library provides 32-bit bit-manipulation functions and unpacks a 16-bit thermostat status word.

## Build and test

Run `make` to compile each C source file into an object file.

Run `make test` to build and run the test program.

Run `make clean` to remove generated object files and the test executable.

## Functions and input ranges

- `print_binary(x, width)` prints the lowest `width` bits of `x`, most significant bit first. `width` must be 1 through 32. Bits are grouped in fours from the least significant end; the leading group can contain fewer than four bits.
- `get_field(word, pos, width)` returns the selected bits shifted down to bit 0. Valid inputs have `pos` from 0 through 31, `width` from 1 through 32, and `pos + width` at most 32.
- `set_field(word, pos, width, value)` replaces the selected bits with the lowest `width` bits of `value`. It uses the same valid ranges as `get_field`.
- `sign_extend(value, width)` interprets the lowest `width` bits as a two's-complement number. `width` must be 1 through 32.
- `status_unpack(word)` decodes the thermostat fields from a 16-bit status word.

For `get_field` and `set_field`, an input outside the valid range is handled without an error: `get_field` returns 0, and `set_field` returns the original `word` unchanged.

A width of 32 is handled separately so the code does not shift by 32. `set_field` keeps only the lowest `width` bits of `value`.

## Thermostat status word

The `mode` field preserves its raw three-bit value. Values 0 through 4 are valid modes; values 5 through 7 remain visible as invalid values in `status_t.mode`. The `reserved` member contains bit 7; the specification requires that bit to be 0.
