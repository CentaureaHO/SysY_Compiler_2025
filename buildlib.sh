#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONFIG_FILE="$SCRIPT_DIR/toolchain.conf"

if [ ! -f "$CONFIG_FILE" ]; then
  echo "Configuration file toolchain.conf not found"
  exit 1
fi

RISCV_GCC=$(grep "^RISCV_GCC=" "$CONFIG_FILE" | cut -d'=' -f2)
RISCV_AR=$(grep "^RISCV_AR=" "$CONFIG_FILE" | cut -d'=' -f2)

clang -c lib/sylib.c -o lib/libsysy_x86.o
ar rcs lib/libsysy_x86.a lib/libsysy_x86.o
rm lib/libsysy_x86.o

$RISCV_GCC -c lib/sylib.c -o lib/libsysy_riscv.o -static
$RISCV_AR rcs lib/libsysy_riscv.a lib/libsysy_riscv.o
rm lib/libsysy_riscv.o
