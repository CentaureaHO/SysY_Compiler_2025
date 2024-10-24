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
    #include <common/type/type_defs.h>
    #include <common/type/node/basic_node.h>
    #include <common/type/node/statement.h>
    #include <common/type/node/expression.h>
    #include <common/type/symtab/symbol_table.h>
    #include <common/type/node/helper.h>

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
%token <std::shared_ptr<std::string>> STR_CONST ERR_TOKEN SLASH_COMMENT

%token <std::shared_ptr<std::string>> IDENT 

%token INT FLOAT VOID 
%token IF ELSE FOR WHILE CONTINUE BREAK SWITCH CASE GOTO DO RETURN CONST
%token SEMICOLON COMMA LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE NOT BITOR BITAND DOT 
%token END

%left PLUS MINUS
%right UPLUS UMINUS
%token STAR SLASH
%token GT GE LT LE EQ ASSIGN MOD
%token NEQ AND OR

%nterm <OpCode> UNARY_OP

%nterm <Type*> TYPE

%nterm <InitNode*> INITIALIZER
%nterm <std::vector<InitNode*>*> INITIALIZER_LIST
%nterm <DefNode*> DEF
%nterm <std::vector<DefNode*>*> DEF_LIST

%nterm <ExprNode*> CONST_EXPR
%nterm <ExprNode*> BASIC_EXPR

%nterm <ExprNode*> UNARY_EXPR

%nterm <ExprNode*> MULDIV_EXPR
%nterm <ExprNode*> ADDSUB_EXPR
%nterm <ExprNode*> RELATIONAL_EXPR
%nterm <ExprNode*> EQUALITY_EXPR
%nterm <ExprNode*> LOGICAL_AND_EXPR
%nterm <ExprNode*> LOGICAL_OR_EXPR
%nterm <ExprNode*> ASSIGN_EXPR      /* 由于C语言中=语句也有返回值，因此定义ASSIGN为EXPR而非STMT */
%nterm <ExprNode*> EXPR

%nterm <ExprNode*> ARRAY_DIMESION_EXPR
%nterm <std::vector<ExprNode*>*> ARRAY_DIMESION_EXPR_LIST

%nterm <ExprNode*> LEFT_VAL_EXPR

%nterm <StmtNode*> EXPR_STMT
%nterm <StmtNode*> VAR_DECL_STMT
%nterm <StmtNode*> STMT

%nterm <ASTree*> PROGRAM
%start PROGRAM

%%

PROGRAM:
    STMT {
        std::cout << "program: STMT " << std::endl;
        $1->printAST(&std::cout, 0);
        $$ = new ASTree();
        driver.setAST($$);
        // delete $1;
    }
    | PROGRAM END {
        YYACCEPT;
    }
    ;

STMT:
    EXPR_STMT {
        $$ = $1;
    }
    | VAR_DECL_STMT {
        $$ = $1;
    }       
    ;

EXPR_STMT:
    EXPR SEMICOLON {
        $$ = new ExprStmt($1);
    }

VAR_DECL_STMT:
    TYPE DEF_LIST SEMICOLON {
        $$ = new VarDeclStmt($1, $2);
    }
    | CONST TYPE DEF_LIST SEMICOLON {
        $$ = new VarDeclStmt($2, $3, true);
    }

DEF:
    LEFT_VAL_EXPR {
        $$ = new DefNode($1, nullptr);
    }
    | LEFT_VAL_EXPR ASSIGN INITIALIZER {
        $$ = new DefNode($1, $3);
    }

DEF_LIST:
    DEF {
        $$ = new std::vector<DefNode*>();
        $$->push_back($1);
    }
    | DEF_LIST COMMA DEF {
        $$ = $1;
        $$->push_back($3);
    }
    ;

INITIALIZER:
    EXPR {
        $$ = new InitSingle($1);
    }
    | LBRACE INITIALIZER_LIST RBRACE {
        $$ = new InitMulti($2);
    }
    ;

INITIALIZER_LIST:
    INITIALIZER { 
        $$ = new std::vector<InitNode*>();
        $$->push_back($1);
    }
    | INITIALIZER_LIST COMMA INITIALIZER {
        $$ = $1;
        $$->push_back($3);
    }
    ;

ASSIGN_EXPR:
    LEFT_VAL_EXPR ASSIGN EXPR {
        $$ = new BinaryExpr(OpCode::Assign, $1, $3);
    }
    ;

EXPR:
    LOGICAL_OR_EXPR {
        $$ = $1;
    }
    | ASSIGN_EXPR {
        $$ = $1;
    }
    ;

LOGICAL_OR_EXPR:
    LOGICAL_AND_EXPR {
        $$ = $1;
    }
    | LOGICAL_OR_EXPR OR LOGICAL_AND_EXPR {
        $$ = new BinaryExpr(OpCode::Or, $1, $3);
    }
    ;

LOGICAL_AND_EXPR:
    EQUALITY_EXPR {
        $$ = $1;
    }
    | LOGICAL_AND_EXPR AND EQUALITY_EXPR {
        $$ = new BinaryExpr(OpCode::And, $1, $3);
    }
    ;

EQUALITY_EXPR:
    RELATIONAL_EXPR {
        $$ = $1;
    }
    | EQUALITY_EXPR EQ RELATIONAL_EXPR {
        $$ = new BinaryExpr(OpCode::Eq, $1, $3);
    }
    | EQUALITY_EXPR NEQ RELATIONAL_EXPR {
        $$ = new BinaryExpr(OpCode::Neq, $1, $3);
    }
    ;

RELATIONAL_EXPR:
    ADDSUB_EXPR {
        $$ = $1;
    }
    | RELATIONAL_EXPR GT ADDSUB_EXPR {
        $$ = new BinaryExpr(OpCode::Gt, $1, $3);
    }
    | RELATIONAL_EXPR GE ADDSUB_EXPR {
        $$ = new BinaryExpr(OpCode::Ge, $1, $3);
    }
    | RELATIONAL_EXPR LT ADDSUB_EXPR {
        $$ = new BinaryExpr(OpCode::Lt, $1, $3);
    }
    | RELATIONAL_EXPR LE ADDSUB_EXPR {
        $$ = new BinaryExpr(OpCode::Le, $1, $3);
    }
    ;

ADDSUB_EXPR:
    MULDIV_EXPR {
        $$ = $1;
    }
    | ADDSUB_EXPR PLUS MULDIV_EXPR {
        $$ = new BinaryExpr(OpCode::Add, $1, $3);
    }
    | ADDSUB_EXPR MINUS MULDIV_EXPR {
        $$ = new BinaryExpr(OpCode::Sub, $1, $3);
    }
    ;

MULDIV_EXPR:
    UNARY_EXPR {
        $$ = $1;
    }
    | MULDIV_EXPR STAR UNARY_EXPR {
        $$ = new BinaryExpr(OpCode::Mul, $1, $3);
    }
    | MULDIV_EXPR SLASH UNARY_EXPR {
        $$ = new BinaryExpr(OpCode::Div, $1, $3);
    }
    | MULDIV_EXPR MOD UNARY_EXPR {
        $$ = new BinaryExpr(OpCode::Mod, $1, $3);
    }
    ;

UNARY_EXPR:
    BASIC_EXPR {
        $$ = $1;
    }
    | UNARY_OP BASIC_EXPR {
        $$ = new UnaryExpr($1, $2);
    }
    ;

BASIC_EXPR:
    CONST_EXPR {
        $$ = $1;
    }
    | LEFT_VAL_EXPR {
        $$ = $1;
    }
    | LPAREN EXPR RPAREN {
        $$ = $2;
    }
    ;

ARRAY_DIMESION_EXPR:
    LBRACKET EXPR RBRACKET {
        $$ = $2;
    }
    ;

ARRAY_DIMESION_EXPR_LIST:
    ARRAY_DIMESION_EXPR {
        $$ = new std::vector<ExprNode*>();
        $$->push_back($1);
    }
    | ARRAY_DIMESION_EXPR_LIST ARRAY_DIMESION_EXPR {
        $$ = $1;
        $$->push_back($2);
    }
    ;

LEFT_VAL_EXPR:
    IDENT {
        Symbol::Entry* entry = Symbol::Entry::getEntry(*$1);
        $$ = new LeftValueExpr(entry, nullptr, -1);
    }
    | IDENT ARRAY_DIMESION_EXPR_LIST {
        Symbol::Entry* entry = Symbol::Entry::getEntry(*$1);
        $$ = new LeftValueExpr(entry, $2, -1);
    }
    ;

CONST_EXPR:
    INT_CONST {
        $$ = new ConstExpr($1);
    }
    | LL_CONST {
        $$ = new ConstExpr($1);
    }
    | FLOAT_CONST {
        $$ = new ConstExpr($1);
    }
    | STR_CONST {
        $$ = new ConstExpr($1);
    }
    ;

TYPE:
    INT {
        $$ = intType;
    }
    | FLOAT {
        $$ = floatType;
    }
    | VOID {
        $$ = voidType;
    }
    ;

UNARY_OP:
    PLUS {
        $$ = OpCode::Add;
    }
    | MINUS {
        $$ = OpCode::Sub;
    }
    | NOT {
        $$ = OpCode::Not;
    }
    ;

%%

void Yacc::Parser::error(const Yacc::location& location, const std::string& message)
{
    std::cerr << "msg: " << message << ", error happened at: " << location << std::endl;
}