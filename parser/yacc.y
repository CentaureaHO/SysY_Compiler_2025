%skeleton "lalr1.cc"
%require "3.2"

%define api.namespace { Yacc }
%define api.parser.class { Parser }
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%defines

%code requires
{
    #include <memory>
    #include <string>

    namespace Yacc
    {
        class Driver;
        class Scanner;
    }
}

%code top
{
/*
 * @ref https://github.com/ronnie88597/yacc_bison_practice/tree/master/ch3/3.05
 *
 * @参考范围：
 *      1. flex/bison的C++版本使用方式；
 *      2. Scanner类设计。    
 */
    #include <iostream>

    #include <parser/driver.h>
    #include <parser/location.hh>
    #include <parser/scanner.h>
    #include <parser/yacc.hpp>

    using namespace Yacc;

    static Parser::symbol_type yylex(Scanner& scanner, Driver &driver)
    {
        return scanner.nextToken(); 
    }
}

%lex-param { Yacc::Scanner& scanner }
%lex-param { Yacc::Driver& driver }
%parse-param { Yacc::Scanner& scanner }
%parse-param { Yacc::Driver& driver }

%locations

%define parse.error verbose
%define api.token.prefix {TOKEN_}

%token <int> INT_CONST
%token <long long> LL_CONST
%token <float> FLOAT_CONST
%token <std::shared_ptr<std::string>> IDENT STR_CONST ERR_TOKEN SLASH_COMMENT
%token INT FLOAT VOID IF ELSE FOR WHILE CONTINUE BREAK SWITCH CASE GOTO DO RETURN CONST
%token SEMICOLON COMMA LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE NOT BITOR BITAND DOT 
%token END

%token PLUS MINUS
%token STAR SLASH
%token GT GE LT LE EQ ASSIGN MOD
%token NEQ AND OR

%%

program:
    /* todo */

%%

void Yacc::Parser::error(const Yacc::location& location, const std::string& message)
{
    std::cerr << "msg: " << message << ", error happened at: " << location << std::endl;
}