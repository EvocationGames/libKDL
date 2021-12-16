#!/usr/bin/env bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
INPUT="input.kdl"
OUTPUT="result.txt"

build/kdl-test --lexeme-output "$INPUT" "$OUTPUT"