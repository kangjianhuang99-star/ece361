#!/bin/sh
set -eu

cd "$(dirname "$0")"

actual=$(mktemp)
trap 'rm -f "$actual"' 0
trap 'exit 1' 1 2 15

passed=0
failed=0

for scenario in scenarios/*.txt
do
    [ -f "$scenario" ] || continue
    name=${scenario##*/}

    if ./replay "$scenario" > "$actual" &&
       diff -u "expected/$name" "$actual"
    then
        printf 'PASS: %s\n' "$name"
        passed=$((passed + 1))
    else
        printf 'FAIL: %s\n' "$name"
        failed=$((failed + 1))
    fi
done

printf 'Regression: %d passed, %d failed\n' "$passed" "$failed"

[ "$passed" -gt 0 ] && [ "$failed" -eq 0 ]

