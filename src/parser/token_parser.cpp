#include <parser/token_parser.h>
using namespace std;

extern int     curRow, curHeadCol, curTailCol;
extern YYSTYPE yylval;

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
        else if (token == STR_CONST || token == IDENT) { ss << yylval.str_val; }
        else { ss << ""; }

        tokens.push_back(make_tuple(token, yytext, ss.str(), curRow, curHeadCol));
    }

    fclose(inputFile);
    return token_count;
}