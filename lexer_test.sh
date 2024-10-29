#!/bin/bash

# Step 1: 执行 make lexer_test
echo "Running 'make lexer_test'..."
make lexer_test
if [ $? -ne 0 ]; then
  echo "Error: Failed to build lexer_test."
  exit 1
fi

output_dir="test_output"
token_dir="$output_dir/token"
ast_dir="$output_dir/ast"
mkdir -p "$token_dir"
mkdir -p "$ast_dir"

input_dir="testcase"
sy_files=$(find "$input_dir" -type f -name "*.sy")
if [ -z "$sy_files" ]; then
  echo "No .sy files found in $input_dir"
  exit 1
fi

for input_file in $sy_files; do
  filename=$(basename -- "$input_file")
  name="${filename%.*}"

  lexer_output="$token_dir/${name}.token"
  # echo "Running lexer mode for $input_file..."
  ./bin/lexer_test lexer "$input_file" "$lexer_output"

  parser_output="$ast_dir/${name}.ast"
  # echo "Running parser mode for $input_file..."
  ./bin/lexer_test parser "$input_file" "$parser_output"
done

echo "All tests completed. Outputs saved to $token_dir and $ast_dir."