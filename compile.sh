#! /usr/bin/env bash

set -euo pipefail

HERE=$(dirname -- "$BASH_SOURCE")

clear

FLAGS_STANDARD='-std=c++23'

FLAGS_STRICT='-Werror -Wextra -Wall -pedantic -Wfatal-errors -Wshadow -Wconversion -fsanitize=undefined'
# `-fsanitize=undefined` will detect undefined behaviour at runtime (example: overflow, left shift by >=32 for uint32_t)
#    however, it doesn't seem like it's detecting multiplication overflow
#    it doesn't seem to be able to detect underflow as well

FLAGS_MISC='-Ofast'
# can't have both `-static` and `-fsanitize=undefined`

FLAGS="$FLAGS_STANDARD $FLAGS_STRICT $FLAGS_MISC"

g++ $FLAGS "$HERE/fixed-point.cpp" -c -o "$HERE/fixed-point.o"

g++ $FLAGS "$HERE/test.cpp" -o "$HERE/test" "$HERE/fixed-point.o"
