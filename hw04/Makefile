CC = gcc
CPPFLAGS = -I.
CFLAGS = -std=c11 -Wall -Wextra

all: events

events: events.o tally.o
	$(CC) $(CFLAGS) events.o tally.o -o events

events.o: events.c tally.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c events.c -o events.o

tally.o: tally.c tally.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c tally.c -o tally.o

tests/test_tally.o: tests/test_tally.c tally.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c tests/test_tally.c -o tests/test_tally.o

tests/test_tally: tests/test_tally.o tally.o
	$(CC) $(CFLAGS) tests/test_tally.o tally.o -o tests/test_tally

test: tests/test_tally
	./tests/test_tally

clean:
	rm -f events events.o tally.o tests/test_tally tests/test_tally.o

.PHONY: all test clean
