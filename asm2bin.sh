INPUT_FILE="${1:-test.s}"
OUTPUT_BIN="${2:-test.bin}"
OBJ_FILE="${INPUT_FILE%.s}.o"

if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: Input file '$INPUT_FILE' not found"
    exit 1
fi

riscv64-unknown-linux-gnu-gcc "$INPUT_FILE" -c -o "$OBJ_FILE" -w
riscv64-unknown-linux-gnu-gcc -static "$OBJ_FILE" -L./lib -lsysy_riscv -o "$OUTPUT_BIN"

rm "$OBJ_FILE"

echo "Successfully compiled $INPUT_FILE to $OUTPUT_BIN"
