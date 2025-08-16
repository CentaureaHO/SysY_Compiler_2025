INPUT_FILE="${1:-test.ll}"
OUTPUT_BIN="${2:-test.bin}"
OBJ_FILE="${INPUT_FILE%.s}.o"

if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: Input file '$INPUT_FILE' not found"
    exit 1
fi

if [ -f "$OUTPUT_BIN" ]; then
    rm "$OUTPUT_BIN"
fi

clang "$INPUT_FILE" -c -o "$OBJ_FILE" -w
clang -static "$OBJ_FILE" -L./lib -lsysy_x86 -lutils_x86 -o "$OUTPUT_BIN"

rm "$OBJ_FILE"

echo "Successfully compiled $INPUT_FILE to $OUTPUT_BIN"
