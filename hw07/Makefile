CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
SANITIZER_FLAGS = -g -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=all

.PHONY: all test memcheck clean

all: balance

balance: balance.c istack.c istack.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) balance.c istack.c $(LDLIBS) -o $@

tests/test_istack: tests/test_istack.c istack.c istack.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -I. $(LDFLAGS) tests/test_istack.c istack.c $(LDLIBS) -o $@

test: balance tests/test_istack
	./tests/test_istack
	./regress.sh

memcheck:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) $(SANITIZER_FLAGS)" test

clean:
	rm -f balance tests/test_istack
