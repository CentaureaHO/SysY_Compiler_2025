#include <bits/stdc++.h>
#include <parser/lexer.h>
#include <parser/token.h>
using namespace std;

extern int     curRow, curHeadCol, curTailCol;
extern YYSTYPE yylval;

int main()
{
    freopen("test.in", "r", stdin);
    int token;
    while ((token = yylex()))
    {
        printf("Token %s at (%d, %d)", getName(token), curRow, curHeadCol);
        if (token == INT_CONST)
            printf(": %d", yylval.int_val);
        else if (token == FLOAT_CONST)
            printf(": %.2f", yylval.float_val);
        else if (token == IDENT || token == STR_CONST)
            printf(": %s", yylval.str_val);
        printf("\n");
    }
}