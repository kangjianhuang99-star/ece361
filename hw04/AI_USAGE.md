# AI Usage

## Tool

OpenAI Codex, used as an AI coding assistant and tutor.

## What I used it for

The assistant helped interpret the assignment, generated initial code for tally.h, tally.c, events.c, the automated tests, and the Makefile, and drafted this documentation.

It explained macros, structures, pointers, const, opaque types, string comparison and copying, sorted array insertion, input handling, and testing. It also helped interpret compiler diagnostics and terminal errors.

I entered and edited the files in Ubuntu, ran the build and tests, checked the sample output, and performed the opaque type experiment.

## One thing the model got wrong

The assistant assumed the events executable existed when it instructed me to run:

```sh
./events < data/door.txt
```

After the input file was created, the shell still reported:

```text
-bash: ./events: No such file or directory
```

The executable had not been built. We corrected this by compiling:

```sh
gcc -std=c11 -Wall -Wextra events.c tally.c -o events
```

The program then produced the expected output. The Makefile now provides the build steps through make.
