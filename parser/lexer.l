%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include <common/str_convert.h>
#include "yacc.h"

extern int curRow;
extern int curHeadCol, curTailCol;
extern int commentLines;

#define NEW_ROW                                 \
        curRow = curRow + 1 + commentLines;     \
        commentLines = 0;                       \
        curTailCol = 0;                         

#define YY_USER_ACTION          \
    curHeadCol = curTailCol;    \
    curTailCol += yyleng;

extern void yyerror(const char* format, ...);

%}

%%

\n                  { NEW_ROW }
[ \t\f\r\v]+        { /* mpty */ }

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

"//".*              { return SLASH_COMMENT; }

"/*"([^*]|\*[^/])*"*/" {
    for (int i = 0; yytext[i] != '\0'; ++i) {
        if (yytext[i] == '\n') {
            ++commentLines;
            yytext[i] = ' ';
        }
    }
    return SLASH_COMMENT;
}

"/*" {
    yyerror("Unclosed left comment %s at (%d, %d)\n", yytext, curRow, curHeadCol);
    yylval.str_val = strdup(yytext);
    return ERR_TOKEN;
}

"*/" {
    yyerror("Unclosed right comment %s at (%d, %d)\n", yytext, curRow, curHeadCol);
    yylval.str_val = strdup(yytext);
    return ERR_TOKEN;
}

\"[^\"]*\"          { yylval.str_val = strdup(yytext); return STR_CONST; }

[+-]?[0-9]*\.[0-9]*([eE][+-]?[0-9]+)? {
    yylval.float_val = convertToFloatDec(yytext);
    return FLOAT_CONST;
}

[+-]?0[xX][0-9a-fA-F]*\.[0-9a-fA-F]*([pP][+-]?[0-9a-fA-F]+)? {
    yylval.float_val = convertToFloatHex(yytext);
    return FLOAT_CONST;
}

[+-]?0[xX][0-9a-fA-F]* {
    yylval.int_val = convertToInt(yytext, '\0');
    return INT_CONST;
}

[+-]?0[0-7]* {
    yylval.int_val = convertToInt(yytext, '\0');
    return INT_CONST;
}

[+-]?[1-9][0-9]* {
    yylval.int_val = convertToInt(yytext, '\0');
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