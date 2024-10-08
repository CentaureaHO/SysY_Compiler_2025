#ifndef __TOKEN_PARSER_H__
#define __TOKEN_PARSER_H__

#include <parser/lexer.h>
#include <parser/token.h>
#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class TokenParser
{
  public:
    // token, lexeme, property, row, col
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

#endif