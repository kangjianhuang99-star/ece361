CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck clean

all: treestats

treestats: treestats.c bst.c bst.h
	$(CC) $(CFLAGS) treestats.c bst.c -o $@

tests/test_bst: tests/test_bst.c bst.c bst.h
	$(CC) $(CFLAGS) tests/test_bst.c bst.c -o $@

test: tests/test_bst
	./tests/test_bst

memcheck:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) $(SANFLAGS)" all test

clean:
	$(RM) treestats tests/test_bst
