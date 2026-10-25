# Homework 4

## Build, test, and run

Run these commands from the hw04 directory:

```sh
make
make test
./events < data/door.txt
```

Remove generated files with:

```sh
make clean
```

## Opaque type experiment

I created opaque_check.c with:

```c
#include "tally.h"

int inspect(tally_t *t)
{
    return t->n;
}
```

Then I ran:

```sh
gcc -std=c11 -Wall -Wextra -fsyntax-only opaque_check.c
```

The compiler reported:

```text
opaque_check.c:5:13: error: invalid use of incomplete typedef ‘tally_t’ {aka ‘struct tally’}
```

Only tally.c defines the members of struct tally. Other files can call its functions but cannot directly read or change its members. This helps keep names sorted and counts correct. It also lets us change the internal structure without changing the code that uses the tally.

I removed opaque_check.c after this experiment.
