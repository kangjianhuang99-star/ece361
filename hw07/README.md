# ECE 361 Homework 7

## Build and test

- `make` builds the bracket checker.
- `./balance < inputs/balanced.txt` runs it on a sample input.
- `make test` runs the stack unit tests and bracket-checker regression tests.
- `make memcheck` rebuilds and runs both test suites with AddressSanitizer
  and UndefinedBehaviorSanitizer.
- `make clean` removes the compiled programs.

## Part 4: Memory errors

Each experiment ran under `make memcheck` on Ubuntu using a temporary
copy of the source. Each trial started from the working version.

### 1. Destroy the same stack twice

```text
==2315==ERROR: AddressSanitizer: heap-use-after-free on address 0x70112e5e0040 at pc 0x5575029e9c5c bp 0x7ffe3f73b790 sp 0x7ffe3f73b780
```

The second call to `istack_destroy` tried to read the data pointer from a
stack structure that the first call had already freed.

### 2. Push after destroying the stack

```text
==2368==ERROR: AddressSanitizer: heap-use-after-free on address 0x6e34219e0048 at pc 0x56e70c2a5d14 bp 0x7fffbdfab060 sp 0x7fffbdfab050
```

The call to `istack_push` tried to read fields in a stack structure that
had already been freed.

### 3. Omit the call to istack_destroy

```text
==2421==ERROR: LeakSanitizer: detected memory leaks
```

The program exited without freeing the heap allocations for the stack
structure and its backing array.
