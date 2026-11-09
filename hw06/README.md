# ECE 361 Homework 6

Author: Alvin Huang

## Build and run

Run these commands from the hw06 directory:

    make
    ./replay scenarios/basic.txt

The project uses GCC, GNU Make, and the C math library.

## Tests

    make test

This runs the unit tests in tests/test_hw06.c, followed by
regress.sh, which compares each scenario's output with the
corresponding file in expected/.

The tests cover sensor conversion, unchanged readings after
a failed read, switch edges, preservation of other LEDs,
scenario parsing, invalid input, and values holding between lines.

The regression scenarios cover repeated presses, held switches,
multiple switches, changing readings, and crossing midnight.

## Clean

    make clean

This removes the executable, object files, test executable,
and temporary test input.

## Modules

- sensor: reads the sensor registers, masks the low 20 bits,
  converts the values, and rounds them to one decimal place.
- panel: detects switch presses and releases, and updates LEDs
  using read-modify-write.
- scenario: loads up to 1000 data rows without malloc and
  finds the values in force at a tick.
- main: replays the scenario and prints one trace line per tick.

The supplied iom361_r4.h and iom361_r4.c are unchanged.

## Scenario format

    # Example
    start 1791273600
    0 21.5 40.0 0x0
    2 21.5 40.0 0x1
    4 21.5 40.0 0x0

Blank lines and comments beginning with # are allowed.
Inline comments are also allowed.

Each data row contains a nonnegative integer tick, temperature,
humidity, and a hexadecimal switch value. Ticks must strictly
increase. Values hold between rows.

Replay includes tick 0 through the final tick.
If the first data row is after tick 0, the initialized hardware
values apply until that row: 23.5 C, 75.0 %RH, and all switches off.

scenario_load returns 0 on success, a positive physical line
number for invalid input, or -1 for an I/O error.

All trace timestamps are printed in UTC using gmtime and strftime.
Each newly pressed switch toggles its corresponding LED.
Holding or releasing a switch does not toggle the LED.
