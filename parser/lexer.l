%{
    #include <stdexcept>
    #include <string>
    #include <memory>

    #include <parser/yacc.hpp>
    #include <parser/scanner.h>
    #include <parser/location.hh>
    #include <parser/driver.h>

    using namespace Yacc;

    static location loc;

    #define YY_USER_ACTION      \
        loc.step();             \
        loc.columns(yyleng);

    #define yywrap() 1
    #define RETT(x, y) \
        return Parser::make_##x(y);

    #define TAB_WIDTH 4

    extern long long convertToInt(const char* str, const char end, bool& isLongLong);
    extern float convertToFloatDec(const char* str);
    extern float convertToFloatHex(const char* str);

    int handleTab();
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

[ \f\r\v]+    { /* empty */ }
[\t]    {
    loc.columns(handleTab());
}

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
"%"                 { RETT(MOD, loc) }
"."                 { RETT(DOT, loc) }
">="                { RETT(GE, loc) }
"<="                { RETT(LE, loc) }
"=="                { RETT(EQ, loc) }
"!="                { RETT(NEQ, loc) }
"&&"                { RETT(AND, loc) }
"||"                { RETT(OR, loc) }

"//".*              { 
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_SLASH_COMMENT(str, loc);
}

"/*"([^*]|\*[^/])*"*/" {
    int cnt = 0;
    for (int i = 0; yytext[i] != '\0'; ++i) {
        if (yytext[i] == '\n') {
            ++cnt;
            yytext[i] = ' ';
        }
    }
    for (int i = 0; i < cnt; ++i) loc.lines();
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

[A-Za-z_]+[A-Za-z0-9_]* {
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_IDENT(str, loc); 
}

\"[^\"]*\"          { 
    auto str = std::make_shared<std::string>(yytext);
    return Parser::make_STR_CONST(str, loc); 
}

[0-9]*\.[0-9]*([eE][+-]?[0-9]+)? {
    try {
        return Parser::make_FLOAT_CONST(convertToFloatDec(yytext), loc);
    } catch (const std::exception& e) {
        _driver.reportError(loc, std::string("Error parsing float: ") + e.what());
        auto str = std::make_shared<std::string>(yytext);
        return Parser::make_ERR_TOKEN(str, loc);
    }
}

[0-9]*[eE][+-]?[0-9]+ {
    try {
        return Parser::make_FLOAT_CONST(convertToFloatDec(yytext), loc);
    } catch (const std::exception& e) {
        _driver.reportError(loc, std::string("Error parsing float: ") + e.what());
        auto str = std::make_shared<std::string>(yytext);
        return Parser::make_ERR_TOKEN(str, loc);
    }
}

0[xX][0-9a-fA-F]*\.[0-9a-fA-F]*([pP][+-]?[0-9a-fA-F]+)? {
    try {
        return Parser::make_FLOAT_CONST(convertToFloatHex(yytext), loc);
    } catch (const std::exception& e) {
        _driver.reportError(loc, std::string("Error parsing hexadecimal float: ") + e.what());
        auto str = std::make_shared<std::string>(yytext);
        return Parser::make_ERR_TOKEN(str, loc);
    }
}

0[xX][0-9a-fA-F][pP][+-]?[0-9a-fA-F]+ {
    try {
        return Parser::make_FLOAT_CONST(convertToFloatHex(yytext), loc);
    } catch (const std::exception& e) {
        _driver.reportError(loc, std::string("Error parsing hexadecimal float: ") + e.what());
        auto str = std::make_shared<std::string>(yytext);
        return Parser::make_ERR_TOKEN(str, loc);
    }
}

0[xX][0-9a-fA-F]* {
    try {
        bool isLL = false;
        long long result = convertToInt(yytext, '\0', isLL);
        if (isLL) return Parser::make_LL_CONST(result, loc);
        return Parser::make_INT_CONST(static_cast<int>(result), loc);
    } catch (const std::exception& e) {
        _driver.reportError(loc, std::string("Error parsing hexadecimal int: ") + e.what());
        auto str = std::make_shared<std::string>(yytext);
        return Parser::make_ERR_TOKEN(str, loc);
    }
}

0[0-7]* {
    try {
        bool isLL = false;
        long long result = convertToInt(yytext, '\0', isLL);
        if (isLL) return Parser::make_LL_CONST(result, loc);
        return Parser::make_INT_CONST(static_cast<int>(result), loc);
    } catch (const std::exception& e) {
        _driver.reportError(loc, std::string("Error parsing hexadecimal int: ") + e.what());
        auto str = std::make_shared<std::string>(yytext);
        return Parser::make_ERR_TOKEN(str, loc);
    }
}

[1-9][0-9]* {
    try {
        bool isLL = false;
        long long result = convertToInt(yytext, '\0', isLL);
        if (isLL) return Parser::make_LL_CONST(result, loc);
        return Parser::make_INT_CONST(static_cast<int>(result), loc);
    } catch (const std::exception& e) {
        _driver.reportError(loc, std::string("Error parsing hexadecimal int: ") + e.what());
        auto str = std::make_shared<std::string>(yytext);
        return Parser::make_ERR_TOKEN(str, loc);
    }
}

"("                 { RETT(LPAREN, loc) }
")"                 { RETT(RPAREN, loc) }
"["                 { RETT(LBRACKET, loc) }
"]"                 { RETT(RBRACKET, loc) }
"{"                 { RETT(LBRACE, loc) }
"}"                 { RETT(RBRACE, loc) }

<<EOF>>         { RETT(END, loc); }

%%

int handleTab()
{
    int& c = loc.begin.column;
    return TAB_WIDTH - ((c - 1) % TAB_WIDTH) - 1;
}