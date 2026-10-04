CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I.
OBJECTS = bits.o status.o tests/test_bits.o

.PHONY: all test clean

all: $(OBJECTS)

bits.o: bits.c bits.h
status.o: status.c status.h bits.h
tests/test_bits.o: tests/test_bits.c bits.h status.h

tests/test_bits: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

test: tests/test_bits
	./tests/test_bits

clean:
	rm -f $(OBJECTS) tests/test_bits
