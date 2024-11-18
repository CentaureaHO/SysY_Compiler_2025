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

                        case 45:  // DEF: IDENT ARRAY_DIMESION_EXPR_LIST INITIALIZER
#line 346 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[2].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<DefNode*>() = new DefNode(
                                new LeftValueExpr(entry, yystack_[1].value.as<std::vector<ExprNode*>*>(), -1),
                                yystack_[0].value.as<InitNode*>());
                        }
#line 1417 "parser/yacc.cpp"
                        break;

                        case 46:  // DEF: IDENT LBRACKET RBRACKET ASSIGN INITIALIZER
#line 350 "parser/yacc.y"
                        {
                            std::vector<ExprNode*>* dim = new std::vector<ExprNode*>();
                            dim->emplace_back(
                                new ConstExpr(static_cast<InitMulti*>(yystack_[0].value.as<InitNode*>())->getSize()));
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[4].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<DefNode*>() =
                                new DefNode(new LeftValueExpr(entry, dim, -1), yystack_[0].value.as<InitNode*>());
                        }
#line 1428 "parser/yacc.cpp"
                        break;

                        case 47:  // DEF: IDENT LBRACKET RBRACKET INITIALIZER
#line 356 "parser/yacc.y"
                        {
                            std::vector<ExprNode*>* dim = new std::vector<ExprNode*>();
                            dim->emplace_back(
                                new ConstExpr(static_cast<InitMulti*>(yystack_[0].value.as<InitNode*>())->getSize()));
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[3].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<DefNode*>() =
                                new DefNode(new LeftValueExpr(entry, dim, -1), yystack_[0].value.as<InitNode*>());
                        }
#line 1439 "parser/yacc.cpp"
                        break;

                        case 48:  // DEF: IDENT LBRACKET RBRACKET ARRAY_DIMESION_EXPR_LIST ASSIGN INITIALIZER
#line 362 "parser/yacc.y"
                        {
                            std::vector<ExprNode*>* dim = yystack_[2].value.as<std::vector<ExprNode*>*>();
                            dim->insert(dim->begin(),
                                new ConstExpr(static_cast<InitMulti*>(yystack_[0].value.as<InitNode*>())->getSize()));
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[5].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<DefNode*>() =
                                new DefNode(new LeftValueExpr(entry, dim, -1), yystack_[0].value.as<InitNode*>());
                        }
#line 1450 "parser/yacc.cpp"
                        break;

                        case 49:  // DEF: IDENT LBRACKET RBRACKET ARRAY_DIMESION_EXPR_LIST INITIALIZER
#line 368 "parser/yacc.y"
                        {
                            std::vector<ExprNode*>* dim = yystack_[1].value.as<std::vector<ExprNode*>*>();
                            dim->insert(dim->begin(),
                                new ConstExpr(static_cast<InitMulti*>(yystack_[0].value.as<InitNode*>())->getSize()));
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[4].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<DefNode*>() =
                                new DefNode(new LeftValueExpr(entry, dim, -1), yystack_[0].value.as<InitNode*>());
                        }
#line 1461 "parser/yacc.cpp"
                        break;

                        case 50:  // DEF_LIST: DEF
#line 377 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<DefNode*>*>() = new std::vector<DefNode*>();
                            yylhs.value.as<std::vector<DefNode*>*>()->push_back(yystack_[0].value.as<DefNode*>());
                        }
#line 1470 "parser/yacc.cpp"
                        break;

                        case 51:  // DEF_LIST: DEF_LIST COMMA DEF
#line 381 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<DefNode*>*>() = yystack_[2].value.as<std::vector<DefNode*>*>();
                            yylhs.value.as<std::vector<DefNode*>*>()->push_back(yystack_[0].value.as<DefNode*>());
                        }
#line 1479 "parser/yacc.cpp"
                        break;

                        case 52:  // INITIALIZER: EXPR
#line 388 "parser/yacc.y"
                        {
                            yylhs.value.as<InitNode*>() = new InitSingle(yystack_[0].value.as<ExprNode*>());
                        }
#line 1487 "parser/yacc.cpp"
                        break;

                        case 53:  // INITIALIZER: LBRACE INITIALIZER_LIST RBRACE
#line 391 "parser/yacc.y"
                        {
                            yylhs.value.as<InitNode*>() =
                                new InitMulti(yystack_[1].value.as<std::vector<InitNode*>*>());
                        }
#line 1495 "parser/yacc.cpp"
                        break;

                        case 54:  // INITIALIZER: LBRACE RBRACE
#line 394 "parser/yacc.y"
                        {
                            yylhs.value.as<InitNode*>() = new InitMulti(nullptr);
                        }
#line 1503 "parser/yacc.cpp"
                        break;

                        case 55:  // INITIALIZER_LIST: INITIALIZER
#line 400 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<InitNode*>*>() = new std::vector<InitNode*>();
                            yylhs.value.as<std::vector<InitNode*>*>()->push_back(yystack_[0].value.as<InitNode*>());
                        }
#line 1512 "parser/yacc.cpp"
                        break;

                        case 56:  // INITIALIZER_LIST: INITIALIZER_LIST COMMA INITIALIZER
#line 404 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<InitNode*>*>() = yystack_[2].value.as<std::vector<InitNode*>*>();
                            yylhs.value.as<std::vector<InitNode*>*>()->push_back(yystack_[0].value.as<InitNode*>());
                        }
#line 1521 "parser/yacc.cpp"
                        break;

                        case 57:  // ASSIGN_EXPR: LEFT_VAL_EXPR ASSIGN EXPR
#line 411 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Assign, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1529 "parser/yacc.cpp"
                        break;

                        case 58:  // EXPR_LIST: EXPR
#line 417 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<ExprNode*>*>() = new std::vector<ExprNode*>();
                            yylhs.value.as<std::vector<ExprNode*>*>()->push_back(yystack_[0].value.as<ExprNode*>());
                        }
#line 1538 "parser/yacc.cpp"
                        break;

                        case 59:  // EXPR_LIST: EXPR_LIST COMMA EXPR
#line 421 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<ExprNode*>*>() = yystack_[2].value.as<std::vector<ExprNode*>*>();
                            yylhs.value.as<std::vector<ExprNode*>*>()->push_back(yystack_[0].value.as<ExprNode*>());
                        }
#line 1547 "parser/yacc.cpp"
                        break;

                        case 60:  // EXPR: LOGICAL_OR_EXPR
#line 428 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1556 "parser/yacc.cpp"
                        break;

                        case 61:  // EXPR: ASSIGN_EXPR
#line 432 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1565 "parser/yacc.cpp"
                        break;

                        case 62:  // LOGICAL_OR_EXPR: LOGICAL_AND_EXPR
#line 439 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1573 "parser/yacc.cpp"
                        break;

                        case 63:  // LOGICAL_OR_EXPR: LOGICAL_OR_EXPR OR LOGICAL_AND_EXPR
#line 442 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Or, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1581 "parser/yacc.cpp"
                        break;

                        case 64:  // LOGICAL_AND_EXPR: EQUALITY_EXPR
#line 448 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1589 "parser/yacc.cpp"
                        break;

                        case 65:  // LOGICAL_AND_EXPR: LOGICAL_AND_EXPR AND EQUALITY_EXPR
#line 451 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::And, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1597 "parser/yacc.cpp"
                        break;

                        case 66:  // EQUALITY_EXPR: RELATIONAL_EXPR
#line 457 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1605 "parser/yacc.cpp"
                        break;

                        case 67:  // EQUALITY_EXPR: EQUALITY_EXPR EQ RELATIONAL_EXPR
#line 460 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Eq, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1613 "parser/yacc.cpp"
                        break;

                        case 68:  // EQUALITY_EXPR: EQUALITY_EXPR NEQ RELATIONAL_EXPR
#line 463 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Neq, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1621 "parser/yacc.cpp"
                        break;

                        case 69:  // RELATIONAL_EXPR: ADDSUB_EXPR
#line 469 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1629 "parser/yacc.cpp"
                        break;

                        case 70:  // RELATIONAL_EXPR: RELATIONAL_EXPR GT ADDSUB_EXPR
#line 472 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Gt, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1637 "parser/yacc.cpp"
                        break;

                        case 71:  // RELATIONAL_EXPR: RELATIONAL_EXPR GE ADDSUB_EXPR
#line 475 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Ge, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1645 "parser/yacc.cpp"
                        break;

                        case 72:  // RELATIONAL_EXPR: RELATIONAL_EXPR LT ADDSUB_EXPR
#line 478 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Lt, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1653 "parser/yacc.cpp"
                        break;

                        case 73:  // RELATIONAL_EXPR: RELATIONAL_EXPR LE ADDSUB_EXPR
#line 481 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Le, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1661 "parser/yacc.cpp"
                        break;

                        case 74:  // ADDSUB_EXPR: MULDIV_EXPR
#line 487 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1669 "parser/yacc.cpp"
                        break;

                        case 75:  // ADDSUB_EXPR: ADDSUB_EXPR PLUS MULDIV_EXPR
#line 490 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Add, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1677 "parser/yacc.cpp"
                        break;

                        case 76:  // ADDSUB_EXPR: ADDSUB_EXPR MINUS MULDIV_EXPR
#line 493 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Sub, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1685 "parser/yacc.cpp"
                        break;

                        case 77:  // MULDIV_EXPR: UNARY_EXPR
#line 499 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1693 "parser/yacc.cpp"
                        break;

                        case 78:  // MULDIV_EXPR: MULDIV_EXPR STAR UNARY_EXPR
#line 502 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Mul, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1701 "parser/yacc.cpp"
                        break;

                        case 79:  // MULDIV_EXPR: MULDIV_EXPR SLASH UNARY_EXPR
#line 505 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Div, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1709 "parser/yacc.cpp"
                        break;

                        case 80:  // MULDIV_EXPR: MULDIV_EXPR MOD UNARY_EXPR
#line 508 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new BinaryExpr(
                                OpCode::Mod, yystack_[2].value.as<ExprNode*>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1717 "parser/yacc.cpp"
                        break;

                        case 81:  // UNARY_EXPR: BASIC_EXPR
#line 514 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1725 "parser/yacc.cpp"
                        break;

                        case 82:  // UNARY_EXPR: UNARY_OP UNARY_EXPR
#line 517 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() =
                                new UnaryExpr(yystack_[1].value.as<OpCode>(), yystack_[0].value.as<ExprNode*>());
                        }
#line 1733 "parser/yacc.cpp"
                        break;

                        case 83:  // BASIC_EXPR: CONST_EXPR
#line 523 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1741 "parser/yacc.cpp"
                        break;

                        case 84:  // BASIC_EXPR: LEFT_VAL_EXPR
#line 526 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1749 "parser/yacc.cpp"
                        break;

                        case 85:  // BASIC_EXPR: LPAREN EXPR RPAREN
#line 529 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[1].value.as<ExprNode*>();
                        }
#line 1757 "parser/yacc.cpp"
                        break;

                        case 86:  // BASIC_EXPR: FUNC_CALL_EXPR
#line 532 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[0].value.as<ExprNode*>();
                        }
#line 1765 "parser/yacc.cpp"
                        break;

                        case 87:  // FUNC_CALL_EXPR: IDENT LPAREN RPAREN
#line 538 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[2].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>() = new FuncCallExpr(entry, nullptr);
                        }
#line 1774 "parser/yacc.cpp"
                        break;

                        case 88:  // FUNC_CALL_EXPR: IDENT LPAREN EXPR_LIST RPAREN
#line 542 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[3].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>() =
                                new FuncCallExpr(entry, yystack_[1].value.as<std::vector<ExprNode*>*>());
                        }
#line 1783 "parser/yacc.cpp"
                        break;

                        case 89:  // ARRAY_DIMESION_EXPR: LBRACKET EXPR RBRACKET
#line 549 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = yystack_[1].value.as<ExprNode*>();
                        }
#line 1791 "parser/yacc.cpp"
                        break;

                        case 90:  // ARRAY_DIMESION_EXPR_LIST: ARRAY_DIMESION_EXPR
#line 555 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<ExprNode*>*>() = new std::vector<ExprNode*>();
                            yylhs.value.as<std::vector<ExprNode*>*>()->push_back(yystack_[0].value.as<ExprNode*>());
                        }
#line 1800 "parser/yacc.cpp"
                        break;

                        case 91:  // ARRAY_DIMESION_EXPR_LIST: ARRAY_DIMESION_EXPR_LIST ARRAY_DIMESION_EXPR
#line 559 "parser/yacc.y"
                        {
                            yylhs.value.as<std::vector<ExprNode*>*>() = yystack_[1].value.as<std::vector<ExprNode*>*>();
                            yylhs.value.as<std::vector<ExprNode*>*>()->push_back(yystack_[0].value.as<ExprNode*>());
                        }
#line 1809 "parser/yacc.cpp"
                        break;

                        case 92:  // LEFT_VAL_EXPR: IDENT
#line 566 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[0].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>() = new LeftValueExpr(entry, nullptr, -1);
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1819 "parser/yacc.cpp"
                        break;

                        case 93:  // LEFT_VAL_EXPR: IDENT ARRAY_DIMESION_EXPR_LIST
#line 571 "parser/yacc.y"
                        {
                            Symbol::Entry* entry =
                                Symbol::Entry::getEntry(*yystack_[1].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>() =
                                new LeftValueExpr(entry, yystack_[0].value.as<std::vector<ExprNode*>*>(), -1);
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[1].location.begin.line);
                        }
#line 1829 "parser/yacc.cpp"
                        break;

                        case 94:  // CONST_EXPR: INT_CONST
#line 579 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new ConstExpr(yystack_[0].value.as<int>());
                            yylhs.value.as<ExprNode*>()->setConst();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1839 "parser/yacc.cpp"
                        break;

                        case 95:  // CONST_EXPR: LL_CONST
#line 584 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new ConstExpr(yystack_[0].value.as<long long>());
                            yylhs.value.as<ExprNode*>()->setConst();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1849 "parser/yacc.cpp"
                        break;

                        case 96:  // CONST_EXPR: FLOAT_CONST
#line 589 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() = new ConstExpr(yystack_[0].value.as<float>());
                            yylhs.value.as<ExprNode*>()->setConst();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1859 "parser/yacc.cpp"
                        break;

                        case 97:  // CONST_EXPR: STR_CONST
#line 594 "parser/yacc.y"
                        {
                            yylhs.value.as<ExprNode*>() =
                                new ConstExpr(yystack_[0].value.as<std::shared_ptr<std::string> >());
                            yylhs.value.as<ExprNode*>()->setConst();
                            yylhs.value.as<ExprNode*>()->set_line(yystack_[0].location.begin.line);
                        }
#line 1869 "parser/yacc.cpp"
                        break;

                        case 98:  // TYPE: INT
#line 602 "parser/yacc.y"
                        {
                            yylhs.value.as<Type*>() = intType;
                        }
#line 1877 "parser/yacc.cpp"
                        break;

                        case 99:  // TYPE: FLOAT
#line 605 "parser/yacc.y"
                        {
                            yylhs.value.as<Type*>() = floatType;
                        }
#line 1885 "parser/yacc.cpp"
                        break;

                        case 100:  // TYPE: VOID
#line 608 "parser/yacc.y"
                        {
                            yylhs.value.as<Type*>() = voidType;
                        }
#line 1893 "parser/yacc.cpp"
                        break;

                        case 101:  // UNARY_OP: PLUS
#line 614 "parser/yacc.y"
                        {
                            yylhs.value.as<OpCode>() = OpCode::Add;
                        }
#line 1901 "parser/yacc.cpp"
                        break;

                        case 102:  // UNARY_OP: MINUS
#line 617 "parser/yacc.y"
                        {
                            yylhs.value.as<OpCode>() = OpCode::Sub;
                        }
#line 1909 "parser/yacc.cpp"
                        break;

                        case 103:  // UNARY_OP: NOT
#line 620 "parser/yacc.y"
                        {
                            yylhs.value.as<OpCode>() = OpCode::Not;
                        }
#line 1917 "parser/yacc.cpp"
                        break;

#line 1921 "parser/yacc.cpp"

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

    const signed char Parser ::yypact_ninf_ = -53;

    const signed char Parser ::yytable_ninf_ = -1;

    const short Parser ::yypact_[] = {260,
        -53,
        -53,
        -53,
        -53,
        -22,
        -53,
        -53,
        -53,
        -18,
        -4,
        3,
        12,
        23,
        348,
        9,
        -53,
        400,
        198,
        -53,
        -53,
        -53,
        17,
        260,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        37,
        -53,
        40,
        43,
        -19,
        21,
        31,
        -28,
        -53,
        -53,
        -53,
        10,
        -53,
        98,
        400,
        355,
        400,
        -53,
        79,
        400,
        291,
        400,
        -53,
        -53,
        -53,
        87,
        107,
        92,
        -53,
        229,
        -53,
        -53,
        -53,
        -53,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        400,
        6,
        -53,
        62,
        73,
        -53,
        -53,
        -53,
        15,
        93,
        -53,
        99,
        -53,
        -53,
        -53,
        400,
        107,
        101,
        -53,
        97,
        72,
        -53,
        -53,
        -53,
        43,
        -19,
        21,
        21,
        31,
        31,
        31,
        31,
        -28,
        -28,
        -53,
        -53,
        -53,
        -53,
        9,
        362,
        304,
        -53,
        107,
        22,
        -53,
        -53,
        260,
        105,
        260,
        -53,
        -53,
        66,
        122,
        7,
        317,
        -53,
        -53,
        -53,
        -53,
        119,
        400,
        -53,
        9,
        103,
        106,
        22,
        -53,
        151,
        -53,
        -53,
        -8,
        260,
        111,
        115,
        -53,
        -53,
        393,
        79,
        -53,
        22,
        -53,
        22,
        -53,
        -53,
        260,
        79,
        -53,
        -53,
        -53,
        79};

    const signed char Parser ::yydefact_[] = {0,
        94,
        95,
        96,
        97,
        92,
        98,
        99,
        100,
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
        103,
        101,
        102,
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
        61,
        0,
        58,
        60,
        62,
        64,
        66,
        69,
        74,
        77,
        81,
        86,
        84,
        83,
        0,
        0,
        0,
        0,
        90,
        93,
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
        92,
        50,
        0,
        43,
        82,
        84,
        87,
        0,
        0,
        91,
        0,
        32,
        30,
        31,
        0,
        0,
        0,
        26,
        92,
        0,
        85,
        19,
        59,
        63,
        65,
        67,
        68,
        70,
        71,
        72,
        73,
        75,
        76,
        78,
        79,
        80,
        57,
        40,
        0,
        93,
        22,
        0,
        0,
        88,
        89,
        0,
        0,
        0,
        23,
        41,
        0,
        0,
        0,
        0,
        45,
        52,
        51,
        44,
        28,
        33,
        27,
        0,
        0,
        36,
        0,
        47,
        0,
        54,
        55,
        0,
        0,
        0,
        34,
        42,
        24,
        0,
        39,
        46,
        0,
        49,
        0,
        53,
        29,
        0,
        37,
        48,
        56,
        35,
        38};

    const signed char Parser ::yypgoto_[] = {-53,
        -53,
        124,
        -15,
        -53,
        -53,
        -3,
        89,
        90,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        -53,
        5,
        -53,
        35,
        86,
        32,
        -53,
        -53,
        -48,
        -13,
        -53,
        78,
        91,
        27,
        28,
        39,
        8,
        -53,
        -53,
        -52,
        -5,
        2,
        -53,
        -9,
        -53};

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
        156,
        34,
        134,
        135,
        86,
        87,
        139,
        154,
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
        124,
        47,
        48,
        49,
        50};

    const unsigned char Parser ::yytable_[] = {54,
        61,
        94,
        92,
        63,
        51,
        62,
        52,
        68,
        55,
        1,
        2,
        3,
        4,
        81,
        82,
        5,
        66,
        165,
        6,
        7,
        8,
        83,
        56,
        166,
        1,
        2,
        3,
        4,
        73,
        57,
        5,
        74,
        122,
        17,
        123,
        52,
        58,
        138,
        93,
        19,
        70,
        95,
        128,
        101,
        20,
        21,
        100,
        59,
        17,
        68,
        88,
        90,
        138,
        67,
        19,
        149,
        107,
        89,
        84,
        20,
        21,
        69,
        70,
        88,
        75,
        76,
        77,
        78,
        79,
        80,
        121,
        94,
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
        131,
        125,
        126,
        118,
        119,
        120,
        146,
        71,
        147,
        72,
        157,
        133,
        126,
        94,
        110,
        111,
        88,
        112,
        113,
        114,
        115,
        85,
        52,
        94,
        93,
        140,
        102,
        136,
        140,
        143,
        103,
        145,
        116,
        117,
        105,
        94,
        127,
        129,
        140,
        140,
        123,
        130,
        88,
        132,
        144,
        148,
        151,
        155,
        18,
        160,
        140,
        136,
        140,
        168,
        167,
        70,
        65,
        161,
        159,
        97,
        98,
        93,
        104,
        108,
        140,
        158,
        140,
        172,
        1,
        2,
        3,
        4,
        0,
        142,
        5,
        141,
        0,
        109,
        173,
        0,
        0,
        0,
        0,
        150,
        153,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        17,
        0,
        52,
        162,
        138,
        164,
        19,
        0,
        0,
        0,
        0,
        20,
        21,
        0,
        0,
        0,
        0,
        170,
        0,
        171,
        0,
        0,
        163,
        1,
        2,
        3,
        4,
        0,
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
        64,
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
        106,
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
        0,
        0,
        0,
        1,
        2,
        3,
        4,
        0,
        0,
        5,
        0,
        15,
        96,
        0,
        17,
        0,
        1,
        2,
        3,
        4,
        19,
        0,
        5,
        0,
        0,
        20,
        21,
        17,
        0,
        52,
        0,
        138,
        0,
        19,
        0,
        0,
        0,
        0,
        20,
        21,
        17,
        0,
        0,
        0,
        138,
        152,
        19,
        1,
        2,
        3,
        4,
        20,
        21,
        5,
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
        60,
        0,
        17,
        0,
        0,
        0,
        0,
        0,
        19,
        17,
        91,
        0,
        0,
        20,
        21,
        19,
        17,
        0,
        0,
        137,
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
        1,
        2,
        3,
        4,
        0,
        0,
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
        17,
        0,
        0,
        169,
        0,
        0,
        19,
        17,
        0,
        0,
        0,
        20,
        21,
        19,
        0,
        0,
        0,
        0,
        20,
        21};

    const short Parser ::yycheck_[] = {5,
        14,
        54,
        51,
        17,
        27,
        15,
        29,
        23,
        27,
        3,
        4,
        5,
        6,
        42,
        43,
        9,
        0,
        26,
        10,
        11,
        12,
        50,
        27,
        32,
        3,
        4,
        5,
        6,
        48,
        27,
        9,
        51,
        27,
        27,
        29,
        29,
        25,
        31,
        52,
        33,
        26,
        55,
        28,
        57,
        38,
        39,
        56,
        25,
        27,
        65,
        49,
        50,
        31,
        37,
        33,
        49,
        70,
        50,
        49,
        38,
        39,
        25,
        26,
        62,
        44,
        45,
        46,
        47,
        38,
        39,
        84,
        124,
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
        99,
        25,
        26,
        81,
        82,
        83,
        26,
        53,
        28,
        52,
        144,
        25,
        26,
        151,
        73,
        74,
        100,
        75,
        76,
        77,
        78,
        9,
        29,
        161,
        123,
        124,
        25,
        122,
        127,
        130,
        9,
        132,
        79,
        80,
        28,
        173,
        49,
        30,
        137,
        138,
        29,
        28,
        126,
        28,
        25,
        9,
        137,
        14,
        31,
        29,
        149,
        146,
        151,
        28,
        155,
        26,
        18,
        148,
        147,
        56,
        56,
        160,
        62,
        71,
        163,
        146,
        165,
        168,
        3,
        4,
        5,
        6,
        -1,
        127,
        9,
        126,
        -1,
        72,
        169,
        -1,
        -1,
        -1,
        -1,
        137,
        138,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        -1,
        27,
        -1,
        29,
        149,
        31,
        151,
        33,
        -1,
        -1,
        -1,
        -1,
        38,
        39,
        -1,
        -1,
        -1,
        -1,
        163,
        -1,
        165,
        -1,
        -1,
        49,
        3,
        4,
        5,
        6,
        -1,
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
        32,
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
        32,
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
        -1,
        -1,
        -1,
        3,
        4,
        5,
        6,
        -1,
        -1,
        9,
        -1,
        24,
        25,
        -1,
        27,
        -1,
        3,
        4,
        5,
        6,
        33,
        -1,
        9,
        -1,
        -1,
        38,
        39,
        27,
        -1,
        29,
        -1,
        31,
        -1,
        33,
        -1,
        -1,
        -1,
        -1,
        38,
        39,
        27,
        -1,
        -1,
        -1,
        31,
        32,
        33,
        3,
        4,
        5,
        6,
        38,
        39,
        9,
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
        25,
        -1,
        27,
        -1,
        -1,
        -1,
        -1,
        -1,
        33,
        27,
        28,
        -1,
        -1,
        38,
        39,
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
        3,
        4,
        5,
        6,
        -1,
        -1,
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
        27,
        -1,
        -1,
        30,
        -1,
        -1,
        33,
        27,
        -1,
        -1,
        -1,
        38,
        39,
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
        90,
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
        31,
        75,
        79,
        73,
        75,
        58,
        25,
        58,
        26,
        28,
        9,
        49,
        75,
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
        75,
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
        3,
        5,
        4,
        6,
        5,
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
        350,
        356,
        362,
        368,
        377,
        381,
        388,
        391,
        394,
        400,
        404,
        411,
        417,
        421,
        428,
        432,
        439,
        442,
        448,
        451,
        457,
        460,
        463,
        469,
        472,
        475,
        478,
        481,
        487,
        490,
        493,
        499,
        502,
        505,
        508,
        514,
        517,
        523,
        526,
        529,
        532,
        538,
        542,
        549,
        555,
        559,
        566,
        571,
        579,
        584,
        589,
        594,
        602,
        605,
        608,
        614,
        617,
        620};

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
#line 2570 "parser/yacc.cpp"

#line 625 "parser/yacc.y"

void Yacc::Parser::error(const Yacc::location& location, const std::string& message)
{
    std::cerr << "msg: " << message << ", error happened at: " << location << std::endl;
}
