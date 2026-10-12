# AI Usage - Homework 2

## AI Tools Used

ChatGPT (OpenAI)

## How AI Was Used

I used ChatGPT to help understand the homework requirements,
learn how to organize C code into separate modules, and
understand compilation and testing commands.

AI also helped me troubleshoot compiler and linker errors,
including a missing main function and an undeclared
histogram_print function.

## Example of an AI-Assisted Error and Fix

During development, the compiler reported that
histogram_print was implicitly declared.

The problem was that the function declaration was missing
from histogram.h.

I added the function declaration and recompiled the program.

## Verification

I checked the results using:

    make
    make test

The unit tests passed, and the regression tests matched
the expected output.