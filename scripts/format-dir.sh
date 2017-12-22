#!/bin/sh

set -e
set -u

for file in $1/*.c; do
	clang-format "$file" >"$file"
done

for file in $1/*.h; do
	clang-format "$file" >"$file"
done
