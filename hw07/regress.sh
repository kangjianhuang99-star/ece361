#!/bin/sh

cd "$(dirname "$0")" || exit 2

temp_dir=$(mktemp -d) || exit 2
trap 'rm -f "$temp_dir/stdout" "$temp_dir/stderr"; rmdir "$temp_dir"' EXIT

passed=0
total=0

for input in inputs/*.txt; do
    name=${input##*/}
    name=${name%.txt}
    total=$((total + 1))

    ./balance < "$input" > "$temp_dir/stdout" 2> "$temp_dir/stderr"
    actual_status=$?

    if ! expected_status=$(cat "expected/$name.status"); then
        printf 'FAIL: %s (missing expected exit status)\n' "$name"
        continue
    fi

    if [ "$actual_status" -eq "$expected_status" ] &&
       [ ! -s "$temp_dir/stderr" ] &&
       cmp -s "expected/$name.out" "$temp_dir/stdout"; then
        printf 'PASS: %s\n' "$name"
        passed=$((passed + 1))
    else
        printf 'FAIL: %s (expected exit %s, got %s)\n' \
            "$name" "$expected_status" "$actual_status"
        diff -u "expected/$name.out" "$temp_dir/stdout"
        cat "$temp_dir/stderr"
    fi
done

printf 'Regression summary: %s/%s passed\n' "$passed" "$total"
[ "$passed" -eq "$total" ]
