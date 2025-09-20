#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_FILE="$SCRIPT_DIR/toolchain.conf"

if [ ! -f "$CONFIG_FILE" ]; then
  echo "Configuration file toolchain.conf not found"
  exit 1
fi

RISCV_GCC=$(grep "^RISCV_GCC=" "$CONFIG_FILE" | cut -d'=' -f2)
TEXT_ADDR=$(grep "^TEXT_ADDR=" "$CONFIG_FILE" | cut -d'=' -f2)

INPUT_FILE="${1:-test.s}"
OUTPUT_BIN="${2:-test.bin}"
OBJ_FILE="${INPUT_FILE%.s}.o"

if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: Input file '$INPUT_FILE' not found"
    exit 1
fi

$RISCV_GCC "$INPUT_FILE" -c -o "$OBJ_FILE" -w
$RISCV_GCC "$OBJ_FILE" -o "$OUTPUT_BIN"\
    -L./lib -lsysy_riscv\
    -static -mcmodel=medany\
    -Wl,--no-relax,-Ttext=$TEXT_ADDR

rm "$OBJ_FILE"

echo "Successfully compiled $INPUT_FILE to $OUTPUT_BIN"
