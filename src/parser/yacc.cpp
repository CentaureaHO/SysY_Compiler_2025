// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

// "%code top" blocks.
#line 31 "parser/yacc.y"

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

static Parser::symbol_type yylex(Scanner& scanner, Driver& driver) { return scanner.nextToken(); }

#line 62 "parser/yacc.cpp"

#include <parser/yacc.hpp>

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h>  // FIXME: INFRINGES ON USER NAME SPACE.
#define YY_(msgid) dgettext("bison-runtime", msgid)
#endif
#endif
#ifndef YY_
#define YY_(msgid) msgid
#endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
#if defined __GNUC__ && !defined __EXCEPTIONS
#define YY_EXCEPTIONS 0
#else
#define YY_EXCEPTIONS 1
#endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)                                  \
    do                                                                   \
        if (N)                                                           \
        {                                                                \
            (Current).begin = YYRHSLOC(Rhs, 1).begin;                    \
            (Current).end   = YYRHSLOC(Rhs, N).end;                      \
        }                                                                \
        else { (Current).begin = (Current).end = YYRHSLOC(Rhs, 0).end; } \
    while (false)
#endif

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
#define YYCDEBUG \
    if (yydebug_) (*yycdebug_)

#define YY_SYMBOL_PRINT(Title, Symbol)     \
    do {                                   \
        if (yydebug_)                      \
        {                                  \
            *yycdebug_ << Title << ' ';    \
            yy_print_(*yycdebug_, Symbol); \
            *yycdebug_ << '\n';            \
        }                                  \
    } while (false)

#define YY_REDUCE_PRINT(Rule)                 \
    do {                                      \
        if (yydebug_) yy_reduce_print_(Rule); \
    } while (false)

#define YY_STACK_PRINT()                 \
    do {                                 \
        if (yydebug_) yy_stack_print_(); \
    } while (false)

#else  // !YYDEBUG

#define YYCDEBUG \
    if (false) std::cerr
#define YY_SYMBOL_PRINT(Title, Symbol) YY_USE(Symbol)
#define YY_REDUCE_PRINT(Rule) static_cast<void>(0)
#define YY_STACK_PRINT() static_cast<void>(0)

#endif  // !YYDEBUG

#define yyerrok (yyerrstatus_ = 0)
#define yyclearin (yyla.clear())

#define YYACCEPT goto yyacceptlab
#define YYABORT goto yyabortlab
#define YYERROR goto yyerrorlab
#define YYRECOVERING() (!!yyerrstatus_)

#line 4 "parser/yacc.y"
namespace Yacc
{
#line 162 "parser/yacc.cpp"

    /// Build a parser object.
    Parser ::Parser(Yacc::Scanner& scanner_yyarg, Yacc::Driver& driver_yyarg)
#if YYDEBUG
        : yydebug_(false),
          yycdebug_(&std::cerr),
#else
        :
#endif
          scanner(scanner_yyarg),
          driver(driver_yyarg)
    {}

    Parser ::~Parser() {}

    Parser ::syntax_error::~syntax_error() YY_NOEXCEPT YY_NOTHROW {}

    /*---------.
    | symbol.  |
    `---------*/

    // by_state.
    Parser ::by_state::by_state() YY_NOEXCEPT : state(empty_state) {}

    Parser ::by_state::by_state(const by_state& that) YY_NOEXCEPT : state(that.state) {}

    void Parser ::by_state::clear() YY_NOEXCEPT { state = empty_state; }

    void Parser ::by_state::move(by_state& that)
    {
        state = that.state;
        that.clear();
    }

    Parser ::by_state::by_state(state_type s) YY_NOEXCEPT : state(s) {}

    Parser ::symbol_kind_type Parser ::by_state::kind() const YY_NOEXCEPT
    {
        if (state == empty_state)
            return symbol_kind::S_YYEMPTY;
        else
            return YY_CAST(symbol_kind_type, yystos_[+state]);
    }

    Parser ::stack_symbol_type::stack_symbol_type() {}

    Parser ::stack_symbol_type::stack_symbol_type(YY_RVREF(stack_symbol_type) that)
        : super_type(YY_MOVE(that.state), YY_MOVE(that.location))
    {
        switch (that.kind())
        {
            case symbol_kind::S_PROGRAM:  // PROGRAM
                value.YY_MOVE_OR_COPY<ASTree*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_DEF:  // DEF
                value.YY_MOVE_OR_COPY<DefNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_ASSIGN_EXPR:          // ASSIGN_EXPR
            case symbol_kind::S_EXPR:                 // EXPR
            case symbol_kind::S_LOGICAL_OR_EXPR:      // LOGICAL_OR_EXPR
            case symbol_kind::S_LOGICAL_AND_EXPR:     // LOGICAL_AND_EXPR
            case symbol_kind::S_EQUALITY_EXPR:        // EQUALITY_EXPR
            case symbol_kind::S_RELATIONAL_EXPR:      // RELATIONAL_EXPR
            case symbol_kind::S_ADDSUB_EXPR:          // ADDSUB_EXPR
            case symbol_kind::S_MULDIV_EXPR:          // MULDIV_EXPR
            case symbol_kind::S_UNARY_EXPR:           // UNARY_EXPR
            case symbol_kind::S_BASIC_EXPR:           // BASIC_EXPR
            case symbol_kind::S_FUNC_CALL_EXPR:       // FUNC_CALL_EXPR
            case symbol_kind::S_ARRAY_DIMESION_EXPR:  // ARRAY_DIMESION_EXPR
            case symbol_kind::S_LEFT_VAL_EXPR:        // LEFT_VAL_EXPR
            case symbol_kind::S_CONST_EXPR:           // CONST_EXPR
                value.YY_MOVE_OR_COPY<ExprNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_FUNC_PARAM_DEF:  // FUNC_PARAM_DEF
                value.YY_MOVE_OR_COPY<FuncParamDefNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_INITIALIZER:  // INITIALIZER
                value.YY_MOVE_OR_COPY<InitNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_UNARY_OP:  // UNARY_OP
                value.YY_MOVE_OR_COPY<OpCode>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_STMT:            // STMT
            case symbol_kind::S_CONTINUE_STMT:   // CONTINUE_STMT
            case symbol_kind::S_BREAK_STMT:      // BREAK_STMT
            case symbol_kind::S_BLOCK_STMT:      // BLOCK_STMT
            case symbol_kind::S_EXPR_STMT:       // EXPR_STMT
            case symbol_kind::S_VAR_DECL_STMT:   // VAR_DECL_STMT
            case symbol_kind::S_FUNC_DECL_STMT:  // FUNC_DECL_STMT
            case symbol_kind::S_RETURN_STMT:     // RETURN_STMT
            case symbol_kind::S_WHILE_STMT:      // WHILE_STMT
            case symbol_kind::S_IF_STMT:         // IF_STMT
            case symbol_kind::S_FOR_INIT_STMT:   // FOR_INIT_STMT
            case symbol_kind::S_FOR_INCR_STMT:   // FOR_INCR_STMT
            case symbol_kind::S_FOR_STMT:        // FOR_STMT
                value.YY_MOVE_OR_COPY<StmtNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_TYPE:  // TYPE
                value.YY_MOVE_OR_COPY<Type*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_FLOAT_CONST:  // FLOAT_CONST
                value.YY_MOVE_OR_COPY<float>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_INT_CONST:  // INT_CONST
                value.YY_MOVE_OR_COPY<int>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_LL_CONST:  // LL_CONST
                value.YY_MOVE_OR_COPY<long long>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_STR_CONST:      // STR_CONST
            case symbol_kind::S_ERR_TOKEN:      // ERR_TOKEN
            case symbol_kind::S_SLASH_COMMENT:  // SLASH_COMMENT
            case symbol_kind::S_IDENT:          // IDENT
                value.YY_MOVE_OR_COPY<std::shared_ptr<std::string> >(YY_MOVE(that.value));
                break;

            case symbol_kind::S_DEF_LIST:  // DEF_LIST
                value.YY_MOVE_OR_COPY<std::vector<DefNode*>*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_EXPR_LIST:                 // EXPR_LIST
            case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST:  // ARRAY_DIMESION_EXPR_LIST
                value.YY_MOVE_OR_COPY<std::vector<ExprNode*>*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_FUNC_PARAM_DEF_LIST:  // FUNC_PARAM_DEF_LIST
                value.YY_MOVE_OR_COPY<std::vector<FuncParamDefNode*>*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_INITIALIZER_LIST:  // INITIALIZER_LIST
                value.YY_MOVE_OR_COPY<std::vector<InitNode*>*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_STMT_LIST:  // STMT_LIST
                value.YY_MOVE_OR_COPY<std::vector<StmtNode*>*>(YY_MOVE(that.value));
                break;

            default: break;
        }

#if 201103L <= YY_CPLUSPLUS
        // that is emptied.
        that.state = empty_state;
#endif
    }

    Parser ::stack_symbol_type::stack_symbol_type(state_type s, YY_MOVE_REF(symbol_type) that)
        : super_type(s, YY_MOVE(that.location))
    {
        switch (that.kind())
        {
            case symbol_kind::S_PROGRAM:  // PROGRAM
                value.move<ASTree*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_DEF:  // DEF
                value.move<DefNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_ASSIGN_EXPR:          // ASSIGN_EXPR
            case symbol_kind::S_EXPR:                 // EXPR
            case symbol_kind::S_LOGICAL_OR_EXPR:      // LOGICAL_OR_EXPR
            case symbol_kind::S_LOGICAL_AND_EXPR:     // LOGICAL_AND_EXPR
            case symbol_kind::S_EQUALITY_EXPR:        // EQUALITY_EXPR
            case symbol_kind::S_RELATIONAL_EXPR:      // RELATIONAL_EXPR
            case symbol_kind::S_ADDSUB_EXPR:          // ADDSUB_EXPR
            case symbol_kind::S_MULDIV_EXPR:          // MULDIV_EXPR
            case symbol_kind::S_UNARY_EXPR:           // UNARY_EXPR
            case symbol_kind::S_BASIC_EXPR:           // BASIC_EXPR
            case symbol_kind::S_FUNC_CALL_EXPR:       // FUNC_CALL_EXPR
            case symbol_kind::S_ARRAY_DIMESION_EXPR:  // ARRAY_DIMESION_EXPR
            case symbol_kind::S_LEFT_VAL_EXPR:        // LEFT_VAL_EXPR
            case symbol_kind::S_CONST_EXPR:           // CONST_EXPR
                value.move<ExprNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_FUNC_PARAM_DEF:  // FUNC_PARAM_DEF
                value.move<FuncParamDefNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_INITIALIZER:  // INITIALIZER
                value.move<InitNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_UNARY_OP:  // UNARY_OP
                value.move<OpCode>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_STMT:            // STMT
            case symbol_kind::S_CONTINUE_STMT:   // CONTINUE_STMT
            case symbol_kind::S_BREAK_STMT:      // BREAK_STMT
            case symbol_kind::S_BLOCK_STMT:      // BLOCK_STMT
            case symbol_kind::S_EXPR_STMT:       // EXPR_STMT
            case symbol_kind::S_VAR_DECL_STMT:   // VAR_DECL_STMT
            case symbol_kind::S_FUNC_DECL_STMT:  // FUNC_DECL_STMT
            case symbol_kind::S_RETURN_STMT:     // RETURN_STMT
            case symbol_kind::S_WHILE_STMT:      // WHILE_STMT
            case symbol_kind::S_IF_STMT:         // IF_STMT
            case symbol_kind::S_FOR_INIT_STMT:   // FOR_INIT_STMT
            case symbol_kind::S_FOR_INCR_STMT:   // FOR_INCR_STMT
            case symbol_kind::S_FOR_STMT:        // FOR_STMT
                value.move<StmtNode*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_TYPE:  // TYPE
                value.move<Type*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_FLOAT_CONST:  // FLOAT_CONST
                value.move<float>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_INT_CONST:  // INT_CONST
                value.move<int>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_LL_CONST:  // LL_CONST
                value.move<long long>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_STR_CONST:      // STR_CONST
            case symbol_kind::S_ERR_TOKEN:      // ERR_TOKEN
            case symbol_kind::S_SLASH_COMMENT:  // SLASH_COMMENT
            case symbol_kind::S_IDENT:          // IDENT
                value.move<std::shared_ptr<std::string> >(YY_MOVE(that.value));
                break;

            case symbol_kind::S_DEF_LIST:  // DEF_LIST
                value.move<std::vector<DefNode*>*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_EXPR_LIST:                 // EXPR_LIST
            case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST:  // ARRAY_DIMESION_EXPR_LIST
                value.move<std::vector<ExprNode*>*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_FUNC_PARAM_DEF_LIST:  // FUNC_PARAM_DEF_LIST
                value.move<std::vector<FuncParamDefNode*>*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_INITIALIZER_LIST:  // INITIALIZER_LIST
                value.move<std::vector<InitNode*>*>(YY_MOVE(that.value));
                break;

            case symbol_kind::S_STMT_LIST:  // STMT_LIST
                value.move<std::vector<StmtNode*>*>(YY_MOVE(that.value));
                break;

            default: break;
        }

        // that is emptied.
        that.kind_ = symbol_kind::S_YYEMPTY;
    }

#if YY_CPLUSPLUS < 201103L
    Parser ::stack_symbol_type& Parser ::stack_symbol_type::operator=(const stack_symbol_type& that)
    {
        state = that.state;
        switch (that.kind())
        {
            case symbol_kind::S_PROGRAM:  // PROGRAM
                value.copy<ASTree*>(that.value);
                break;

            case symbol_kind::S_DEF:  // DEF
                value.copy<DefNode*>(that.value);
                break;

            case symbol_kind::S_ASSIGN_EXPR:          // ASSIGN_EXPR
            case symbol_kind::S_EXPR:                 // EXPR
            case symbol_kind::S_LOGICAL_OR_EXPR:      // LOGICAL_OR_EXPR
            case symbol_kind::S_LOGICAL_AND_EXPR:     // LOGICAL_AND_EXPR
            case symbol_kind::S_EQUALITY_EXPR:        // EQUALITY_EXPR
            case symbol_kind::S_RELATIONAL_EXPR:      // RELATIONAL_EXPR
            case symbol_kind::S_ADDSUB_EXPR:          // ADDSUB_EXPR
            case symbol_kind::S_MULDIV_EXPR:          // MULDIV_EXPR
            case symbol_kind::S_UNARY_EXPR:           // UNARY_EXPR
            case symbol_kind::S_BASIC_EXPR:           // BASIC_EXPR
            case symbol_kind::S_FUNC_CALL_EXPR:       // FUNC_CALL_EXPR
            case symbol_kind::S_ARRAY_DIMESION_EXPR:  // ARRAY_DIMESION_EXPR
            case symbol_kind::S_LEFT_VAL_EXPR:        // LEFT_VAL_EXPR
            case symbol_kind::S_CONST_EXPR:           // CONST_EXPR
                value.copy<ExprNode*>(that.value);
                break;

            case symbol_kind::S_FUNC_PARAM_DEF:  // FUNC_PARAM_DEF
                value.copy<FuncParamDefNode*>(that.value);
                break;

            case symbol_kind::S_INITIALIZER:  // INITIALIZER
                value.copy<InitNode*>(that.value);
                break;

            case symbol_kind::S_UNARY_OP:  // UNARY_OP
                value.copy<OpCode>(that.value);
                break;

            case symbol_kind::S_STMT:            // STMT
            case symbol_kind::S_CONTINUE_STMT:   // CONTINUE_STMT
            case symbol_kind::S_BREAK_STMT:      // BREAK_STMT
            case symbol_kind::S_BLOCK_STMT:      // BLOCK_STMT
            case symbol_kind::S_EXPR_STMT:       // EXPR_STMT
            case symbol_kind::S_VAR_DECL_STMT:   // VAR_DECL_STMT
            case symbol_kind::S_FUNC_DECL_STMT:  // FUNC_DECL_STMT
            case symbol_kind::S_RETURN_STMT:     // RETURN_STMT
            case symbol_kind::S_WHILE_STMT:      // WHILE_STMT
            case symbol_kind::S_IF_STMT:         // IF_STMT
            case symbol_kind::S_FOR_INIT_STMT:   // FOR_INIT_STMT
            case symbol_kind::S_FOR_INCR_STMT:   // FOR_INCR_STMT
            case symbol_kind::S_FOR_STMT:        // FOR_STMT
                value.copy<StmtNode*>(that.value);
                break;

            case symbol_kind::S_TYPE:  // TYPE
                value.copy<Type*>(that.value);
                break;

            case symbol_kind::S_FLOAT_CONST:  // FLOAT_CONST
                value.copy<float>(that.value);
                break;

            case symbol_kind::S_INT_CONST:  // INT_CONST
                value.copy<int>(that.value);
                break;

            case symbol_kind::S_LL_CONST:  // LL_CONST
                value.copy<long long>(that.value);
                break;

            case symbol_kind::S_STR_CONST:      // STR_CONST
            case symbol_kind::S_ERR_TOKEN:      // ERR_TOKEN
            case symbol_kind::S_SLASH_COMMENT:  // SLASH_COMMENT
            case symbol_kind::S_IDENT:          // IDENT
                value.copy<std::shared_ptr<std::string> >(that.value);
                break;

            case symbol_kind::S_DEF_LIST:  // DEF_LIST
                value.copy<std::vector<DefNode*>*>(that.value);
                break;

            case symbol_kind::S_EXPR_LIST:                 // EXPR_LIST
            case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST:  // ARRAY_DIMESION_EXPR_LIST
                value.copy<std::vector<ExprNode*>*>(that.value);
                break;

            case symbol_kind::S_FUNC_PARAM_DEF_LIST:  // FUNC_PARAM_DEF_LIST
                value.copy<std::vector<FuncParamDefNode*>*>(that.value);
                break;

            case symbol_kind::S_INITIALIZER_LIST:  // INITIALIZER_LIST
                value.copy<std::vector<InitNode*>*>(that.value);
                break;

            case symbol_kind::S_STMT_LIST:  // STMT_LIST
                value.copy<std::vector<StmtNode*>*>(that.value);
                break;

            default: break;
        }

        location = that.location;
        return *this;
    }

    Parser ::stack_symbol_type& Parser ::stack_symbol_type::operator=(stack_symbol_type& that)
    {
        state = that.state;
        switch (that.kind())
        {
            case symbol_kind::S_PROGRAM:  // PROGRAM
                value.move<ASTree*>(that.value);
                break;

            case symbol_kind::S_DEF:  // DEF
                value.move<DefNode*>(that.value);
                break;

            case symbol_kind::S_ASSIGN_EXPR:          // ASSIGN_EXPR
            case symbol_kind::S_EXPR:                 // EXPR
            case symbol_kind::S_LOGICAL_OR_EXPR:      // LOGICAL_OR_EXPR
            case symbol_kind::S_LOGICAL_AND_EXPR:     // LOGICAL_AND_EXPR
            case symbol_kind::S_EQUALITY_EXPR:        // EQUALITY_EXPR
            case symbol_kind::S_RELATIONAL_EXPR:      // RELATIONAL_EXPR
            case symbol_kind::S_ADDSUB_EXPR:          // ADDSUB_EXPR
            case symbol_kind::S_MULDIV_EXPR:          // MULDIV_EXPR
            case symbol_kind::S_UNARY_EXPR:           // UNARY_EXPR
            case symbol_kind::S_BASIC_EXPR:           // BASIC_EXPR
            case symbol_kind::S_FUNC_CALL_EXPR:       // FUNC_CALL_EXPR
            case symbol_kind::S_ARRAY_DIMESION_EXPR:  // ARRAY_DIMESION_EXPR
            case symbol_kind::S_LEFT_VAL_EXPR:        // LEFT_VAL_EXPR
            case symbol_kind::S_CONST_EXPR:           // CONST_EXPR
                value.move<ExprNode*>(that.value);
                break;

            case symbol_kind::S_FUNC_PARAM_DEF:  // FUNC_PARAM_DEF
                value.move<FuncParamDefNode*>(that.value);
                break;

            case symbol_kind::S_INITIALIZER:  // INITIALIZER
                value.move<InitNode*>(that.value);
                break;

            case symbol_kind::S_UNARY_OP:  // UNARY_OP
                value.move<OpCode>(that.value);
                break;

            case symbol_kind::S_STMT:            // STMT
            case symbol_kind::S_CONTINUE_STMT:   // CONTINUE_STMT
            case symbol_kind::S_BREAK_STMT:      // BREAK_STMT
            case symbol_kind::S_BLOCK_STMT:      // BLOCK_STMT
            case symbol_kind::S_EXPR_STMT:       // EXPR_STMT
            case symbol_kind::S_VAR_DECL_STMT:   // VAR_DECL_STMT
            case symbol_kind::S_FUNC_DECL_STMT:  // FUNC_DECL_STMT
            case symbol_kind::S_RETURN_STMT:     // RETURN_STMT
            case symbol_kind::S_WHILE_STMT:      // WHILE_STMT
            case symbol_kind::S_IF_STMT:         // IF_STMT
            case symbol_kind::S_FOR_INIT_STMT:   // FOR_INIT_STMT
            case symbol_kind::S_FOR_INCR_STMT:   // FOR_INCR_STMT
            case symbol_kind::S_FOR_STMT:        // FOR_STMT
                value.move<StmtNode*>(that.value);
                break;

            case symbol_kind::S_TYPE:  // TYPE
                value.move<Type*>(that.value);
                break;

            case symbol_kind::S_FLOAT_CONST:  // FLOAT_CONST
                value.move<float>(that.value);
                break;

            case symbol_kind::S_INT_CONST:  // INT_CONST
                value.move<int>(that.value);
                break;

            case symbol_kind::S_LL_CONST:  // LL_CONST
                value.move<long long>(that.value);
                break;

            case symbol_kind::S_STR_CONST:      // STR_CONST
            case symbol_kind::S_ERR_TOKEN:      // ERR_TOKEN
            case symbol_kind::S_SLASH_COMMENT:  // SLASH_COMMENT
            case symbol_kind::S_IDENT:          // IDENT
                value.move<std::shared_ptr<std::string> >(that.value);
                break;

            case symbol_kind::S_DEF_LIST:  // DEF_LIST
                value.move<std::vector<DefNode*>*>(that.value);
                break;

            case symbol_kind::S_EXPR_LIST:                 // EXPR_LIST
            case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST:  // ARRAY_DIMESION_EXPR_LIST
                value.move<std::vector<ExprNode*>*>(that.value);
                break;

            case symbol_kind::S_FUNC_PARAM_DEF_LIST:  // FUNC_PARAM_DEF_LIST
                value.move<std::vector<FuncParamDefNode*>*>(that.value);
                break;

            case symbol_kind::S_INITIALIZER_LIST:  // INITIALIZER_LIST
                value.move<std::vector<InitNode*>*>(that.value);
                break;

            case symbol_kind::S_STMT_LIST:  // STMT_LIST
                value.move<std::vector<StmtNode*>*>(that.value);
                break;

            default: break;
        }

        location = that.location;
        // that is emptied.
        that.state = empty_state;
        return *this;
    }
#endif

    template <typename Base>
    void Parser ::yy_destroy_(const char* yymsg, basic_symbol<Base>& yysym) const
    {
        if (yymsg) YY_SYMBOL_PRINT(yymsg, yysym);
    }

#if YYDEBUG
    template <typename Base>
    void Parser ::yy_print_(std::ostream& yyo, const basic_symbol<Base>& yysym) const
    {
        std::ostream& yyoutput = yyo;
        YY_USE(yyoutput);
        if (yysym.empty())
            yyo << "empty symbol";
        else
        {
            symbol_kind_type yykind = yysym.kind();
            yyo << (yykind < YYNTOKENS ? "token" : "nterm") << ' ' << yysym.name() << " (" << yysym.location << ": ";
            YY_USE(yykind);
            yyo << ')';
        }
    }
#endif

    void Parser ::yypush_(const char* m, YY_MOVE_REF(stack_symbol_type) sym)
    {
        if (m) YY_SYMBOL_PRINT(m, sym);
        yystack_.push(YY_MOVE(sym));
    }

    void Parser ::yypush_(const char* m, state_type s, YY_MOVE_REF(symbol_type) sym)
    {
#if 201103L <= YY_CPLUSPLUS
        yypush_(m, stack_symbol_type(s, std::move(sym)));
#else
        stack_symbol_type ss(s, sym);
        yypush_(m, ss);
#endif
    }

    void Parser ::yypop_(int n) YY_NOEXCEPT { yystack_.pop(n); }

#if YYDEBUG
    std::ostream& Parser ::debug_stream() const { return *yycdebug_; }

    void Parser ::set_debug_stream(std::ostream& o) { yycdebug_ = &o; }

    Parser ::debug_level_type Parser ::debug_level() const { return yydebug_; }

    void Parser ::set_debug_level(debug_level_type l) { yydebug_ = l; }
#endif  // YYDEBUG

    Parser ::state_type Parser ::yy_lr_goto_state_(state_type yystate, int yysym)
    {
        int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
        if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
            return yytable_[yyr];
        else
            return yydefgoto_[yysym - YYNTOKENS];
    }

    bool Parser ::yy_pact_value_is_default_(int yyvalue) YY_NOEXCEPT { return yyvalue == yypact_ninf_; }

    bool Parser ::yy_table_value_is_error_(int yyvalue) YY_NOEXCEPT { return yyvalue == yytable_ninf_; }

    int Parser ::operator()() { return parse(); }

    int Parser ::parse()
    {
        int yyn;
        /// Length of the RHS of the rule being reduced.
        int yylen = 0;

        // Error handling.
        int yynerrs_     = 0;
        int yyerrstatus_ = 0;

        /// The lookahead symbol.
        symbol_type yyla;

        /// The locations where the error started and ended.
        stack_symbol_type yyerror_range[3];

        /// The return value of parse ().
        int yyresult;

#if YY_EXCEPTIONS
        try
#endif  // YY_EXCEPTIONS
        {
            YYCDEBUG << "Starting parse\n";

            /* Initialize the stack.  The initial state will be set in
               yynewstate, since the latter expects the semantical and the
               location values to have been already stored, initialize these
               stacks with a primary value.  */
            yystack_.clear();
            yypush_(YY_NULLPTR, 0, YY_MOVE(yyla));

        /*-----------------------------------------------.
        | yynewstate -- push a new symbol on the stack.  |
        `-----------------------------------------------*/
        yynewstate:
            YYCDEBUG << "Entering state " << int(yystack_[0].state) << '\n';
            YY_STACK_PRINT();

            // Accept?
            if (yystack_[0].state == yyfinal_) YYACCEPT;

            goto yybackup;

        /*-----------.
        | yybackup.  |
        `-----------*/
        yybackup:
            // Try to take a decision without lookahead.
            yyn = yypact_[+yystack_[0].state];
            if (yy_pact_value_is_default_(yyn)) goto yydefault;

            // Read a lookahead token.
            if (yyla.empty())
            {
                YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
                try
#endif  // YY_EXCEPTIONS
                {
                    symbol_type yylookahead(yylex(scanner, driver));
                    yyla.move(yylookahead);
                }
#if YY_EXCEPTIONS
                catch (const syntax_error& yyexc)
                {
                    YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
                    error(yyexc);
                    goto yyerrlab1;
                }
#endif  // YY_EXCEPTIONS
            }
            YY_SYMBOL_PRINT("Next token is", yyla);

            if (yyla.kind() == symbol_kind::S_YYerror)
            {
                // The scanner already issued an error message, process directly
                // to error recovery.  But do not keep the error token as
                // lookahead, it is too special and may lead us to an endless
                // loop in error recovery. */
                yyla.kind_ = symbol_kind::S_YYUNDEF;
                goto yyerrlab1;
            }

            /* If the proper action on seeing token YYLA.TYPE is to reduce or
               to detect an error, take that action.  */
            yyn += yyla.kind();
            if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind()) { goto yydefault; }

            // Reduce or error.
            yyn = yytable_[yyn];
            if (yyn <= 0)
            {
                if (yy_table_value_is_error_(yyn)) goto yyerrlab;
                yyn = -yyn;
                goto yyreduce;
            }

            // Count tokens shifted since error; after three, turn off error status.
            if (yyerrstatus_) --yyerrstatus_;

            // Shift the lookahead token.
            yypush_("Shifting", state_type(yyn), YY_MOVE(yyla));
            goto yynewstate;

        /*-----------------------------------------------------------.
        | yydefault -- do the default action for the current state.  |
        `-----------------------------------------------------------*/
        yydefault:
            yyn = yydefact_[+yystack_[0].state];
            if (yyn == 0) goto yyerrlab;
            goto yyreduce;

        /*-----------------------------.
        | yyreduce -- do a reduction.  |
        `-----------------------------*/
        yyreduce:
            yylen = yyr2_[yyn];
            {
                stack_symbol_type yylhs;
                yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
                /* Variants are always initialized to an empty instance of the
                   correct type. The default '$$ = $1' action is NOT applied
                   when using variants.  */
                switch (yyr1_[yyn])
                {
                    case symbol_kind::S_PROGRAM:  // PROGRAM
                        yylhs.value.emplace<ASTree*>();
                        break;

                    case symbol_kind::S_DEF:  // DEF
                        yylhs.value.emplace<DefNode*>();
                        break;

                    case symbol_kind::S_ASSIGN_EXPR:          // ASSIGN_EXPR
                    case symbol_kind::S_EXPR:                 // EXPR
                    case symbol_kind::S_LOGICAL_OR_EXPR:      // LOGICAL_OR_EXPR
                    case symbol_kind::S_LOGICAL_AND_EXPR:     // LOGICAL_AND_EXPR
                    case symbol_kind::S_EQUALITY_EXPR:        // EQUALITY_EXPR
                    case symbol_kind::S_RELATIONAL_EXPR:      // RELATIONAL_EXPR
                    case symbol_kind::S_ADDSUB_EXPR:          // ADDSUB_EXPR
                    case symbol_kind::S_MULDIV_EXPR:          // MULDIV_EXPR
                    case symbol_kind::S_UNARY_EXPR:           // UNARY_EXPR
                    case symbol_kind::S_BASIC_EXPR:           // BASIC_EXPR
                    case symbol_kind::S_FUNC_CALL_EXPR:       // FUNC_CALL_EXPR
                    case symbol_kind::S_ARRAY_DIMESION_EXPR:  // ARRAY_DIMESION_EXPR
                    case symbol_kind::S_LEFT_VAL_EXPR:        // LEFT_VAL_EXPR
                    case symbol_kind::S_CONST_EXPR:           // CONST_EXPR
                        yylhs.value.emplace<ExprNode*>();
                        break;

                    case symbol_kind::S_FUNC_PARAM_DEF:  // FUNC_PARAM_DEF
                        yylhs.value.emplace<FuncParamDefNode*>();
                        break;

                    case symbol_kind::S_INITIALIZER:  // INITIALIZER
                        yylhs.value.emplace<InitNode*>();
                        break;

                    case symbol_kind::S_UNARY_OP:  // UNARY_OP
                        yylhs.value.emplace<OpCode>();
                        break;

                    case symbol_kind::S_STMT:            // STMT
                    case symbol_kind::S_CONTINUE_STMT:   // CONTINUE_STMT
                    case symbol_kind::S_BREAK_STMT:      // BREAK_STMT
                    case symbol_kind::S_BLOCK_STMT:      // BLOCK_STMT
                    case symbol_kind::S_EXPR_STMT:       // EXPR_STMT
                    case symbol_kind::S_VAR_DECL_STMT:   // VAR_DECL_STMT
                    case symbol_kind::S_FUNC_DECL_STMT:  // FUNC_DECL_STMT
                    case symbol_kind::S_RETURN_STMT:     // RETURN_STMT
                    case symbol_kind::S_WHILE_STMT:      // WHILE_STMT
                    case symbol_kind::S_IF_STMT:         // IF_STMT
                    case symbol_kind::S_FOR_INIT_STMT:   // FOR_INIT_STMT
                    case symbol_kind::S_FOR_INCR_STMT:   // FOR_INCR_STMT
                    case symbol_kind::S_FOR_STMT:        // FOR_STMT
                        yylhs.value.emplace<StmtNode*>();
                        break;

                    case symbol_kind::S_TYPE:  // TYPE
                        yylhs.value.emplace<Type*>();
                        break;

                    case symbol_kind::S_FLOAT_CONST:  // FLOAT_CONST
                        yylhs.value.emplace<float>();
                        break;

                    case symbol_kind::S_INT_CONST:  // INT_CONST
                        yylhs.value.emplace<int>();
                        break;

                    case symbol_kind::S_LL_CONST:  // LL_CONST
                        yylhs.value.emplace<long long>();
                        break;

                    case symbol_kind::S_STR_CONST:      // STR_CONST
                    case symbol_kind::S_ERR_TOKEN:      // ERR_TOKEN
                    case symbol_kind::S_SLASH_COMMENT:  // SLASH_COMMENT
                    case symbol_kind::S_IDENT:          // IDENT
                        yylhs.value.emplace<std::shared_ptr<std::string> >();
                        break;

                    case symbol_kind::S_DEF_LIST:  // DEF_LIST
                        yylhs.value.emplace<std::vector<DefNode*>*>();
                        break;

                    case symbol_kind::S_EXPR_LIST:                 // EXPR_LIST
                    case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST:  // ARRAY_DIMESION_EXPR_LIST
                        yylhs.value.emplace<std::vector<ExprNode*>*>();
                        break;

                    case symbol_kind::S_FUNC_PARAM_DEF_LIST:  // FUNC_PARAM_DEF_LIST
                        yylhs.value.emplace<std::vector<FuncParamDefNode*>*>();
                        break;

                    case symbol_kind::S_INITIALIZER_LIST:  // INITIALIZER_LIST
                        yylhs.value.emplace<std::vector<InitNode*>*>();
                        break;

                    case symbol_kind::S_STMT_LIST:  // STMT_LIST
                        yylhs.value.emplace<std::vector<StmtNode*>*>();
                        break;

                    default: break;
                }

                // Default location.
                {
                    stack_type::slice range(yystack_, yylen);
                    YYLLOC_DEFAULT(yylhs.location, range, yylen);
                    yyerror_range[1].location = yylhs.location;
                }

                // Perform the reduction.
                YY_REDUCE_PRINT(yyn);
#if YY_EXCEPTIONS
                try
#endif  // YY_EXCEPTIONS
                {
                    switch (yyn)
                    {
                        case 2:  // PROGRAM: STMT_LIST
#line 141 "parser/yacc.y"
                        {
                            yylhs.value.as<ASTree*>() = new ASTree(yystack_[0].value.as<std::vector<StmtNode*>*>());
                            driver.setAST(yylhs.value.as<ASTree*>());
                        }
#line 1045 "parser/yacc.cpp"
                        break;

                        case 3:  // PROGRAM: PROGRAM END
#line 145 "parser/yacc.y"
                        {
                            YYACCEPT;
                        }
#line 1053 "parser/yacc.cpp"
                        break;

                        case 4:  // STMT_LIST: STMT
#line 151 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<StmtNode*>*>() = new std::vector<StmtNode*>();
                            if (yystack_[0].value.as<StmtNode*>())
                                yylhs.value.as<std::vector<StmtNode*>*>()->push_back(yystack_[0].value.as<StmtNode*>());
                        }
#line 1062 "parser/yacc.cpp"
                        break;

                        case 5:  // STMT_LIST: STMT_LIST STMT
#line 155 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<StmtNode*>*>() = yystack_[1].value.as<std::vector<StmtNode*>*>();
                            if (yystack_[0].value.as<StmtNode*>())
                                yylhs.value.as<std::vector<StmtNode*>*>()->push_back(yystack_[0].value.as<StmtNode*>());
                        }
#line 1071 "parser/yacc.cpp"
                        break;

                        case 6:  // STMT: EXPR_STMT
#line 162 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1079 "parser/yacc.cpp"
                        break;

                        case 7:  // STMT: VAR_DECL_STMT
#line 165 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1087 "parser/yacc.cpp"
                        break;

                        case 8:  // STMT: BLOCK_STMT
#line 168 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1095 "parser/yacc.cpp"
                        break;

                        case 9:  // STMT: FUNC_DECL_STMT
#line 171 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1103 "parser/yacc.cpp"
                        break;

                        case 10:  // STMT: RETURN_STMT
#line 174 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1111 "parser/yacc.cpp"
                        break;

                        case 11:  // STMT: WHILE_STMT
#line 177 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1119 "parser/yacc.cpp"
                        break;

                        case 12:  // STMT: IF_STMT
#line 180 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1127 "parser/yacc.cpp"
                        break;

                        case 13:  // STMT: FOR_STMT
#line 183 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1135 "parser/yacc.cpp"
                        break;

                        case 14:  // STMT: BREAK_STMT
#line 186 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1143 "parser/yacc.cpp"
                        break;

                        case 15:  // STMT: CONTINUE_STMT
#line 189 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1151 "parser/yacc.cpp"
                        break;

                        case 16:  // STMT: SEMICOLON
#line 192 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = nullptr;
                        }
#line 1159 "parser/yacc.cpp"
                        break;

                        case 17:  // CONTINUE_STMT: CONTINUE SEMICOLON
#line 198 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new ContinueStmt();
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[1].location.begin.line);
                        }
#line 1168 "parser/yacc.cpp"
                        break;

                        case 18:  // BREAK_STMT: BREAK SEMICOLON
#line 205 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new BreakStmt();
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[1].location.begin.line);
                        }
#line 1177 "parser/yacc.cpp"
                        break;

                        case 19:  // BLOCK_STMT: LBRACE STMT_LIST RBRACE
#line 212 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() =
                                new BlockStmt(yystack_[1].value.as<std::vector<StmtNode*>*>());
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[2].location.begin.line);
                        }
#line 1186 "parser/yacc.cpp"
                        break;

                        case 20:  // BLOCK_STMT: LBRACE RBRACE
#line 216 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new BlockStmt(nullptr);
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[1].location.begin.line);
                        }
#line 1195 "parser/yacc.cpp"
                        break;

                        case 21:  // EXPR_STMT: EXPR_LIST SEMICOLON
#line 223 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new ExprStmt(yystack_[1].value.as<std::vector<ExprNode*>*>());
                            yylhs.value.as<StmtNode*>()->set_line(
                                (*yystack_[1].value.as<std::vector<ExprNode*>*>())[0]->get_line());
                        }
#line 1204 "parser/yacc.cpp"
                        break;

                        case 22:  // VAR_DECL_STMT: TYPE DEF_LIST SEMICOLON
#line 230 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new VarDeclStmt(
                                yystack_[2].value.as<Type*>(), yystack_[1].value.as<std::vector<DefNode*>*>());
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[2].location.begin.line);
                        }
#line 1213 "parser/yacc.cpp"
                        break;

                        case 23:  // VAR_DECL_STMT: CONST TYPE DEF_LIST SEMICOLON
#line 234 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new VarDeclStmt(
                                yystack_[2].value.as<Type*>(), yystack_[1].value.as<std::vector<DefNode*>*>(), true);
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[3].location.begin.line);
                        }
#line 1222 "parser/yacc.cpp"
                        break;

                        case 24:  // FUNC_DECL_STMT: TYPE IDENT LPAREN FUNC_PARAM_DEF_LIST RPAREN BLOCK_STMT
#line 241 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[4].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<StmtNode*>() = new FuncDeclStmt(entry,
                                yystack_[5].value.as<Type*>(),
                                yystack_[2].value.as<std::vector<FuncParamDefNode*>*>(),
                                yystack_[0].value.as<StmtNode*>());
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[5].location.begin.line);
                        }
#line 1232 "parser/yacc.cpp"
                        break;

                        case 25:  // RETURN_STMT: RETURN SEMICOLON
#line 248 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new ReturnStmt(nullptr);
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[1].location.begin.line);
                        }
#line 1241 "parser/yacc.cpp"
                        break;

                        case 26:  // RETURN_STMT: RETURN EXPR SEMICOLON
#line 252 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new ReturnStmt(yystack_[1].value.as<ExprNode*>());
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[2].location.begin.line);
                        }
#line 1250 "parser/yacc.cpp"
                        break;

                        case 27:  // WHILE_STMT: WHILE LPAREN EXPR RPAREN STMT
#line 259 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() =
                                new WhileStmt(yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<StmtNode*>());
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[4].location.begin.line);
                        }
#line 1259 "parser/yacc.cpp"
                        break;

                        case 28:  // IF_STMT: IF LPAREN EXPR RPAREN STMT
#line 266 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new IfStmt(
                                yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<StmtNode*>(), nullptr);
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[4].location.begin.line);
                        }
#line 1268 "parser/yacc.cpp"
                        break;

                        case 29:  // IF_STMT: IF LPAREN EXPR RPAREN STMT ELSE STMT
#line 270 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new IfStmt(yystack_[4].value.as<ExprNode*>(),
                                yystack_[2].value.as<StmtNode*>(),
                                yystack_[0].value.as<StmtNode*>());
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[6].location.begin.line);
                        }
#line 1277 "parser/yacc.cpp"
                        break;

                        case 30:  // FOR_INIT_STMT: EXPR_STMT
#line 277 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1285 "parser/yacc.cpp"
                        break;

                        case 31:  // FOR_INIT_STMT: VAR_DECL_STMT
#line 280 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = yystack_[0].value.as<StmtNode*>();
                        }
#line 1293 "parser/yacc.cpp"
                        break;

                        case 32:  // FOR_INIT_STMT: SEMICOLON
#line 283 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = nullptr;
                        }
#line 1301 "parser/yacc.cpp"
                        break;

                        case 33:  // FOR_INCR_STMT: %empty
#line 288 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = nullptr;
                        }
#line 1309 "parser/yacc.cpp"
                        break;

                        case 34:  // FOR_INCR_STMT: EXPR_LIST
#line 291 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new ExprStmt(yystack_[0].value.as<std::vector<ExprNode*>*>());
                        }
#line 1317 "parser/yacc.cpp"
                        break;

                        case 35:  // FOR_STMT: FOR LPAREN FOR_INIT_STMT EXPR SEMICOLON FOR_INCR_STMT RPAREN STMT
#line 297 "parser/yacc.y"
                        {
                            yylhs.value.as<StmtNode*>() = new ForStmt(yystack_[5].value.as<StmtNode*>(),
                                yystack_[4].value.as<ExprNode*>(),
                                yystack_[2].value.as<StmtNode*>(),
                                yystack_[0].value.as<StmtNode*>());
                            yylhs.value.as<StmtNode*>()->set_line(yystack_[7].location.begin.line);
                        }
#line 1326 "parser/yacc.cpp"
                        break;

                        case 36:  // FUNC_PARAM_DEF: TYPE IDENT
#line 304 "parser/yacc.y"
                        {  // int a;
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[0].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<FuncParamDefNode*>() =
                                new FuncParamDefNode(yystack_[1].value.as<Type*>(), entry, nullptr);
                        }
#line 1335 "parser/yacc.cpp"
                        break;

                        case 37:  // FUNC_PARAM_DEF: TYPE IDENT LBRACKET RBRACKET
#line 308 "parser/yacc.y"
                        {  // int a[];
                            std::vector<ExprNode*>* dim = new std::vector<ExprNode*>();
                            dim->emplace_back(new ConstExpr(-1));
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[2].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<FuncParamDefNode*>() =
                                new FuncParamDefNode(yystack_[3].value.as<Type*>(), entry, dim);
                        }
#line 1346 "parser/yacc.cpp"
                        break;

                        case 38:  // FUNC_PARAM_DEF: TYPE IDENT LBRACKET RBRACKET ARRAY_DIMESION_EXPR_LIST
#line 314 "parser/yacc.y"
                        {  // int a[][10];
                            std::vector<ExprNode*>* dim = yystack_[0].value.as<std::vector<ExprNode*>*>();
                            dim->insert(dim->begin(), new ConstExpr(-1));
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[3].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<FuncParamDefNode*>() =
                                new FuncParamDefNode(yystack_[4].value.as<Type*>(), entry, dim);
                        }
#line 1357 "parser/yacc.cpp"
                        break;

                        case 39:  // FUNC_PARAM_DEF: TYPE IDENT ARRAY_DIMESION_EXPR_LIST
#line 320 "parser/yacc.y"
                        {  // int a[10][10]
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[1].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<FuncParamDefNode*>() = new FuncParamDefNode(
                                yystack_[2].value.as<Type*>(), entry, yystack_[0].value.as<std::vector<ExprNode*>*>());
                        }
#line 1366 "parser/yacc.cpp"
                        break;

                        case 40:  // FUNC_PARAM_DEF_LIST: %empty
#line 327 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<FuncParamDefNode*>*>() = new std::vector<FuncParamDefNode*>();
                        }
#line 1374 "parser/yacc.cpp"
                        break;

                        case 41:  // FUNC_PARAM_DEF_LIST: FUNC_PARAM_DEF
#line 330 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<FuncParamDefNode*>*>() = new std::vector<FuncParamDefNode*>();
                            yylhs.value.as<std::vector<FuncParamDefNode*>*>()->push_back(
                                yystack_[0].value.as<FuncParamDefNode*>());
                        }
#line 1383 "parser/yacc.cpp"
                        break;

                        case 42:  // FUNC_PARAM_DEF_LIST: FUNC_PARAM_DEF_LIST COMMA FUNC_PARAM_DEF
#line 334 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<FuncParamDefNode*>*>() =
                                yystack_[2].value.as<std::vector<FuncParamDefNode*>*>();
                            yylhs.value.as<std::vector<FuncParamDefNode*>*>()->push_back(
                                yystack_[0].value.as<FuncParamDefNode*>());
                        }
#line 1392 "parser/yacc.cpp"
                        break;

                        case 43:  // DEF: LEFT_VAL_EXPR
#line 340 "parser/yacc.y"
                        {
                            yylhs.value.as<DefNode*>() = new DefNode(yystack_[0].value.as<ExprNode*>(), nullptr);
                        }
#line 1400 "parser/yacc.cpp"
                        break;

                        case 44:  // DEF: LEFT_VAL_EXPR ASSIGN INITIALIZER
#line 343 "parser/yacc.y"
                        {
                            yylhs.value.as<DefNode*>() =
                                new DefNode(yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<InitNode*>());
                        }
#line 1408 "parser/yacc.cpp"
                        break;

                        case 45:  // DEF: IDENT LBRACKET RBRACKET ASSIGN INITIALIZER
#line 346 "parser/yacc.y"
                        {
                            std::vector<ExprNode*>* dim = new std::vector<ExprNode*>();
                            dim->emplace_back(
                                new ConstExpr(static_cast<InitMulti*>(yystack_[0].value.as<InitNode*>())->getSize()));
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[4].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<DefNode*>() =
                                new DefNode(new LeftValueExpr(entry, dim, -1), yystack_[0].value.as<InitNode*>());
                        }
#line 1419 "parser/yacc.cpp"
                        break;

                        case 46:  // DEF: IDENT LBRACKET RBRACKET ARRAY_DIMESION_EXPR_LIST ASSIGN INITIALIZER
#line 352 "parser/yacc.y"
                        {
                            std::vector<ExprNode*>* dim = yystack_[2].value.as<std::vector<ExprNode*>*>();
                            dim->insert(dim->begin(),
                                new ConstExpr(static_cast<InitMulti*>(yystack_[0].value.as<InitNode*>())->getSize()));
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[5].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<DefNode*>() =
                                new DefNode(new LeftValueExpr(entry, dim, -1), yystack_[0].value.as<InitNode*>());
                        }
#line 1430 "parser/yacc.cpp"
                        break;

                        case 47:  // DEF_LIST: DEF
#line 361 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<DefNode*>*>() = new std::vector<DefNode*>();
                            yylhs.value.as<std::vector<DefNode*>*>()->push_back(yystack_[0].value.as<DefNode*>());
                        }
#line 1439 "parser/yacc.cpp"
                        break;

                        case 48:  // DEF_LIST: DEF_LIST COMMA DEF
#line 365 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<DefNode*>*>() = yystack_[2].value.as<std::vector<DefNode*>*>();
                            yylhs.value.as<std::vector<DefNode*>*>()->push_back(yystack_[0].value.as<DefNode*>());
                        }
#line 1448 "parser/yacc.cpp"
                        break;

                        case 49:  // INITIALIZER: EXPR
#line 372 "parser/yacc.y"
                        {
                            yylhs.value.as<InitNode*>() = new InitSingle(yystack_[0].value.as<ExprNode*>());
                        }
#line 1456 "parser/yacc.cpp"
                        break;

                        case 50:  // INITIALIZER: LBRACE INITIALIZER_LIST RBRACE
#line 375 "parser/yacc.y"
                        {
                            yylhs.value.as<InitNode*>() =
                                new InitMulti(yystack_[1].value.as<std::vector<InitNode*>*>());
                        }
#line 1464 "parser/yacc.cpp"
                        break;

                        case 51:  // INITIALIZER: LBRACE RBRACE
#line 378 "parser/yacc.y"
                        {
                            yylhs.value.as<InitNode*>() = new InitMulti(nullptr);
                        }
#line 1472 "parser/yacc.cpp"
                        break;

                        case 52:  // INITIALIZER_LIST: INITIALIZER
#line 384 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<InitNode*>*>() = new std::vector<InitNode*>();
                            yylhs.value.as<std::vector<InitNode*>*>()->push_back(yystack_[0].value.as<InitNode*>());
                        }
#line 1481 "parser/yacc.cpp"
                        break;

                        case 53:  // INITIALIZER_LIST: INITIALIZER_LIST COMMA INITIALIZER
#line 388 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<InitNode*>*>() = yystack_[2].value.as<std::vector<InitNode*>*>();
                            yylhs.value.as<std::vector<InitNode*>*>()->push_back(yystack_[0].value.as<InitNode*>());
                        }
#line 1490 "parser/yacc.cpp"
                        break;

                        case 54:  // ASSIGN_EXPR: LEFT_VAL_EXPR ASSIGN EXPR
#line 395 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Assign, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1498 "parser/yacc.cpp"
                        break;

                        case 55:  // EXPR_LIST: EXPR
#line 401 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<ExprNode*>*>() = new std::vector<ExprNode*>();
                            yylhs.value.as<std::vector<ExprNode*>*>()->push_back(yystack_[0].value.as<ExprNode*>());
                        }
#line 1507 "parser/yacc.cpp"
                        break;

                        case 56:  // EXPR_LIST: EXPR_LIST COMMA EXPR
#line 405 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<ExprNode*>*>() = yystack_[2].value.as<std::vector<ExprNode*>*>();
                            yylhs.value.as<std::vector<ExprNode*>*>()->push_back(yystack_[0].value.as<ExprNode*>());
                        }
#line 1516 "parser/yacc.cpp"
                        break;

                        case 57:  // EXPR: LOGICAL_OR_EXPR
#line 412 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1525 "parser/yacc.cpp"
                        break;

                        case 58:  // EXPR: ASSIGN_EXPR
#line 416 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1534 "parser/yacc.cpp"
                        break;

                        case 59:  // LOGICAL_OR_EXPR: LOGICAL_AND_EXPR
#line 423 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1542 "parser/yacc.cpp"
                        break;

                        case 60:  // LOGICAL_OR_EXPR: LOGICAL_OR_EXPR OR LOGICAL_AND_EXPR
#line 426 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Or, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1550 "parser/yacc.cpp"
                        break;

                        case 61:  // LOGICAL_AND_EXPR: EQUALITY_EXPR
#line 432 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1558 "parser/yacc.cpp"
                        break;

                        case 62:  // LOGICAL_AND_EXPR: LOGICAL_AND_EXPR AND EQUALITY_EXPR
#line 435 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::And, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1566 "parser/yacc.cpp"
                        break;

                        case 63:  // EQUALITY_EXPR: RELATIONAL_EXPR
#line 441 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1574 "parser/yacc.cpp"
                        break;

                        case 64:  // EQUALITY_EXPR: EQUALITY_EXPR EQ RELATIONAL_EXPR
#line 444 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Eq, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1582 "parser/yacc.cpp"
                        break;

                        case 65:  // EQUALITY_EXPR: EQUALITY_EXPR NEQ RELATIONAL_EXPR
#line 447 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Neq, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1590 "parser/yacc.cpp"
                        break;

                        case 66:  // RELATIONAL_EXPR: ADDSUB_EXPR
#line 453 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1598 "parser/yacc.cpp"
                        break;

                        case 67:  // RELATIONAL_EXPR: RELATIONAL_EXPR GT ADDSUB_EXPR
#line 456 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Gt, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1606 "parser/yacc.cpp"
                        break;

                        case 68:  // RELATIONAL_EXPR: RELATIONAL_EXPR GE ADDSUB_EXPR
#line 459 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Ge, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1614 "parser/yacc.cpp"
                        break;

                        case 69:  // RELATIONAL_EXPR: RELATIONAL_EXPR LT ADDSUB_EXPR
#line 462 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Lt, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1622 "parser/yacc.cpp"
                        break;

                        case 70:  // RELATIONAL_EXPR: RELATIONAL_EXPR LE ADDSUB_EXPR
#line 465 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Le, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1630 "parser/yacc.cpp"
                        break;

                        case 71:  // ADDSUB_EXPR: MULDIV_EXPR
#line 471 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1638 "parser/yacc.cpp"
                        break;

                        case 72:  // ADDSUB_EXPR: ADDSUB_EXPR PLUS MULDIV_EXPR
#line 474 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Add, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1646 "parser/yacc.cpp"
                        break;

                        case 73:  // ADDSUB_EXPR: ADDSUB_EXPR MINUS MULDIV_EXPR
#line 477 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Sub, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1654 "parser/yacc.cpp"
                        break;

                        case 74:  // MULDIV_EXPR: UNARY_EXPR
#line 483 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1662 "parser/yacc.cpp"
                        break;

                        case 75:  // MULDIV_EXPR: MULDIV_EXPR STAR UNARY_EXPR
#line 486 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Mul, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1670 "parser/yacc.cpp"
                        break;

                        case 76:  // MULDIV_EXPR: MULDIV_EXPR SLASH UNARY_EXPR
#line 489 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Div, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1678 "parser/yacc.cpp"
                        break;

                        case 77:  // MULDIV_EXPR: MULDIV_EXPR MOD UNARY_EXPR
#line 492 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Mod, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1686 "parser/yacc.cpp"
                        break;

                        case 78:  // UNARY_EXPR: BASIC_EXPR
#line 498 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1694 "parser/yacc.cpp"
                        break;

                        case 79:  // UNARY_EXPR: UNARY_OP UNARY_EXPR
#line 501 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() =
                                new UnaryExpr(yystack_[1].value.as<OpCode>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1702 "parser/yacc.cpp"
                        break;

                        case 80:  // BASIC_EXPR: CONST_EXPR
#line 507 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1710 "parser/yacc.cpp"
                        break;

                        case 81:  // BASIC_EXPR: LEFT_VAL_EXPR
#line 510 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1718 "parser/yacc.cpp"
                        break;

                        case 82:  // BASIC_EXPR: LPAREN EXPR RPAREN
#line 513 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[1].value.as<ExprNode*>();
                        }
#line 1726 "parser/yacc.cpp"
                        break;

                        case 83:  // BASIC_EXPR: FUNC_CALL_EXPR
#line 516 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1734 "parser/yacc.cpp"
                        break;

                        case 84:  // FUNC_CALL_EXPR: IDENT LPAREN RPAREN
#line 522 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[2].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>() = new FuncCallExpr(entry, nullptr);
                        }
#line 1743 "parser/yacc.cpp"
                        break;

                        case 85:  // FUNC_CALL_EXPR: IDENT LPAREN EXPR_LIST RPAREN
#line 526 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[3].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>() =
                                new FuncCallExpr(entry, yystack_[1].value.as<std::vector<ExprNode*>*>());
                        }
#line 1752 "parser/yacc.cpp"
                        break;

                        case 86:  // ARRAY_DIMESION_EXPR: LBRACKET EXPR RBRACKET
#line 533 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[1].value.as<ExprNode*>();
                        }
#line 1760 "parser/yacc.cpp"
                        break;

                        case 87:  // ARRAY_DIMESION_EXPR_LIST: ARRAY_DIMESION_EXPR
#line 539 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<ExprNode*>*>() = new std::vector<ExprNode*>();
                            yylhs.value.as<std::vector<ExprNode*>*>()->push_back(yystack_[0].value.as<ExprNode*>());
                        }
#line 1769 "parser/yacc.cpp"
                        break;

                        case 88:  // ARRAY_DIMESION_EXPR_LIST: ARRAY_DIMESION_EXPR_LIST ARRAY_DIMESION_EXPR
#line 543 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<ExprNode*>*>() = yystack_[1].value.as<std::vector<ExprNode*>*>();
                            yylhs.value.as<std::vector<ExprNode*>*>()->push_back(yystack_[0].value.as<ExprNode*>());
                        }
#line 1778 "parser/yacc.cpp"
                        break;

                        case 89:  // LEFT_VAL_EXPR: IDENT
#line 550 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[0].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>() = new LeftValueExpr(entry, nullptr, -1);
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1788 "parser/yacc.cpp"
                        break;

                        case 90:  // LEFT_VAL_EXPR: IDENT ARRAY_DIMESION_EXPR_LIST
#line 555 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[1].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>() =
                                new LeftValueExpr(entry, yystack_[0].value.as<std::vector<ExprNode*>*>(), -1);
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[1].location.begin.line);
                        }
#line 1798 "parser/yacc.cpp"
                        break;

                        case 91:  // CONST_EXPR: INT_CONST
#line 563 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new ConstExpr(yystack_[0].value.as<int>());
                            yylhs.value.as<ExprNode*>()->setConst();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1808 "parser/yacc.cpp"
                        break;

                        case 92:  // CONST_EXPR: LL_CONST
#line 568 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new ConstExpr(yystack_[0].value.as<long long>());
                            yylhs.value.as<ExprNode*>()->setConst();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1818 "parser/yacc.cpp"
                        break;

                        case 93:  // CONST_EXPR: FLOAT_CONST
#line 573 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new ConstExpr(yystack_[0].value.as<float>());
                            yylhs.value.as<ExprNode*>()->setConst();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1828 "parser/yacc.cpp"
                        break;

                        case 94:  // CONST_EXPR: STR_CONST
#line 578 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() =
                                new ConstExpr(yystack_[0].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>()->setConst();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1838 "parser/yacc.cpp"
                        break;

                        case 95:  // TYPE: INT
#line 586 "parser/yacc.y"
                        {
                            yylhs.value.as<Type*>() = intType;
                        }
#line 1846 "parser/yacc.cpp"
                        break;

                        case 96:  // TYPE: FLOAT
#line 589 "parser/yacc.y"
                        {
                            yylhs.value.as<Type*>() = floatType;
                        }
#line 1854 "parser/yacc.cpp"
                        break;

                        case 97:  // TYPE: VOID
#line 592 "parser/yacc.y"
                        {
                            yylhs.value.as<Type*>() = voidType;
                        }
#line 1862 "parser/yacc.cpp"
                        break;

                        case 98:  // UNARY_OP: PLUS
#line 598 "parser/yacc.y"
                        {
                            yylhs.value.as<OpCode>() = OpCode::Add;
                        }
#line 1870 "parser/yacc.cpp"
                        break;

                        case 99:  // UNARY_OP: MINUS
#line 601 "parser/yacc.y"
                        {
                            yylhs.value.as<OpCode>() = OpCode::Sub;
                        }
#line 1878 "parser/yacc.cpp"
                        break;

                        case 100:  // UNARY_OP: NOT
#line 604 "parser/yacc.y"
                        {
                            yylhs.value.as<OpCode>() = OpCode::Not;
                        }
#line 1886 "parser/yacc.cpp"
                        break;

#line 1890 "parser/yacc.cpp"

                        default: break;
                    }
                }
#if YY_EXCEPTIONS
                catch (const syntax_error& yyexc)
                {
                    YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
                    error(yyexc);
                    YYERROR;
                }
#endif  // YY_EXCEPTIONS
                YY_SYMBOL_PRINT("-> $$ =", yylhs);
                yypop_(yylen);
                yylen = 0;

                // Shift the result of the reduction.
                yypush_(YY_NULLPTR, YY_MOVE(yylhs));
            }
            goto yynewstate;

        /*--------------------------------------.
        | yyerrlab -- here on detecting error.  |
        `--------------------------------------*/
        yyerrlab:
            // If not already recovering from an error, report this error.
            if (!yyerrstatus_)
            {
                ++yynerrs_;
                context     yyctx(*this, yyla);
                std::string msg = yysyntax_error_(yyctx);
                error(yyla.location, YY_MOVE(msg));
            }

            yyerror_range[1].location = yyla.location;
            if (yyerrstatus_ == 3)
            {
                /* If just tried and failed to reuse lookahead token after an
                   error, discard it.  */

                // Return failure if at end of input.
                if (yyla.kind() == symbol_kind::S_YYEOF)
                    YYABORT;
                else if (!yyla.empty())
                {
                    yy_destroy_("Error: discarding", yyla);
                    yyla.clear();
                }
            }

            // Else will try to reuse lookahead token after shifting the error token.
            goto yyerrlab1;

        /*---------------------------------------------------.
        | yyerrorlab -- error raised explicitly by YYERROR.  |
        `---------------------------------------------------*/
        yyerrorlab:
            /* Pacify compilers when the user code never invokes YYERROR and
               the label yyerrorlab therefore never appears in user code.  */
            if (false) YYERROR;

            /* Do not reclaim the symbols of the rule whose action triggered
               this YYERROR.  */
            yypop_(yylen);
            yylen = 0;
            YY_STACK_PRINT();
            goto yyerrlab1;

        /*-------------------------------------------------------------.
        | yyerrlab1 -- common code for both syntax error and YYERROR.  |
        `-------------------------------------------------------------*/
        yyerrlab1:
            yyerrstatus_ = 3;  // Each real token shifted decrements this.
            // Pop stack until we find a state that shifts the error token.
            for (;;)
            {
                yyn = yypact_[+yystack_[0].state];
                if (!yy_pact_value_is_default_(yyn))
                {
                    yyn += symbol_kind::S_YYerror;
                    if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == symbol_kind::S_YYerror)
                    {
                        yyn = yytable_[yyn];
                        if (0 < yyn) break;
                    }
                }

                // Pop the current state because it cannot handle the error token.
                if (yystack_.size() == 1) YYABORT;

                yyerror_range[1].location = yystack_[0].location;
                yy_destroy_("Error: popping", yystack_[0]);
                yypop_();
                YY_STACK_PRINT();
            }
            {
                stack_symbol_type error_token;

                yyerror_range[2].location = yyla.location;
                YYLLOC_DEFAULT(error_token.location, yyerror_range, 2);

                // Shift the error token.
                error_token.state = state_type(yyn);
                yypush_("Shifting", YY_MOVE(error_token));
            }
            goto yynewstate;

        /*-------------------------------------.
        | yyacceptlab -- YYACCEPT comes here.  |
        `-------------------------------------*/
        yyacceptlab:
            yyresult = 0;
            goto yyreturn;

        /*-----------------------------------.
        | yyabortlab -- YYABORT comes here.  |
        `-----------------------------------*/
        yyabortlab:
            yyresult = 1;
            goto yyreturn;

        /*-----------------------------------------------------.
        | yyreturn -- parsing is finished, return the result.  |
        `-----------------------------------------------------*/
        yyreturn:
            if (!yyla.empty()) yy_destroy_("Cleanup: discarding lookahead", yyla);

            /* Do not reclaim the symbols of the rule whose action triggered
               this YYABORT or YYACCEPT.  */
            yypop_(yylen);
            YY_STACK_PRINT();
            while (1 < yystack_.size())
            {
                yy_destroy_("Cleanup: popping", yystack_[0]);
                yypop_();
            }

            return yyresult;
        }
#if YY_EXCEPTIONS
        catch (...)
        {
            YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
            // Do not try to display the values of the reclaimed symbols,
            // as their printers might throw an exception.
            if (!yyla.empty()) yy_destroy_(YY_NULLPTR, yyla);

            while (1 < yystack_.size())
            {
                yy_destroy_(YY_NULLPTR, yystack_[0]);
                yypop_();
            }
            throw;
        }
#endif  // YY_EXCEPTIONS
    }

    void Parser ::error(const syntax_error& yyexc) { error(yyexc.location, yyexc.what()); }

    /* Return YYSTR after stripping away unnecessary quotes and
       backslashes, so that it's suitable for yyerror.  The heuristic is
       that double-quoting is unnecessary unless the string contains an
       apostrophe, a comma, or backslash (other than backslash-backslash).
       YYSTR is taken from yytname.  */
    std::string Parser ::yytnamerr_(const char* yystr)
    {
        if (*yystr == '"')
        {
            std::string yyr;
            char const* yyp = yystr;

            for (;;) switch (*++yyp)
                {
                    case '\'':
                    case ',': goto do_not_strip_quotes;

                    case '\\':
                        if (*++yyp != '\\')
                            goto do_not_strip_quotes;
                        else
                            goto append;

                    append:
                    default: yyr += *yyp; break;

                    case '"': return yyr;
                }
        do_not_strip_quotes:;
        }

        return yystr;
    }

    std::string Parser ::symbol_name(symbol_kind_type yysymbol) { return yytnamerr_(yytname_[yysymbol]); }

    //  Parser ::context.
    Parser ::context::context(const Parser& yyparser, const symbol_type& yyla) : yyparser_(yyparser), yyla_(yyla) {}

    int Parser ::context::expected_tokens(symbol_kind_type yyarg[], int yyargn) const
    {
        // Actual number of expected tokens
        int yycount = 0;

        const int yyn = yypact_[+yyparser_.yystack_[0].state];
        if (!yy_pact_value_is_default_(yyn))
        {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            const int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            const int yychecklim = yylast_ - yyn + 1;
            const int yyxend     = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
                if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror &&
                    !yy_table_value_is_error_(yytable_[yyx + yyn]))
                {
                    if (!yyarg)
                        ++yycount;
                    else if (yycount == yyargn)
                        return 0;
                    else
                        yyarg[yycount++] = YY_CAST(symbol_kind_type, yyx);
                }
        }

        if (yyarg && yycount == 0 && 0 < yyargn) yyarg[0] = symbol_kind::S_YYEMPTY;
        return yycount;
    }

    int Parser ::yy_syntax_error_arguments_(const context& yyctx, symbol_kind_type yyarg[], int yyargn) const
    {
        /* There are many possibilities here to consider:
           - If this state is a consistent state with a default action, then
             the only way this function was invoked is if the default action
             is an error action.  In that case, don't check for expected
             tokens because there are none.
           - The only way there can be no lookahead present (in yyla) is
             if this state is a consistent state with a default action.
             Thus, detecting the absence of a lookahead is sufficient to
             determine that there is no unexpected or expected token to
             report.  In that case, just report a simple "syntax error".
           - Don't assume there isn't a lookahead just because this state is
             a consistent state with a default action.  There might have
             been a previous inconsistent state, consistent state with a
             non-default action, or user semantic action that manipulated
             yyla.  (However, yyla is currently not documented for users.)
           - Of course, the expected token list depends on states to have
             correct lookahead information, and it depends on the parser not
             to perform extra reductions after fetching a lookahead from the
             scanner and before detecting a syntax error.  Thus, state merging
             (from LALR or IELR) and default reductions corrupt the expected
             token list.  However, the list is correct for canonical LR with
             one exception: it will still contain any token that will not be
             accepted due to an error action in a later state.
        */

        if (!yyctx.lookahead().empty())
        {
            if (yyarg) yyarg[0] = yyctx.token();
            int yyn = yyctx.expected_tokens(yyarg ? yyarg + 1 : yyarg, yyargn - 1);
            return yyn + 1;
        }
        return 0;
    }

    // Generate an error message.
    std::string Parser ::yysyntax_error_(const context& yyctx) const
    {
        // Its maximum.
        enum
        {
            YYARGS_MAX = 5
        };
        // Arguments of yyformat.
        symbol_kind_type yyarg[YYARGS_MAX];
        int              yycount = yy_syntax_error_arguments_(yyctx, yyarg, YYARGS_MAX);

        char const* yyformat = YY_NULLPTR;
        switch (yycount)
        {
#define YYCASE_(N, S) \
    case N: yyformat = S; break
            default:  // Avoid compiler warnings.
                YYCASE_(0, YY_("syntax error"));
                YYCASE_(1, YY_("syntax error, unexpected %s"));
                YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
                YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
                YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
                YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
        }

        std::string yyres;
        // Argument number.
        std::ptrdiff_t yyi = 0;
        for (char const* yyp = yyformat; *yyp; ++yyp)
            if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
            {
                yyres += symbol_name(yyarg[yyi++]);
                ++yyp;
            }
            else
                yyres += *yyp;
        return yyres;
    }

    const short Parser ::yypact_ninf_ = -133;

    const signed char Parser ::yytable_ninf_ = -1;

    const short Parser ::yypact_[] = {196,
        -133,
        -133,
        -133,
        -133,
        55,
        -133,
        -133,
        -133,
        -19,
        0,
        3,
        15,
        19,
        96,
        7,
        -133,
        322,
        42,
        -133,
        -133,
        -133,
        5,
        196,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        8,
        -133,
        37,
        16,
        13,
        -21,
        24,
        -29,
        -133,
        -133,
        -133,
        34,
        -133,
        98,
        322,
        244,
        322,
        -133,
        79,
        322,
        227,
        322,
        -133,
        -133,
        -133,
        85,
        102,
        86,
        -133,
        159,
        -133,
        -133,
        -133,
        -133,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        322,
        59,
        -133,
        46,
        70,
        -133,
        -133,
        -133,
        61,
        88,
        -133,
        92,
        -133,
        -133,
        -133,
        322,
        102,
        94,
        -133,
        97,
        69,
        -133,
        -133,
        -133,
        16,
        13,
        -21,
        -21,
        24,
        24,
        24,
        24,
        -29,
        -29,
        -133,
        -133,
        -133,
        -133,
        7,
        275,
        -133,
        102,
        284,
        -133,
        -133,
        196,
        100,
        196,
        -133,
        -133,
        65,
        121,
        -18,
        -133,
        237,
        -133,
        -133,
        117,
        322,
        -133,
        7,
        101,
        108,
        284,
        -17,
        -133,
        -133,
        -10,
        196,
        111,
        115,
        -133,
        -133,
        291,
        79,
        -133,
        284,
        284,
        -133,
        -133,
        196,
        79,
        -133,
        -133,
        -133,
        79};

    const signed char Parser ::yydefact_[] = {0,
        91,
        92,
        93,
        94,
        89,
        95,
        96,
        97,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        16,
        0,
        0,
        100,
        98,
        99,
        0,
        2,
        4,
        15,
        14,
        8,
        6,
        7,
        9,
        10,
        11,
        12,
        13,
        58,
        0,
        55,
        57,
        59,
        61,
        63,
        66,
        71,
        74,
        78,
        83,
        81,
        80,
        0,
        0,
        0,
        0,
        87,
        90,
        0,
        0,
        0,
        17,
        18,
        25,
        0,
        0,
        0,
        20,
        0,
        1,
        3,
        5,
        21,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        89,
        47,
        0,
        43,
        79,
        81,
        84,
        0,
        0,
        88,
        0,
        32,
        30,
        31,
        0,
        0,
        0,
        26,
        89,
        0,
        82,
        19,
        56,
        60,
        62,
        64,
        65,
        67,
        68,
        69,
        70,
        72,
        73,
        75,
        76,
        77,
        54,
        40,
        0,
        22,
        0,
        0,
        85,
        86,
        0,
        0,
        0,
        23,
        41,
        0,
        0,
        0,
        48,
        0,
        44,
        49,
        28,
        33,
        27,
        0,
        0,
        36,
        0,
        0,
        51,
        52,
        0,
        0,
        0,
        34,
        42,
        24,
        0,
        39,
        45,
        0,
        0,
        50,
        29,
        0,
        37,
        46,
        53,
        35,
        38};

    const short Parser ::yypgoto_[] = {-133,
        -133,
        124,
        -16,
        -133,
        -133,
        -1,
        89,
        110,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        -133,
        23,
        -133,
        48,
        118,
        -132,
        -133,
        -133,
        -50,
        -14,
        -133,
        114,
        107,
        30,
        1,
        18,
        -46,
        -133,
        -133,
        -52,
        -126,
        78,
        -133,
        -6,
        -133};

    const unsigned char Parser ::yydefgoto_[] = {0,
        22,
        23,
        24,
        25,
        26,
        27,
        28,
        29,
        30,
        31,
        32,
        33,
        99,
        153,
        34,
        133,
        134,
        86,
        87,
        139,
        151,
        35,
        36,
        37,
        38,
        39,
        40,
        41,
        42,
        43,
        44,
        45,
        46,
        53,
        54,
        47,
        48,
        49,
        50};

    const unsigned char Parser ::yytable_[] = {61,
        92,
        94,
        63,
        89,
        66,
        150,
        68,
        55,
        62,
        148,
        52,
        52,
        81,
        82,
        159,
        161,
        6,
        7,
        8,
        158,
        83,
        162,
        75,
        76,
        77,
        78,
        56,
        166,
        167,
        57,
        147,
        160,
        69,
        70,
        118,
        119,
        120,
        93,
        169,
        58,
        95,
        67,
        101,
        59,
        1,
        2,
        3,
        4,
        68,
        100,
        5,
        6,
        7,
        8,
        9,
        107,
        10,
        11,
        12,
        13,
        73,
        79,
        80,
        74,
        14,
        15,
        16,
        72,
        17,
        121,
        124,
        125,
        18,
        64,
        19,
        112,
        113,
        114,
        115,
        20,
        21,
        51,
        84,
        52,
        130,
        122,
        70,
        123,
        127,
        71,
        144,
        154,
        145,
        132,
        125,
        94,
        116,
        117,
        1,
        2,
        3,
        4,
        110,
        111,
        5,
        94,
        85,
        52,
        93,
        102,
        103,
        140,
        141,
        105,
        143,
        135,
        94,
        128,
        126,
        129,
        60,
        131,
        17,
        140,
        142,
        123,
        88,
        90,
        19,
        146,
        152,
        18,
        140,
        20,
        21,
        163,
        157,
        135,
        164,
        88,
        70,
        65,
        93,
        156,
        97,
        140,
        140,
        168,
        90,
        90,
        90,
        90,
        90,
        90,
        90,
        90,
        90,
        90,
        90,
        90,
        90,
        1,
        2,
        3,
        4,
        98,
        155,
        5,
        6,
        7,
        8,
        9,
        137,
        10,
        11,
        12,
        13,
        88,
        109,
        104,
        0,
        14,
        15,
        16,
        108,
        17,
        0,
        0,
        0,
        18,
        106,
        19,
        0,
        0,
        0,
        0,
        20,
        21,
        1,
        2,
        3,
        4,
        88,
        0,
        5,
        6,
        7,
        8,
        9,
        0,
        10,
        11,
        12,
        13,
        0,
        0,
        0,
        0,
        14,
        15,
        16,
        0,
        17,
        0,
        0,
        0,
        18,
        0,
        19,
        1,
        2,
        3,
        4,
        20,
        21,
        5,
        6,
        7,
        8,
        1,
        2,
        3,
        4,
        0,
        0,
        5,
        1,
        2,
        3,
        4,
        15,
        96,
        5,
        17,
        0,
        0,
        0,
        0,
        0,
        19,
        0,
        0,
        0,
        17,
        20,
        21,
        0,
        138,
        149,
        19,
        17,
        91,
        0,
        0,
        20,
        21,
        19,
        1,
        2,
        3,
        4,
        20,
        21,
        5,
        0,
        0,
        1,
        2,
        3,
        4,
        0,
        0,
        5,
        1,
        2,
        3,
        4,
        0,
        0,
        5,
        0,
        17,
        0,
        0,
        136,
        0,
        0,
        19,
        0,
        0,
        17,
        0,
        20,
        21,
        138,
        0,
        19,
        17,
        0,
        0,
        165,
        20,
        21,
        19,
        1,
        2,
        3,
        4,
        20,
        21,
        5,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        17,
        0,
        0,
        0,
        0,
        0,
        19,
        0,
        0,
        0,
        0,
        20,
        21};

    const short Parser ::yycheck_[] = {14,
        51,
        54,
        17,
        50,
        0,
        138,
        23,
        27,
        15,
        136,
        29,
        29,
        42,
        43,
        147,
        26,
        10,
        11,
        12,
        146,
        50,
        32,
        44,
        45,
        46,
        47,
        27,
        160,
        161,
        27,
        49,
        49,
        25,
        26,
        81,
        82,
        83,
        52,
        165,
        25,
        55,
        37,
        57,
        25,
        3,
        4,
        5,
        6,
        65,
        56,
        9,
        10,
        11,
        12,
        13,
        70,
        15,
        16,
        17,
        18,
        48,
        38,
        39,
        51,
        23,
        24,
        25,
        52,
        27,
        84,
        25,
        26,
        31,
        32,
        33,
        75,
        76,
        77,
        78,
        38,
        39,
        27,
        49,
        29,
        99,
        27,
        26,
        29,
        28,
        53,
        26,
        142,
        28,
        25,
        26,
        148,
        79,
        80,
        3,
        4,
        5,
        6,
        73,
        74,
        9,
        158,
        9,
        29,
        123,
        25,
        9,
        126,
        129,
        28,
        131,
        122,
        169,
        30,
        49,
        28,
        25,
        28,
        27,
        138,
        25,
        29,
        49,
        50,
        33,
        9,
        14,
        31,
        147,
        38,
        39,
        152,
        29,
        144,
        28,
        62,
        26,
        18,
        157,
        145,
        56,
        160,
        161,
        164,
        71,
        72,
        73,
        74,
        75,
        76,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        3,
        4,
        5,
        6,
        56,
        144,
        9,
        10,
        11,
        12,
        13,
        125,
        15,
        16,
        17,
        18,
        100,
        72,
        62,
        -1,
        23,
        24,
        25,
        71,
        27,
        -1,
        -1,
        -1,
        31,
        32,
        33,
        -1,
        -1,
        -1,
        -1,
        38,
        39,
        3,
        4,
        5,
        6,
        125,
        -1,
        9,
        10,
        11,
        12,
        13,
        -1,
        15,
        16,
        17,
        18,
        -1,
        -1,
        -1,
        -1,
        23,
        24,
        25,
        -1,
        27,
        -1,
        -1,
        -1,
        31,
        -1,
        33,
        3,
        4,
        5,
        6,
        38,
        39,
        9,
        10,
        11,
        12,
        3,
        4,
        5,
        6,
        -1,
        -1,
        9,
        3,
        4,
        5,
        6,
        24,
        25,
        9,
        27,
        -1,
        -1,
        -1,
        -1,
        -1,
        33,
        -1,
        -1,
        -1,
        27,
        38,
        39,
        -1,
        31,
        32,
        33,
        27,
        28,
        -1,
        -1,
        38,
        39,
        33,
        3,
        4,
        5,
        6,
        38,
        39,
        9,
        -1,
        -1,
        3,
        4,
        5,
        6,
        -1,
        -1,
        9,
        3,
        4,
        5,
        6,
        -1,
        -1,
        9,
        -1,
        27,
        -1,
        -1,
        30,
        -1,
        -1,
        33,
        -1,
        -1,
        27,
        -1,
        38,
        39,
        31,
        -1,
        33,
        27,
        -1,
        -1,
        30,
        38,
        39,
        33,
        3,
        4,
        5,
        6,
        38,
        39,
        9,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        27,
        -1,
        -1,
        -1,
        -1,
        -1,
        33,
        -1,
        -1,
        -1,
        -1,
        38,
        39};

    const signed char Parser ::yystos_[] = {0,
        3,
        4,
        5,
        6,
        9,
        10,
        11,
        12,
        13,
        15,
        16,
        17,
        18,
        23,
        24,
        25,
        27,
        31,
        33,
        38,
        39,
        56,
        57,
        58,
        59,
        60,
        61,
        62,
        63,
        64,
        65,
        66,
        67,
        70,
        77,
        78,
        79,
        80,
        81,
        82,
        83,
        84,
        85,
        86,
        87,
        88,
        91,
        92,
        93,
        94,
        27,
        29,
        89,
        90,
        27,
        27,
        27,
        25,
        25,
        25,
        79,
        93,
        79,
        32,
        57,
        0,
        37,
        58,
        25,
        26,
        53,
        52,
        48,
        51,
        44,
        45,
        46,
        47,
        38,
        39,
        42,
        43,
        50,
        49,
        9,
        73,
        74,
        91,
        86,
        91,
        28,
        78,
        79,
        89,
        79,
        25,
        62,
        63,
        68,
        93,
        79,
        25,
        9,
        74,
        28,
        32,
        79,
        81,
        82,
        83,
        83,
        84,
        84,
        84,
        84,
        85,
        85,
        86,
        86,
        86,
        79,
        27,
        29,
        25,
        26,
        49,
        28,
        30,
        28,
        79,
        28,
        25,
        71,
        72,
        93,
        30,
        73,
        31,
        75,
        79,
        58,
        25,
        58,
        26,
        28,
        9,
        49,
        90,
        32,
        75,
        76,
        14,
        69,
        78,
        71,
        61,
        29,
        90,
        75,
        49,
        26,
        32,
        58,
        28,
        30,
        75,
        75,
        58,
        90};

    const signed char Parser ::yyr1_[] = {0,
        55,
        56,
        56,
        57,
        57,
        58,
        58,
        58,
        58,
        58,
        58,
        58,
        58,
        58,
        58,
        58,
        59,
        60,
        61,
        61,
        62,
        63,
        63,
        64,
        65,
        65,
        66,
        67,
        67,
        68,
        68,
        68,
        69,
        69,
        70,
        71,
        71,
        71,
        71,
        72,
        72,
        72,
        73,
        73,
        73,
        73,
        74,
        74,
        75,
        75,
        75,
        76,
        76,
        77,
        78,
        78,
        79,
        79,
        80,
        80,
        81,
        81,
        82,
        82,
        82,
        83,
        83,
        83,
        83,
        83,
        84,
        84,
        84,
        85,
        85,
        85,
        85,
        86,
        86,
        87,
        87,
        87,
        87,
        88,
        88,
        89,
        90,
        90,
        91,
        91,
        92,
        92,
        92,
        92,
        93,
        93,
        93,
        94,
        94,
        94};

    const signed char Parser ::yyr2_[] = {0,
        2,
        1,
        2,
        1,
        2,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        2,
        2,
        3,
        2,
        2,
        3,
        4,
        6,
        2,
        3,
        5,
        5,
        7,
        1,
        1,
        1,
        0,
        1,
        8,
        2,
        4,
        5,
        3,
        0,
        1,
        3,
        1,
        3,
        5,
        6,
        1,
        3,
        1,
        3,
        2,
        1,
        3,
        3,
        1,
        3,
        1,
        1,
        1,
        3,
        1,
        3,
        1,
        3,
        3,
        1,
        3,
        3,
        3,
        3,
        1,
        3,
        3,
        1,
        3,
        3,
        3,
        1,
        2,
        1,
        1,
        3,
        1,
        3,
        4,
        3,
        1,
        2,
        1,
        2,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1};

#if YYDEBUG || 1
    // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
    // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
    const char* const Parser ::yytname_[] = {"\"end of file\"",
        "error",
        "\"invalid token\"",
        "INT_CONST",
        "LL_CONST",
        "FLOAT_CONST",
        "STR_CONST",
        "ERR_TOKEN",
        "SLASH_COMMENT",
        "IDENT",
        "INT",
        "FLOAT",
        "VOID",
        "IF",
        "ELSE",
        "FOR",
        "WHILE",
        "CONTINUE",
        "BREAK",
        "SWITCH",
        "CASE",
        "GOTO",
        "DO",
        "RETURN",
        "CONST",
        "SEMICOLON",
        "COMMA",
        "LPAREN",
        "RPAREN",
        "LBRACKET",
        "RBRACKET",
        "LBRACE",
        "RBRACE",
        "NOT",
        "BITOR",
        "BITAND",
        "DOT",
        "END",
        "PLUS",
        "MINUS",
        "UPLUS",
        "UMINUS",
        "STAR",
        "SLASH",
        "GT",
        "GE",
        "LT",
        "LE",
        "EQ",
        "ASSIGN",
        "MOD",
        "NEQ",
        "AND",
        "OR",
        "THEN",
        "$accept",
        "PROGRAM",
        "STMT_LIST",
        "STMT",
        "CONTINUE_STMT",
        "BREAK_STMT",
        "BLOCK_STMT",
        "EXPR_STMT",
        "VAR_DECL_STMT",
        "FUNC_DECL_STMT",
        "RETURN_STMT",
        "WHILE_STMT",
        "IF_STMT",
        "FOR_INIT_STMT",
        "FOR_INCR_STMT",
        "FOR_STMT",
        "FUNC_PARAM_DEF",
        "FUNC_PARAM_DEF_LIST",
        "DEF",
        "DEF_LIST",
        "INITIALIZER",
        "INITIALIZER_LIST",
        "ASSIGN_EXPR",
        "EXPR_LIST",
        "EXPR",
        "LOGICAL_OR_EXPR",
        "LOGICAL_AND_EXPR",
        "EQUALITY_EXPR",
        "RELATIONAL_EXPR",
        "ADDSUB_EXPR",
        "MULDIV_EXPR",
        "UNARY_EXPR",
        "BASIC_EXPR",
        "FUNC_CALL_EXPR",
        "ARRAY_DIMESION_EXPR",
        "ARRAY_DIMESION_EXPR_LIST",
        "LEFT_VAL_EXPR",
        "CONST_EXPR",
        "TYPE",
        "UNARY_OP",
        YY_NULLPTR};
#endif

#if YYDEBUG
    const short Parser ::yyrline_[] = {0,
        141,
        141,
        145,
        151,
        155,
        162,
        165,
        168,
        171,
        174,
        177,
        180,
        183,
        186,
        189,
        192,
        198,
        205,
        212,
        216,
        223,
        230,
        234,
        241,
        248,
        252,
        259,
        266,
        270,
        277,
        280,
        283,
        288,
        291,
        297,
        304,
        308,
        314,
        320,
        327,
        330,
        334,
        340,
        343,
        346,
        352,
        361,
        365,
        372,
        375,
        378,
        384,
        388,
        395,
        401,
        405,
        412,
        416,
        423,
        426,
        432,
        435,
        441,
        444,
        447,
        453,
        456,
        459,
        462,
        465,
        471,
        474,
        477,
        483,
        486,
        489,
        492,
        498,
        501,
        507,
        510,
        513,
        516,
        522,
        526,
        533,
        539,
        543,
        550,
        555,
        563,
        568,
        573,
        578,
        586,
        589,
        592,
        598,
        601,
        604};

    void Parser ::yy_stack_print_() const
    {
        *yycdebug_ << "Stack now";
        for (stack_type::const_iterator i = yystack_.begin(), i_end = yystack_.end(); i != i_end; ++i)
            *yycdebug_ << ' ' << int(i->state);
        *yycdebug_ << '\n';
    }

    void Parser ::yy_reduce_print_(int yyrule) const
    {
        int yylno  = yyrline_[yyrule];
        int yynrhs = yyr2_[yyrule];
        // Print the symbols being reduced, and their result.
        *yycdebug_ << "Reducing stack by rule " << yyrule - 1 << " (line " << yylno << "):\n";
        // The symbols being reduced.
        for (int yyi = 0; yyi < yynrhs; yyi++)
            YY_SYMBOL_PRINT("   $" << yyi + 1 << " =", yystack_[(yynrhs) - (yyi + 1)]);
    }
#endif  // YYDEBUG

#line 4 "parser/yacc.y"
}  // namespace Yacc
#line 2522 "parser/yacc.cpp"

#line 609 "parser/yacc.y"

void Yacc::Parser::error(const Yacc::location& location, const std::string& message)
{
    std::cerr << "msg: " << message << ", error happened at: " << location << std::endl;
}
