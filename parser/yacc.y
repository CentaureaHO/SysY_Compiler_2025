%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char* s);
int yylex();
%}

%union {
    int int_val;
    float float_val;
    char* str_val;
}

%token <int_val> INT_CONST
%token <float_val> FLOAT_CONST
%token <str_val> IDENT STR_CONST
%token INT FLOAT VOID IF ELSE FOR WHILE CONTINUE BREAK SWITCH CASE GOTO DO RETURN CONST
%token SEMICOLON COMMA LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE NOT BITOR BITAND DOT SLASH_COMMENT

%token PLUS MINUS
%token STAR SLASH
%token GT GE LT LE EQ ASSIGN

%%

program:
    /* todo */

%%

void yyerror(const char* s)
{
    fprintf(stderr, "Error: %s\n", s);
}
