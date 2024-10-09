#ifndef __TOKEN_H__
#define __TOKEN_H__

#define TOKEN_LIST \
    X(YYEOF, 0) \
    X(YYerror, 256) \
    X(YYUNDEF, 257) \
    X(INT_CONST, 258) \
    X(FLOAT_CONST, 259) \
    X(IDENT, 260) \
    X(STR_CONST, 261) \
    X(INT, 262) \
    X(FLOAT, 263) \
    X(VOID, 264) \
    X(IF, 265) \
    X(ELSE, 266) \
    X(FOR, 267) \
    X(WHILE, 268) \
    X(CONTINUE, 269) \
    X(BREAK, 270) \
    X(SWITCH, 271) \
    X(CASE, 272) \
    X(GOTO, 273) \
    X(DO, 274) \
    X(RETURN, 275) \
    X(CONST, 276) \
    X(SEMICOLON, 277) \
    X(COMMA, 278) \
    X(LPAREN, 279) \
    X(RPAREN, 280) \
    X(LBRACKET, 281) \
    X(RBRACKET, 282) \
    X(LBRACE, 283) \
    X(RBRACE, 284) \
    X(NOT, 285) \
    X(BITOR, 286) \
    X(BITAND, 287) \
    X(DOT, 288) \
    X(SLASH_COMMENT, 289) \
    X(PLUS, 290) \
    X(MINUS, 291) \
    X(STAR, 292) \
    X(SLASH, 293) \
    X(GT, 294) \
    X(GE, 295) \
    X(LT, 296) \
    X(LE, 297) \
    X(EQ, 298) \
    X(ASSIGN, 299) \
    X(END, -1)

const char* getName(int token);


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


#endif  // __TOKEN_H__
