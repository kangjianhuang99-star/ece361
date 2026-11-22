## Insertion order

Increasing order (`seq 1 1023 | ./treestats`):
```text
n 1023, height 1023
```

Shuffled order (`seq 1 1023 | sort -R | ./treestats`):
```text
n 1023, height 23
```

Inserting the keys in increasing order made every new key a right child, so the tree had height 1023 and finding the deepest key took 1023 visits. With the same keys shuffled, the tree had height 23 in this run, so finding the deepest key took only 23 visits.

## Build and test

Run these commands from the hw08 directory.

- `make` builds `treestats`.
- `make test` builds and runs the BST tests.
- `make memcheck` rebuilds with AddressSanitizer and UndefinedBehaviorSanitizer, then runs the tests.
- `make clean` removes the compiled programs.

To read integers from a file, run `./treestats < input.txt`.

## Empty-tree search crash

The test `test_empty_tree` creates an empty tree and calls
`bst_contains(t, 31, &visits)`. The first diagnostic from
`make memcheck` with the original search loop was:

```text
bst.c:137:19: runtime error: member access within null pointer of type 'const struct node'
```

The original example never hit this bug because its main function
inserted seven keys before calling search. The fixed loop checks that
the current node exists before reading its data.
