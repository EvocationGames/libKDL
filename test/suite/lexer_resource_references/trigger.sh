#!/usr/bin/env bash
SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
SCRIPT_DIR=${SCRIPT_DIR//$(pwd)\//}
INPUT="$SCRIPT_DIR/input.kdl"
OUTPUT="$SCRIPT_DIR/result.txt"

build/kdl-test lexemes "$INPUT" > test/output.txt
if ! cmp --silent "$OUTPUT" test/output.txt; then
  diff "$OUTPUT" test/output.txt
  exit 1
fi