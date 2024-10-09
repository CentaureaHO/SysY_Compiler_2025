import re
import sys
import os

def extract(filename):
    with open(filename, 'r') as file:
        content = file.read()

    match = re.search(r'enum\s+yytokentype\s*{([^}]*)}', content, re.DOTALL)
    if not match:
        return []

    enum_body = match.group(1)
    tokens = re.findall(r'(\w+)\s*=\s*(\d+)', enum_body)
    return tokens

def genmacro(tokens):
    x_macro_lines = []
    for name, value in tokens:
        x_macro_lines.append(f"    X({name}, {value}) \\")
    x_macro_lines.append("    X(END, -1)")
    return "\n".join(x_macro_lines)

def genfunc(tokens):
    getname_lines = []
    getname_lines.append("const char* getName(int token);\n")

    getname_lines.append(
        '''
#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class TokenParser
{
  public:
    std::vector<std::tuple<int, std::string, std::string, int, int>> tokens;

    template <typename InputStream>
    TokenParser(InputStream& input);

    int parse(std::istream& input);
};

template <typename InputStream>
TokenParser::TokenParser(InputStream& input)
{
    parse(input);
}
'''
    )
    return "\n".join(getname_lines)

def main():
    if len(sys.argv) != 3:
        print("Usage: python generate_x_macro.py <input_file> <output_file>")
        return

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    tokens = extract(input_file)
    if not tokens:
        return

    file_base_name = os.path.basename(output_file).replace('.', '_').upper()

    x_macro = genmacro(tokens)
    get_name_func = genfunc(tokens)

    with open(output_file, 'w') as file:
        file.write(f"#ifndef __{file_base_name}__\n")
        file.write(f"#define __{file_base_name}__\n\n")
        file.write("#define TOKEN_LIST \\\n")
        file.write(x_macro + "\n\n")
        file.write(get_name_func + "\n\n")
        file.write(f"#endif  // __{file_base_name}__\n")

if __name__ == "__main__":
    main()