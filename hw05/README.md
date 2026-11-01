# HW05: Combination Lock

A table-driven finite state machine for a lock with the code 3-1-4.

## Build and run

Run these commands from the hw05 directory:

```sh
make
./lock < data/codes.txt
```

## Test and clean

```sh
make test
make clean
```

The tests print PASS or FAIL for each check, followed by a summary.
A failed test produces a nonzero exit status.

## Inputs and states

- LOCKED: waiting for 3.
- GOT1: received 3, waiting for 1.
- GOT2: received 3-1, waiting for 4.
- OPEN: unlocked; digits have no effect.
- c: door-closing event.
- t: simulates expiration of the five-second timer.
- Other characters, including spaces and newlines, are ignored.

A wrong digit beeps and resets the lock. That digit is consumed;
it is not reused as the beginning of another attempt.

## State diagram

Arrow labels use EVENT/ACTION.

```text
LOCKED --RIGHT/NONE--> GOT1 --RIGHT/NONE--> GOT2 --RIGHT/UNLOCK--> OPEN

LOCKED --WRONG/BEEP------------> LOCKED
LOCKED --CLOSE,TIMEOUT/NONE-----> LOCKED

GOT1   --WRONG/BEEP------------> LOCKED
GOT1   --TIMEOUT/NONE----------> LOCKED
GOT1   --CLOSE/NONE------------> GOT1

GOT2   --WRONG/BEEP------------> LOCKED
GOT2   --TIMEOUT/NONE----------> LOCKED
GOT2   --CLOSE/NONE------------> GOT2

OPEN   --RIGHT,WRONG/NONE------> OPEN
OPEN   --CLOSE,TIMEOUT/LOCK----> LOCKED
```

The initial state is LOCKED. Arrows returning to the same state
are self-loops.

## Transition table

Each cell contains the next state and action.

| State | RIGHT | WRONG | CLOSE | TIMEOUT |
|-------|-------|-------|-------|---------|
| LOCKED | GOT1 / NONE | LOCKED / BEEP | LOCKED / NONE | LOCKED / NONE |
| GOT1 | GOT2 / NONE | LOCKED / BEEP | GOT1 / NONE | LOCKED / NONE |
| GOT2 | OPEN / UNLOCK | LOCKED / BEEP | GOT2 / NONE | LOCKED / NONE |
| OPEN | OPEN / NONE | OPEN / NONE | LOCKED / LOCK | LOCKED / LOCK |

## Implementation and tests

lock_event converts input characters into events.
lock_step performs one table lookup and returns the next state,
while writing the action through a pointer.

The 56 checks cover all 16 table entries, lock_step, character-to-event
conversion, digits while OPEN, closing, timeout, ignored characters,
and these sequences:

- 314: opens.
- 3314: remains locked.
- 31t4: remains locked.
- 314c: opens and then locks again.

Sequence tests also check the number of unlock, lock, and beep actions.
