# AI Usage

Tool: OpenAI ChatGPT

ChatGPT generated the initial implementations of sensor.h,
sensor.c, panel.h, panel.c, scenario.h, scenario.c, and main.c.

It also supplied the unit tests, regression scenarios, expected
traces, regression script, Makefile, and documentation drafts.
It explained the register conversions, switch edge detection,
LED read-modify-write, scenario lookup, and UTC time formatting.

I created the files in my repository, compiled the code, ran the
programs and tests locally, and shared the terminal results
during the process.

Verification completed:
- make clean
- make
- make test
- 34 unit tests passed, 0 failed
- 2 regression tests passed, 0 failed

One corrected assumption:
ChatGPT initially planned to create hw06, but the directory
listing showed that it already existed. We used the existing
hw06 directory.

The instructor-provided iom361_r4.h and iom361_r4.c were copied
into hw06 without modification.
