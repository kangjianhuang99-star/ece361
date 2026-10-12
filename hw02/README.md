# ECE 361 - Homework 2

## Overview

This program reads temperature and humidity data,
calculates statistics, and prints a histogram.

## Module Structure

main.c
- Handles command-line arguments.
- Coordinates the program and prints results.

reading.c / reading.h
- Reads and parses input data.
- Skips comments and invalid lines.

stats.c / stats.h
- Calculates minimum, maximum, and mean.
- Finds the longest consecutive run above a threshold.
- Uses recursion to calculate the maximum.

histogram.c / histogram.h
- Generates and prints the temperature histogram.

## Module Dependency Diagram

main.c
  |-- reading.h --> reading.c
  |-- stats.h --> stats.c
  |-- histogram.h --> histogram.c

## Build

Run:

    make

## Test

Run:

    make test

The tests include unit tests and regression tests.

## Clean

Run:

    make clean