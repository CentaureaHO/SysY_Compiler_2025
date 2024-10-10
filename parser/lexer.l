%{
    #include <string>
    #include <memory>

    #include <parser/yacc.hpp>
    #include <parser/scanner.h>
    #include <parser/location.hh>
    #include <parser/driver.h>

    using namespace Yacc;

    static location loc;

    #define YY_USER_ACTION  loc.columns (yyleng);
    #define yywrap() 1
    #define RETT(x, y) \
        return Parser::make_##x(y);

    extern int convertToInt(const char* str, const char end);
    extern float convertToFloatDec(const char* str);
    extern float convertToFloatHex(const char* str);
%}

%option c++
%option noyywrap debug
%option yyclass="Scanner"
%option prefix="Yacc_"

%%

\n              {
                    loc.lines(yyleng);
                    loc.step();
                }

[ \t\f\r\v]+    { loc.step(); }

"int"               { RETT(INT, loc) }
"float"             { RETT(FLOAT, loc) }
"void"              { RETT(VOID, loc) }
"if"                { RETT(IF, loc) }
"else"              { RETT(ELSE, loc) }
"for"               { RETT(FOR, loc) }
"while"             { RETT(WHILE, loc) }
"continue"          { RETT(CONTINUE, loc) }
"break"             { RETT(BREAK, loc) }
"switch"            { RETT(SWITCH, loc) }
"case"              { RETT(CASE, loc) }
"goto"              { RETT(GOTO, loc) }
"do"                { RETT(DO, loc) }
"return"            { RETT(RETURN, loc) }

"const"             { RETT(CONST, loc) }

";"                 { RETT(SEMICOLON, loc) }
","                 { RETT(COMMA, loc) }
"="                 { RETT(ASSIGN, loc) }
"!"                 { RETT(NOT, loc) }
"|"                 { RETT(BITOR, loc) }
"&"                 { RETT(BITAND, loc) }
">"                 { RETT(GT, loc) }
"<"                 { RETT(LT, loc) }
"+"                 { RETT(PLUS, loc) }
"-"                 { RETT(MINUS, loc) }
"*"                 { RETT(STAR, loc) }
"/"                 { RETT(SLASH, loc) }
"."                 { RETT(DOT, loc) }
">="                { RETT(GE, loc) }
"<="                { RETT(LE, loc) }
"=="                { RETT(EQ, loc) }

"//".*              { 
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_SLASH_COMMENT(str, loc);
}

"/*"([^*]|\*[^/])*"*/" {
    for (int i = 0; yytext[i] != '\0'; ++i) {
        if (yytext[i] == '\n') {
            yytext[i] = ' ';
        }
    }
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_SLASH_COMMENT(str, loc);
}

"/*" {
    _driver.reportError(loc, yytext);
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_ERR_TOKEN(str, loc);
}

"*/" {
    _driver.reportError(loc, yytext);
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_ERR_TOKEN(str, loc);
}

\"[^\"]*\"          { 
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_STR_CONST(str, loc); 
}

[+-]?[0-9]*\.[0-9]*([eE][+-]?[0-9]+)? {
    return Parser::make_FLOAT_CONST(convertToFloatDec(yytext), loc);
}

[+-]?0[xX][0-9a-fA-F]*\.[0-9a-fA-F]*([pP][+-]?[0-9a-fA-F]+)? {
    return Parser::make_FLOAT_CONST(convertToFloatHex(yytext), loc);
}

[+-]?0[xX][0-9a-fA-F]* {
    return Parser::make_INT_CONST(convertToInt(yytext, '\0'), loc);
}

[+-]?0[0-7]* {
    return Parser::make_INT_CONST(convertToInt(yytext, '\0'), loc);
}

[+-]?[1-9][0-9]* {
    return Parser::make_INT_CONST(convertToInt(yytext, '\0'), loc);
}

"("                 { RETT(LPAREN, loc) }
")"                 { RETT(RPAREN, loc) }
"["                 { RETT(LBRACKET, loc) }
"]"                 { RETT(RBRACKET, loc) }
"{"                 { RETT(LBRACE, loc) }
"}"                 { RETT(RBRACE, loc) }

[A-Za-z_]+[A-Za-z0-9_]* {
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_IDENT(str, loc); 
}

<<EOF>>         { RETT(END, loc); }

%%