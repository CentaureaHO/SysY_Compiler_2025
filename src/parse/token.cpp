#include <parse/token.h>
#include <lexer.h>
#include <yacc.h>
using namespace std;

int curRow     = 1;
int curHeadCol = 0, curTailCol = 0;
int commentLines = 0;

int TokenParser::parse(istream& input)
{
    FILE*  inputFile = tmpfile();
    string content((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    fputs(content.c_str(), inputFile);
    rewind(inputFile);
    yyin = inputFile;

    int token;
    int token_count = 0;
    while ((token = yylex()))
    {
        stringstream ss;

        if (token == INT_CONST) { ss << yylval.int_val; }
        else if (token == FLOAT_CONST) { ss << yylval.float_val; }
        else if (token == STR_CONST || token == IDENT || token == ERR_TOKEN) { ss << yylval.str_val; }
        else { ss << ""; }

        tokens.push_back(make_tuple(token, yytext, ss.str(), curRow, curHeadCol));
    }

    fclose(inputFile);
    return token_count;
}

const char* getName(int token)
{
    switch (token)
    {
#define X(a, b) \
    case b: return #a;
        TOKEN_LIST
#undef X
        default: return "Unknown token";
    }
}
