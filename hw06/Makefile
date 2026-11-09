.RECIPEPREFIX := >

CC = gcc
CPPFLAGS = -I.
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lm

COMMON_OBJS = sensor.o panel.o scenario.o iom361_r4.o

.PHONY: all test clean

all: replay

replay: main.o $(COMMON_OBJS)
>$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

tests/test_hw06: tests/test_hw06.o $(COMMON_OBJS)
>$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c sensor.h panel.h scenario.h iom361_r4.h
>$(CC) $(CPPFLAGS) $(CFLAGS) -c main.c -o $@

sensor.o: sensor.c sensor.h iom361_r4.h
>$(CC) $(CPPFLAGS) $(CFLAGS) -c sensor.c -o $@

panel.o: panel.c panel.h iom361_r4.h
>$(CC) $(CPPFLAGS) $(CFLAGS) -c panel.c -o $@

scenario.o: scenario.c scenario.h
>$(CC) $(CPPFLAGS) $(CFLAGS) -c scenario.c -o $@

iom361_r4.o: iom361_r4.c iom361_r4.h
>$(CC) $(CPPFLAGS) $(CFLAGS) -c iom361_r4.c -o $@

tests/test_hw06.o: tests/test_hw06.c sensor.h panel.h scenario.h iom361_r4.h
>$(CC) $(CPPFLAGS) $(CFLAGS) -c tests/test_hw06.c -o $@

test: replay tests/test_hw06
>./tests/test_hw06
>sh ./regress.sh

clean:
>rm -f replay *.o tests/*.o tests/test_hw06 tests/scenario_test_input.tmp
