#!/bin/sh

set -e
set -u

for file in $1/*.c; do
	clang-format -i "$file"
done

for file in $1/*.h; do
	clang-format -i "$file"
done
