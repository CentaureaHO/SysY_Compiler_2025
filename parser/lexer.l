%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parser/token.h>

YYSTYPE yylval;

int curRow     = 1;
int curHeadCol = 0, curTailCol = 0;

extern int convertToInt(const char* str, const char end);

extern float convertToFloatDec(const char* str);
extern float convertToFloatHex(const char* str);

#define NEW_ROW                     \
        ++curRow;                   \
        curTailCol = 0;

#define YY_USER_ACTION          \
    curHeadCol = curTailCol;    \
    curTailCol += yyleng;

void yyerror(char* s, ...);

%}

%%

\n                  { NEW_ROW }
[ \t\f\r\v]+        { /* empty */ }

"int"               { return INT; }
"float"             { return FLOAT; }
"void"              { return VOID; }
"if"                { return IF; }
"else"              { return ELSE; }
"for"               { return FOR; }
"while"             { return WHILE; }
"continue"          { return CONTINUE; }
"break"             { return BREAK; }
"switch"            { return SWITCH; }
"case"              { return CASE; }
"goto"              { return GOTO; }
"do"                { return DO; }
"return"            { return RETURN; }

"const"             { return CONST; }

";"                 { return SEMICOLON; }
","                 { return COMMA; }
"="                 { return ASSIGN; }
"!"                 { return NOT; }
"|"                 { return BITOR; }
"&"                 { return BITAND; }
">"                 { return GT; }
"<"                 { return LT; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return SLASH; }
"."                 { return DOT; }
">="                { return GE; }
"<="                { return LE; }
"=="                { return EQ; }

\"[^\"]*\" {
    yylval.str_val = strdup(yytext);
    return STR_CONST;
}

[+-]?[0-9]*\.[0-9]*([eE][+-]?[0-9]+)? {
    yylval.float_val = atof(yytext);
    return FLOAT_CONST;
}

[+-]?0[xX][0-9a-fA-F]*\.[0-9a-fA-F]*([pP][+-]?[0-9a-fA-F]+)? {
    yylval.float_val = atof(yytext);
    return FLOAT_CONST;
}

[+-]?0[0-7]* {
    yylval.int_val = atoi(yytext);
    return INT_CONST;
}

[+-]?[1-9][0-9]* {
    yylval.int_val = atoi(yytext);
    return INT_CONST;
}

[+-]?0[xX][0-9]* {
    yylval.int_val = atoi(yytext);
    return INT_CONST;
}

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"["                 { return LBRACKET; }
"]"                 { return RBRACKET; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }

[A-Za-z_]+[A-Za-z0-9_]* {
    yylval.str_val = strdup(yytext);
    return IDENT;
}

%%