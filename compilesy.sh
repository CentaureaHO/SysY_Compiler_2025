#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Check if the input file is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <input_file.s>"
    exit 1
fi

# Input and output file paths
INPUT_FILE=$1
OUTPUT_FILE="${INPUT_FILE%.s}.run"

# Check if input file exists and has .s extension
if [[ ! -f "$INPUT_FILE" ]]; then
    echo "Error: Input file '$INPUT_FILE' does not exist."
    exit 1
fi

# Compiler settings
CFLAGS_TOBY="-march=rv64imfd -mabi=lp64d -fno-builtin -ffreestanding -mcmodel=medany -O0 -Wall -fno-pic -fno-common -g -misa-spec=2.2 -ffunction-sections -fdata-sections"
NEWLIB_BASE="$(pwd)/newlib-4.5.0.20241231/riscv64-unknown-elf-binary/newlib/"

# Compilation command
echo "Compiling $INPUT_FILE..."
riscv64-unknown-elf-gcc -I $(pwd)/newlib-4.5.0.20241231/newlib/libc/include -L "$NEWLIB_BASE" crt0.S $INPUT_FILE sylib.c tobylib.c libzzy.c clz_tab.c \
    -T toby.ld $CFLAGS_TOBY -nostdlib -nostartfiles -static \
    -Wl,-Map=toby.map -Wl,--no-relax -Wl,--gc-sections -g -mtune=rocket -o $INPUT_FILE.elf -lc -lgcc

# Convert the output to a binary format
echo "Generating binary output: $OUTPUT_FILE"
riscv64-unknown-elf-objcopy -O binary $INPUT_FILE.elf "$OUTPUT_FILE"

# Clean up
echo "Compilation successful: $OUTPUT_FILE"