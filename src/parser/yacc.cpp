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
#line 30 "parser/yacc.y"

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

#line 62 "parser/yacc.cpp"




#include <parser/yacc.hpp>




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 4 "parser/yacc.y"
namespace  Yacc  {
#line 162 "parser/yacc.cpp"

  /// Build a parser object.
   Parser :: Parser  (Yacc::Scanner& scanner_yyarg, Yacc::Driver& driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

   Parser ::~ Parser  ()
  {}

   Parser ::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
   Parser ::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

   Parser ::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
   Parser ::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
   Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

   Parser ::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

   Parser ::symbol_kind_type
   Parser ::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

   Parser ::stack_symbol_type::stack_symbol_type ()
  {}

   Parser ::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_PROGRAM: // PROGRAM
        value.YY_MOVE_OR_COPY< ASTree* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DEF: // DEF
        value.YY_MOVE_OR_COPY< DefNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN_EXPR: // ASSIGN_EXPR
      case symbol_kind::S_EXPR: // EXPR
      case symbol_kind::S_LOGICAL_OR_EXPR: // LOGICAL_OR_EXPR
      case symbol_kind::S_LOGICAL_AND_EXPR: // LOGICAL_AND_EXPR
      case symbol_kind::S_EQUALITY_EXPR: // EQUALITY_EXPR
      case symbol_kind::S_RELATIONAL_EXPR: // RELATIONAL_EXPR
      case symbol_kind::S_ADDSUB_EXPR: // ADDSUB_EXPR
      case symbol_kind::S_MULDIV_EXPR: // MULDIV_EXPR
      case symbol_kind::S_UNARY_EXPR: // UNARY_EXPR
      case symbol_kind::S_BASIC_EXPR: // BASIC_EXPR
      case symbol_kind::S_ARRAY_DIMESION_EXPR: // ARRAY_DIMESION_EXPR
      case symbol_kind::S_LEFT_VAL_EXPR: // LEFT_VAL_EXPR
      case symbol_kind::S_CONST_EXPR: // CONST_EXPR
        value.YY_MOVE_OR_COPY< ExprNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INITIALIZER: // INITIALIZER
        value.YY_MOVE_OR_COPY< InitNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UNARY_OP: // UNARY_OP
        value.YY_MOVE_OR_COPY< OpCode > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STMT: // STMT
      case symbol_kind::S_BLOCK_STMT: // BLOCK_STMT
      case symbol_kind::S_EXPR_STMT: // EXPR_STMT
      case symbol_kind::S_VAR_DECL_STMT: // VAR_DECL_STMT
        value.YY_MOVE_OR_COPY< StmtNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TYPE: // TYPE
        value.YY_MOVE_OR_COPY< Type* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LL_CONST: // LL_CONST
        value.YY_MOVE_OR_COPY< long long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STR_CONST: // STR_CONST
      case symbol_kind::S_ERR_TOKEN: // ERR_TOKEN
      case symbol_kind::S_SLASH_COMMENT: // SLASH_COMMENT
      case symbol_kind::S_IDENT: // IDENT
        value.YY_MOVE_OR_COPY< std::shared_ptr<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DEF_LIST: // DEF_LIST
        value.YY_MOVE_OR_COPY< std::vector<DefNode*>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST: // ARRAY_DIMESION_EXPR_LIST
        value.YY_MOVE_OR_COPY< std::vector<ExprNode*>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INITIALIZER_LIST: // INITIALIZER_LIST
        value.YY_MOVE_OR_COPY< std::vector<InitNode*>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STMT_LIST: // STMT_LIST
        value.YY_MOVE_OR_COPY< std::vector<StmtNode*>* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

   Parser ::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_PROGRAM: // PROGRAM
        value.move< ASTree* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DEF: // DEF
        value.move< DefNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ASSIGN_EXPR: // ASSIGN_EXPR
      case symbol_kind::S_EXPR: // EXPR
      case symbol_kind::S_LOGICAL_OR_EXPR: // LOGICAL_OR_EXPR
      case symbol_kind::S_LOGICAL_AND_EXPR: // LOGICAL_AND_EXPR
      case symbol_kind::S_EQUALITY_EXPR: // EQUALITY_EXPR
      case symbol_kind::S_RELATIONAL_EXPR: // RELATIONAL_EXPR
      case symbol_kind::S_ADDSUB_EXPR: // ADDSUB_EXPR
      case symbol_kind::S_MULDIV_EXPR: // MULDIV_EXPR
      case symbol_kind::S_UNARY_EXPR: // UNARY_EXPR
      case symbol_kind::S_BASIC_EXPR: // BASIC_EXPR
      case symbol_kind::S_ARRAY_DIMESION_EXPR: // ARRAY_DIMESION_EXPR
      case symbol_kind::S_LEFT_VAL_EXPR: // LEFT_VAL_EXPR
      case symbol_kind::S_CONST_EXPR: // CONST_EXPR
        value.move< ExprNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INITIALIZER: // INITIALIZER
        value.move< InitNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UNARY_OP: // UNARY_OP
        value.move< OpCode > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STMT: // STMT
      case symbol_kind::S_BLOCK_STMT: // BLOCK_STMT
      case symbol_kind::S_EXPR_STMT: // EXPR_STMT
      case symbol_kind::S_VAR_DECL_STMT: // VAR_DECL_STMT
        value.move< StmtNode* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TYPE: // TYPE
        value.move< Type* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.move< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_LL_CONST: // LL_CONST
        value.move< long long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STR_CONST: // STR_CONST
      case symbol_kind::S_ERR_TOKEN: // ERR_TOKEN
      case symbol_kind::S_SLASH_COMMENT: // SLASH_COMMENT
      case symbol_kind::S_IDENT: // IDENT
        value.move< std::shared_ptr<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DEF_LIST: // DEF_LIST
        value.move< std::vector<DefNode*>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST: // ARRAY_DIMESION_EXPR_LIST
        value.move< std::vector<ExprNode*>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INITIALIZER_LIST: // INITIALIZER_LIST
        value.move< std::vector<InitNode*>* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STMT_LIST: // STMT_LIST
        value.move< std::vector<StmtNode*>* > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_PROGRAM: // PROGRAM
        value.copy< ASTree* > (that.value);
        break;

      case symbol_kind::S_DEF: // DEF
        value.copy< DefNode* > (that.value);
        break;

      case symbol_kind::S_ASSIGN_EXPR: // ASSIGN_EXPR
      case symbol_kind::S_EXPR: // EXPR
      case symbol_kind::S_LOGICAL_OR_EXPR: // LOGICAL_OR_EXPR
      case symbol_kind::S_LOGICAL_AND_EXPR: // LOGICAL_AND_EXPR
      case symbol_kind::S_EQUALITY_EXPR: // EQUALITY_EXPR
      case symbol_kind::S_RELATIONAL_EXPR: // RELATIONAL_EXPR
      case symbol_kind::S_ADDSUB_EXPR: // ADDSUB_EXPR
      case symbol_kind::S_MULDIV_EXPR: // MULDIV_EXPR
      case symbol_kind::S_UNARY_EXPR: // UNARY_EXPR
      case symbol_kind::S_BASIC_EXPR: // BASIC_EXPR
      case symbol_kind::S_ARRAY_DIMESION_EXPR: // ARRAY_DIMESION_EXPR
      case symbol_kind::S_LEFT_VAL_EXPR: // LEFT_VAL_EXPR
      case symbol_kind::S_CONST_EXPR: // CONST_EXPR
        value.copy< ExprNode* > (that.value);
        break;

      case symbol_kind::S_INITIALIZER: // INITIALIZER
        value.copy< InitNode* > (that.value);
        break;

      case symbol_kind::S_UNARY_OP: // UNARY_OP
        value.copy< OpCode > (that.value);
        break;

      case symbol_kind::S_STMT: // STMT
      case symbol_kind::S_BLOCK_STMT: // BLOCK_STMT
      case symbol_kind::S_EXPR_STMT: // EXPR_STMT
      case symbol_kind::S_VAR_DECL_STMT: // VAR_DECL_STMT
        value.copy< StmtNode* > (that.value);
        break;

      case symbol_kind::S_TYPE: // TYPE
        value.copy< Type* > (that.value);
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.copy< float > (that.value);
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_LL_CONST: // LL_CONST
        value.copy< long long > (that.value);
        break;

      case symbol_kind::S_STR_CONST: // STR_CONST
      case symbol_kind::S_ERR_TOKEN: // ERR_TOKEN
      case symbol_kind::S_SLASH_COMMENT: // SLASH_COMMENT
      case symbol_kind::S_IDENT: // IDENT
        value.copy< std::shared_ptr<std::string> > (that.value);
        break;

      case symbol_kind::S_DEF_LIST: // DEF_LIST
        value.copy< std::vector<DefNode*>* > (that.value);
        break;

      case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST: // ARRAY_DIMESION_EXPR_LIST
        value.copy< std::vector<ExprNode*>* > (that.value);
        break;

      case symbol_kind::S_INITIALIZER_LIST: // INITIALIZER_LIST
        value.copy< std::vector<InitNode*>* > (that.value);
        break;

      case symbol_kind::S_STMT_LIST: // STMT_LIST
        value.copy< std::vector<StmtNode*>* > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_PROGRAM: // PROGRAM
        value.move< ASTree* > (that.value);
        break;

      case symbol_kind::S_DEF: // DEF
        value.move< DefNode* > (that.value);
        break;

      case symbol_kind::S_ASSIGN_EXPR: // ASSIGN_EXPR
      case symbol_kind::S_EXPR: // EXPR
      case symbol_kind::S_LOGICAL_OR_EXPR: // LOGICAL_OR_EXPR
      case symbol_kind::S_LOGICAL_AND_EXPR: // LOGICAL_AND_EXPR
      case symbol_kind::S_EQUALITY_EXPR: // EQUALITY_EXPR
      case symbol_kind::S_RELATIONAL_EXPR: // RELATIONAL_EXPR
      case symbol_kind::S_ADDSUB_EXPR: // ADDSUB_EXPR
      case symbol_kind::S_MULDIV_EXPR: // MULDIV_EXPR
      case symbol_kind::S_UNARY_EXPR: // UNARY_EXPR
      case symbol_kind::S_BASIC_EXPR: // BASIC_EXPR
      case symbol_kind::S_ARRAY_DIMESION_EXPR: // ARRAY_DIMESION_EXPR
      case symbol_kind::S_LEFT_VAL_EXPR: // LEFT_VAL_EXPR
      case symbol_kind::S_CONST_EXPR: // CONST_EXPR
        value.move< ExprNode* > (that.value);
        break;

      case symbol_kind::S_INITIALIZER: // INITIALIZER
        value.move< InitNode* > (that.value);
        break;

      case symbol_kind::S_UNARY_OP: // UNARY_OP
        value.move< OpCode > (that.value);
        break;

      case symbol_kind::S_STMT: // STMT
      case symbol_kind::S_BLOCK_STMT: // BLOCK_STMT
      case symbol_kind::S_EXPR_STMT: // EXPR_STMT
      case symbol_kind::S_VAR_DECL_STMT: // VAR_DECL_STMT
        value.move< StmtNode* > (that.value);
        break;

      case symbol_kind::S_TYPE: // TYPE
        value.move< Type* > (that.value);
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        value.move< float > (that.value);
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        value.move< int > (that.value);
        break;

      case symbol_kind::S_LL_CONST: // LL_CONST
        value.move< long long > (that.value);
        break;

      case symbol_kind::S_STR_CONST: // STR_CONST
      case symbol_kind::S_ERR_TOKEN: // ERR_TOKEN
      case symbol_kind::S_SLASH_COMMENT: // SLASH_COMMENT
      case symbol_kind::S_IDENT: // IDENT
        value.move< std::shared_ptr<std::string> > (that.value);
        break;

      case symbol_kind::S_DEF_LIST: // DEF_LIST
        value.move< std::vector<DefNode*>* > (that.value);
        break;

      case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST: // ARRAY_DIMESION_EXPR_LIST
        value.move< std::vector<ExprNode*>* > (that.value);
        break;

      case symbol_kind::S_INITIALIZER_LIST: // INITIALIZER_LIST
        value.move< std::vector<InitNode*>* > (that.value);
        break;

      case symbol_kind::S_STMT_LIST: // STMT_LIST
        value.move< std::vector<StmtNode*>* > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
   Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
   Parser ::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
   Parser ::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
   Parser ::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
   Parser ::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   Parser ::debug_level_type
   Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

   Parser ::state_type
   Parser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
   Parser ::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
   Parser ::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
   Parser ::operator() ()
  {
    return parse ();
  }

  int
   Parser ::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (scanner, driver));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
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
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_PROGRAM: // PROGRAM
        yylhs.value.emplace< ASTree* > ();
        break;

      case symbol_kind::S_DEF: // DEF
        yylhs.value.emplace< DefNode* > ();
        break;

      case symbol_kind::S_ASSIGN_EXPR: // ASSIGN_EXPR
      case symbol_kind::S_EXPR: // EXPR
      case symbol_kind::S_LOGICAL_OR_EXPR: // LOGICAL_OR_EXPR
      case symbol_kind::S_LOGICAL_AND_EXPR: // LOGICAL_AND_EXPR
      case symbol_kind::S_EQUALITY_EXPR: // EQUALITY_EXPR
      case symbol_kind::S_RELATIONAL_EXPR: // RELATIONAL_EXPR
      case symbol_kind::S_ADDSUB_EXPR: // ADDSUB_EXPR
      case symbol_kind::S_MULDIV_EXPR: // MULDIV_EXPR
      case symbol_kind::S_UNARY_EXPR: // UNARY_EXPR
      case symbol_kind::S_BASIC_EXPR: // BASIC_EXPR
      case symbol_kind::S_ARRAY_DIMESION_EXPR: // ARRAY_DIMESION_EXPR
      case symbol_kind::S_LEFT_VAL_EXPR: // LEFT_VAL_EXPR
      case symbol_kind::S_CONST_EXPR: // CONST_EXPR
        yylhs.value.emplace< ExprNode* > ();
        break;

      case symbol_kind::S_INITIALIZER: // INITIALIZER
        yylhs.value.emplace< InitNode* > ();
        break;

      case symbol_kind::S_UNARY_OP: // UNARY_OP
        yylhs.value.emplace< OpCode > ();
        break;

      case symbol_kind::S_STMT: // STMT
      case symbol_kind::S_BLOCK_STMT: // BLOCK_STMT
      case symbol_kind::S_EXPR_STMT: // EXPR_STMT
      case symbol_kind::S_VAR_DECL_STMT: // VAR_DECL_STMT
        yylhs.value.emplace< StmtNode* > ();
        break;

      case symbol_kind::S_TYPE: // TYPE
        yylhs.value.emplace< Type* > ();
        break;

      case symbol_kind::S_FLOAT_CONST: // FLOAT_CONST
        yylhs.value.emplace< float > ();
        break;

      case symbol_kind::S_INT_CONST: // INT_CONST
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_LL_CONST: // LL_CONST
        yylhs.value.emplace< long long > ();
        break;

      case symbol_kind::S_STR_CONST: // STR_CONST
      case symbol_kind::S_ERR_TOKEN: // ERR_TOKEN
      case symbol_kind::S_SLASH_COMMENT: // SLASH_COMMENT
      case symbol_kind::S_IDENT: // IDENT
        yylhs.value.emplace< std::shared_ptr<std::string> > ();
        break;

      case symbol_kind::S_DEF_LIST: // DEF_LIST
        yylhs.value.emplace< std::vector<DefNode*>* > ();
        break;

      case symbol_kind::S_ARRAY_DIMESION_EXPR_LIST: // ARRAY_DIMESION_EXPR_LIST
        yylhs.value.emplace< std::vector<ExprNode*>* > ();
        break;

      case symbol_kind::S_INITIALIZER_LIST: // INITIALIZER_LIST
        yylhs.value.emplace< std::vector<InitNode*>* > ();
        break;

      case symbol_kind::S_STMT_LIST: // STMT_LIST
        yylhs.value.emplace< std::vector<StmtNode*>* > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // PROGRAM: STMT_LIST
#line 124 "parser/yacc.y"
              {
        yylhs.value.as < ASTree* > () = new ASTree(yystack_[0].value.as < std::vector<StmtNode*>* > ());
        driver.setAST(yylhs.value.as < ASTree* > ());
    }
#line 950 "parser/yacc.cpp"
    break;

  case 3: // PROGRAM: PROGRAM END
#line 128 "parser/yacc.y"
                  {
        YYACCEPT;
    }
#line 958 "parser/yacc.cpp"
    break;

  case 4: // STMT_LIST: STMT
#line 134 "parser/yacc.y"
         {
        yylhs.value.as < std::vector<StmtNode*>* > () = new std::vector<StmtNode*>();
        yylhs.value.as < std::vector<StmtNode*>* > ()->push_back(yystack_[0].value.as < StmtNode* > ());
    }
#line 967 "parser/yacc.cpp"
    break;

  case 5: // STMT_LIST: STMT_LIST STMT
#line 138 "parser/yacc.y"
                     {
        yylhs.value.as < std::vector<StmtNode*>* > () = yystack_[1].value.as < std::vector<StmtNode*>* > ();
        yylhs.value.as < std::vector<StmtNode*>* > ()->push_back(yystack_[0].value.as < StmtNode* > ());
    }
#line 976 "parser/yacc.cpp"
    break;

  case 6: // STMT: EXPR_STMT
#line 145 "parser/yacc.y"
              {
        yylhs.value.as < StmtNode* > () = yystack_[0].value.as < StmtNode* > ();
    }
#line 984 "parser/yacc.cpp"
    break;

  case 7: // STMT: VAR_DECL_STMT
#line 148 "parser/yacc.y"
                    {
        yylhs.value.as < StmtNode* > () = yystack_[0].value.as < StmtNode* > ();
    }
#line 992 "parser/yacc.cpp"
    break;

  case 8: // STMT: BLOCK_STMT
#line 151 "parser/yacc.y"
                 {
        yylhs.value.as < StmtNode* > () = yystack_[0].value.as < StmtNode* > ();
    }
#line 1000 "parser/yacc.cpp"
    break;

  case 9: // BLOCK_STMT: LBRACE STMT_LIST RBRACE
#line 157 "parser/yacc.y"
                            {
        yylhs.value.as < StmtNode* > () = new BlockStmt(yystack_[1].value.as < std::vector<StmtNode*>* > ());
    }
#line 1008 "parser/yacc.cpp"
    break;

  case 10: // EXPR_STMT: EXPR SEMICOLON
#line 163 "parser/yacc.y"
                   {
        yylhs.value.as < StmtNode* > () = new ExprStmt(yystack_[1].value.as < ExprNode* > ());
    }
#line 1016 "parser/yacc.cpp"
    break;

  case 11: // VAR_DECL_STMT: TYPE DEF_LIST SEMICOLON
#line 168 "parser/yacc.y"
                            {
        yylhs.value.as < StmtNode* > () = new VarDeclStmt(yystack_[2].value.as < Type* > (), yystack_[1].value.as < std::vector<DefNode*>* > ());
    }
#line 1024 "parser/yacc.cpp"
    break;

  case 12: // VAR_DECL_STMT: CONST TYPE DEF_LIST SEMICOLON
#line 171 "parser/yacc.y"
                                    {
        yylhs.value.as < StmtNode* > () = new VarDeclStmt(yystack_[2].value.as < Type* > (), yystack_[1].value.as < std::vector<DefNode*>* > (), true);
    }
#line 1032 "parser/yacc.cpp"
    break;

  case 13: // DEF: LEFT_VAL_EXPR
#line 176 "parser/yacc.y"
                  {
        yylhs.value.as < DefNode* > () = new DefNode(yystack_[0].value.as < ExprNode* > (), nullptr);
    }
#line 1040 "parser/yacc.cpp"
    break;

  case 14: // DEF: LEFT_VAL_EXPR ASSIGN INITIALIZER
#line 179 "parser/yacc.y"
                                       {
        yylhs.value.as < DefNode* > () = new DefNode(yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < InitNode* > ());
    }
#line 1048 "parser/yacc.cpp"
    break;

  case 15: // DEF: IDENT LBRACKET RBRACKET ASSIGN INITIALIZER
#line 182 "parser/yacc.y"
                                                 {
        std::vector<ExprNode*>* dim = new std::vector<ExprNode*>();
        dim->emplace_back(new ConstExpr(static_cast<InitMulti*>(yystack_[0].value.as < InitNode* > ())->getSize()));
        Symbol::Entry* entry = Symbol::Entry::getEntry(*yystack_[4].value.as < std::shared_ptr<std::string> > ());
        yylhs.value.as < DefNode* > () = new DefNode(new LeftValueExpr(entry, dim, -1), yystack_[0].value.as < InitNode* > ());
    }
#line 1059 "parser/yacc.cpp"
    break;

  case 16: // DEF_LIST: DEF
#line 191 "parser/yacc.y"
        {
        yylhs.value.as < std::vector<DefNode*>* > () = new std::vector<DefNode*>();
        yylhs.value.as < std::vector<DefNode*>* > ()->push_back(yystack_[0].value.as < DefNode* > ());
    }
#line 1068 "parser/yacc.cpp"
    break;

  case 17: // DEF_LIST: DEF_LIST COMMA DEF
#line 195 "parser/yacc.y"
                         {
        yylhs.value.as < std::vector<DefNode*>* > () = yystack_[2].value.as < std::vector<DefNode*>* > ();
        yylhs.value.as < std::vector<DefNode*>* > ()->push_back(yystack_[0].value.as < DefNode* > ());
    }
#line 1077 "parser/yacc.cpp"
    break;

  case 18: // INITIALIZER: EXPR
#line 202 "parser/yacc.y"
         {
        yylhs.value.as < InitNode* > () = new InitSingle(yystack_[0].value.as < ExprNode* > ());
    }
#line 1085 "parser/yacc.cpp"
    break;

  case 19: // INITIALIZER: LBRACE INITIALIZER_LIST RBRACE
#line 205 "parser/yacc.y"
                                     {
        yylhs.value.as < InitNode* > () = new InitMulti(yystack_[1].value.as < std::vector<InitNode*>* > ());
    }
#line 1093 "parser/yacc.cpp"
    break;

  case 20: // INITIALIZER: LBRACE RBRACE
#line 208 "parser/yacc.y"
                    {
        yylhs.value.as < InitNode* > () = new InitMulti(nullptr);
    }
#line 1101 "parser/yacc.cpp"
    break;

  case 21: // INITIALIZER_LIST: INITIALIZER
#line 214 "parser/yacc.y"
                { 
        yylhs.value.as < std::vector<InitNode*>* > () = new std::vector<InitNode*>();
        yylhs.value.as < std::vector<InitNode*>* > ()->push_back(yystack_[0].value.as < InitNode* > ());
    }
#line 1110 "parser/yacc.cpp"
    break;

  case 22: // INITIALIZER_LIST: INITIALIZER_LIST COMMA INITIALIZER
#line 218 "parser/yacc.y"
                                         {
        yylhs.value.as < std::vector<InitNode*>* > () = yystack_[2].value.as < std::vector<InitNode*>* > ();
        yylhs.value.as < std::vector<InitNode*>* > ()->push_back(yystack_[0].value.as < InitNode* > ());
    }
#line 1119 "parser/yacc.cpp"
    break;

  case 23: // ASSIGN_EXPR: LEFT_VAL_EXPR ASSIGN EXPR
#line 225 "parser/yacc.y"
                              {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Assign, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1127 "parser/yacc.cpp"
    break;

  case 24: // EXPR: LOGICAL_OR_EXPR
#line 231 "parser/yacc.y"
                    {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1135 "parser/yacc.cpp"
    break;

  case 25: // EXPR: ASSIGN_EXPR
#line 234 "parser/yacc.y"
                  {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1143 "parser/yacc.cpp"
    break;

  case 26: // LOGICAL_OR_EXPR: LOGICAL_AND_EXPR
#line 240 "parser/yacc.y"
                     {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1151 "parser/yacc.cpp"
    break;

  case 27: // LOGICAL_OR_EXPR: LOGICAL_OR_EXPR OR LOGICAL_AND_EXPR
#line 243 "parser/yacc.y"
                                          {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Or, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1159 "parser/yacc.cpp"
    break;

  case 28: // LOGICAL_AND_EXPR: EQUALITY_EXPR
#line 249 "parser/yacc.y"
                  {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1167 "parser/yacc.cpp"
    break;

  case 29: // LOGICAL_AND_EXPR: LOGICAL_AND_EXPR AND EQUALITY_EXPR
#line 252 "parser/yacc.y"
                                         {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::And, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1175 "parser/yacc.cpp"
    break;

  case 30: // EQUALITY_EXPR: RELATIONAL_EXPR
#line 258 "parser/yacc.y"
                    {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1183 "parser/yacc.cpp"
    break;

  case 31: // EQUALITY_EXPR: EQUALITY_EXPR EQ RELATIONAL_EXPR
#line 261 "parser/yacc.y"
                                       {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Eq, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1191 "parser/yacc.cpp"
    break;

  case 32: // EQUALITY_EXPR: EQUALITY_EXPR NEQ RELATIONAL_EXPR
#line 264 "parser/yacc.y"
                                        {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Neq, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1199 "parser/yacc.cpp"
    break;

  case 33: // RELATIONAL_EXPR: ADDSUB_EXPR
#line 270 "parser/yacc.y"
                {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1207 "parser/yacc.cpp"
    break;

  case 34: // RELATIONAL_EXPR: RELATIONAL_EXPR GT ADDSUB_EXPR
#line 273 "parser/yacc.y"
                                     {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Gt, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1215 "parser/yacc.cpp"
    break;

  case 35: // RELATIONAL_EXPR: RELATIONAL_EXPR GE ADDSUB_EXPR
#line 276 "parser/yacc.y"
                                     {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Ge, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1223 "parser/yacc.cpp"
    break;

  case 36: // RELATIONAL_EXPR: RELATIONAL_EXPR LT ADDSUB_EXPR
#line 279 "parser/yacc.y"
                                     {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Lt, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1231 "parser/yacc.cpp"
    break;

  case 37: // RELATIONAL_EXPR: RELATIONAL_EXPR LE ADDSUB_EXPR
#line 282 "parser/yacc.y"
                                     {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Le, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1239 "parser/yacc.cpp"
    break;

  case 38: // ADDSUB_EXPR: MULDIV_EXPR
#line 288 "parser/yacc.y"
                {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1247 "parser/yacc.cpp"
    break;

  case 39: // ADDSUB_EXPR: ADDSUB_EXPR PLUS MULDIV_EXPR
#line 291 "parser/yacc.y"
                                   {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Add, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1255 "parser/yacc.cpp"
    break;

  case 40: // ADDSUB_EXPR: ADDSUB_EXPR MINUS MULDIV_EXPR
#line 294 "parser/yacc.y"
                                    {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Sub, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1263 "parser/yacc.cpp"
    break;

  case 41: // MULDIV_EXPR: UNARY_EXPR
#line 300 "parser/yacc.y"
               {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1271 "parser/yacc.cpp"
    break;

  case 42: // MULDIV_EXPR: MULDIV_EXPR STAR UNARY_EXPR
#line 303 "parser/yacc.y"
                                  {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Mul, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1279 "parser/yacc.cpp"
    break;

  case 43: // MULDIV_EXPR: MULDIV_EXPR SLASH UNARY_EXPR
#line 306 "parser/yacc.y"
                                   {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Div, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1287 "parser/yacc.cpp"
    break;

  case 44: // MULDIV_EXPR: MULDIV_EXPR MOD UNARY_EXPR
#line 309 "parser/yacc.y"
                                 {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Mod, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1295 "parser/yacc.cpp"
    break;

  case 45: // UNARY_EXPR: BASIC_EXPR
#line 315 "parser/yacc.y"
               {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1303 "parser/yacc.cpp"
    break;

  case 46: // UNARY_EXPR: UNARY_OP BASIC_EXPR
#line 318 "parser/yacc.y"
                          {
        yylhs.value.as < ExprNode* > () = new UnaryExpr(yystack_[1].value.as < OpCode > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1311 "parser/yacc.cpp"
    break;

  case 47: // BASIC_EXPR: CONST_EXPR
#line 324 "parser/yacc.y"
               {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1319 "parser/yacc.cpp"
    break;

  case 48: // BASIC_EXPR: LEFT_VAL_EXPR
#line 327 "parser/yacc.y"
                    {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1327 "parser/yacc.cpp"
    break;

  case 49: // BASIC_EXPR: LPAREN EXPR RPAREN
#line 330 "parser/yacc.y"
                         {
        yylhs.value.as < ExprNode* > () = yystack_[1].value.as < ExprNode* > ();
    }
#line 1335 "parser/yacc.cpp"
    break;

  case 50: // ARRAY_DIMESION_EXPR: LBRACKET EXPR RBRACKET
#line 336 "parser/yacc.y"
                           {
        yylhs.value.as < ExprNode* > () = yystack_[1].value.as < ExprNode* > ();
    }
#line 1343 "parser/yacc.cpp"
    break;

  case 51: // ARRAY_DIMESION_EXPR_LIST: ARRAY_DIMESION_EXPR
#line 342 "parser/yacc.y"
                        {
        yylhs.value.as < std::vector<ExprNode*>* > () = new std::vector<ExprNode*>();
        yylhs.value.as < std::vector<ExprNode*>* > ()->push_back(yystack_[0].value.as < ExprNode* > ());
    }
#line 1352 "parser/yacc.cpp"
    break;

  case 52: // ARRAY_DIMESION_EXPR_LIST: ARRAY_DIMESION_EXPR_LIST ARRAY_DIMESION_EXPR
#line 346 "parser/yacc.y"
                                                   {
        yylhs.value.as < std::vector<ExprNode*>* > () = yystack_[1].value.as < std::vector<ExprNode*>* > ();
        yylhs.value.as < std::vector<ExprNode*>* > ()->push_back(yystack_[0].value.as < ExprNode* > ());
    }
#line 1361 "parser/yacc.cpp"
    break;

  case 53: // LEFT_VAL_EXPR: IDENT
#line 353 "parser/yacc.y"
          {
        Symbol::Entry* entry = Symbol::Entry::getEntry(*yystack_[0].value.as < std::shared_ptr<std::string> > ());
        yylhs.value.as < ExprNode* > () = new LeftValueExpr(entry, nullptr, -1);
    }
#line 1370 "parser/yacc.cpp"
    break;

  case 54: // LEFT_VAL_EXPR: IDENT ARRAY_DIMESION_EXPR_LIST
#line 357 "parser/yacc.y"
                                     {
        Symbol::Entry* entry = Symbol::Entry::getEntry(*yystack_[1].value.as < std::shared_ptr<std::string> > ());
        yylhs.value.as < ExprNode* > () = new LeftValueExpr(entry, yystack_[0].value.as < std::vector<ExprNode*>* > (), -1);
    }
#line 1379 "parser/yacc.cpp"
    break;

  case 55: // CONST_EXPR: INT_CONST
#line 364 "parser/yacc.y"
              {
        yylhs.value.as < ExprNode* > () = new ConstExpr(yystack_[0].value.as < int > ());
    }
#line 1387 "parser/yacc.cpp"
    break;

  case 56: // CONST_EXPR: LL_CONST
#line 367 "parser/yacc.y"
               {
        yylhs.value.as < ExprNode* > () = new ConstExpr(yystack_[0].value.as < long long > ());
    }
#line 1395 "parser/yacc.cpp"
    break;

  case 57: // CONST_EXPR: FLOAT_CONST
#line 370 "parser/yacc.y"
                  {
        yylhs.value.as < ExprNode* > () = new ConstExpr(yystack_[0].value.as < float > ());
    }
#line 1403 "parser/yacc.cpp"
    break;

  case 58: // CONST_EXPR: STR_CONST
#line 373 "parser/yacc.y"
                {
        yylhs.value.as < ExprNode* > () = new ConstExpr(yystack_[0].value.as < std::shared_ptr<std::string> > ());
    }
#line 1411 "parser/yacc.cpp"
    break;

  case 59: // TYPE: INT
#line 379 "parser/yacc.y"
        {
        yylhs.value.as < Type* > () = intType;
    }
#line 1419 "parser/yacc.cpp"
    break;

  case 60: // TYPE: FLOAT
#line 382 "parser/yacc.y"
            {
        yylhs.value.as < Type* > () = floatType;
    }
#line 1427 "parser/yacc.cpp"
    break;

  case 61: // TYPE: VOID
#line 385 "parser/yacc.y"
           {
        yylhs.value.as < Type* > () = voidType;
    }
#line 1435 "parser/yacc.cpp"
    break;

  case 62: // UNARY_OP: PLUS
#line 391 "parser/yacc.y"
         {
        yylhs.value.as < OpCode > () = OpCode::Add;
    }
#line 1443 "parser/yacc.cpp"
    break;

  case 63: // UNARY_OP: MINUS
#line 394 "parser/yacc.y"
            {
        yylhs.value.as < OpCode > () = OpCode::Sub;
    }
#line 1451 "parser/yacc.cpp"
    break;

  case 64: // UNARY_OP: NOT
#line 397 "parser/yacc.y"
          {
        yylhs.value.as < OpCode > () = OpCode::Not;
    }
#line 1459 "parser/yacc.cpp"
    break;


#line 1463 "parser/yacc.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
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
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
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
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
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
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
   Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   Parser ::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
   Parser ::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  //  Parser ::context.
   Parser ::context::context (const  Parser & yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
   Parser ::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
   Parser ::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
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

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
   Parser ::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  Parser ::yypact_ninf_ = -48;

  const signed char  Parser ::yytable_ninf_ = -1;

  const short
   Parser ::yypact_[] =
  {
       3,   -48,   -48,   -48,   -48,    -3,   -48,   -48,   -48,    61,
     140,     3,   -48,   -48,   -48,     2,     3,   -48,   -48,   -48,
     -48,   -48,     7,   -16,     5,   -47,   106,   -21,   -19,   -48,
     -48,    -4,   -48,    74,    16,   140,   -48,    -3,    74,    66,
      43,   -48,   -48,   -48,   -48,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,    72,
     -48,    40,    62,   -48,   -48,    80,   -48,    53,   -48,   -48,
       5,   -47,   106,   106,   -21,   -21,   -21,   -21,   -19,   -19,
     -48,   -48,   -48,   -48,   100,   -48,    74,   109,   -48,   -48,
      67,   -48,    93,   -48,   -48,   109,   -48,   -48,    18,   -48,
     109,   -48,   -48
  };

  const signed char
   Parser ::yydefact_[] =
  {
       0,    55,    56,    57,    58,    53,    59,    60,    61,     0,
       0,     0,    64,    62,    63,     0,     2,     4,     8,     6,
       7,    25,     0,    24,    26,    28,    30,    33,    38,    41,
      45,    48,    47,     0,     0,     0,    51,    54,     0,     0,
       0,     1,     3,     5,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
      16,     0,    13,    46,    48,     0,    52,     0,    49,     9,
      27,    29,    31,    32,    34,    35,    36,    37,    39,    40,
      42,    43,    44,    23,     0,    11,     0,     0,    50,    12,
       0,    17,     0,    14,    18,     0,    20,    21,     0,    15,
       0,    19,    22
  };

  const signed char
   Parser ::yypgoto_[] =
  {
     -48,   -48,   108,   -11,   -48,   -48,   -48,    31,    83,   -36,
     -48,   -48,    -7,   -48,    77,    82,    21,    11,    54,    34,
      89,    92,   -48,     0,   -48,   125,   -48
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
       0,    15,    16,    17,    18,    19,    20,    60,    61,    93,
      98,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    36,    37,    64,    32,    33,    34
  };

  const signed char
   Parser ::yytable_[] =
  {
      31,    47,    41,    39,    48,    43,     1,     2,     3,     4,
      31,    31,     5,     6,     7,     8,    31,    53,    54,     1,
       2,     3,     4,    55,    56,     5,    35,     9,    65,    43,
      10,    57,    44,    62,    11,    31,    12,    45,    62,    42,
      31,    13,    14,    10,   100,    58,     1,     2,     3,     4,
     101,    83,     5,     6,     7,     8,    97,    46,    31,    99,
      74,    75,    76,    77,   102,    85,    86,     9,    72,    73,
      10,     6,     7,     8,    11,    69,    12,    65,    89,    86,
      94,    13,    14,    59,    31,    94,    62,    31,    94,    80,
      81,    82,    31,    94,    68,    31,     1,     2,     3,     4,
      31,    84,     5,     1,     2,     3,     4,    78,    79,     5,
      88,    87,     1,     2,     3,     4,    95,    91,     5,    40,
      10,    67,    70,    63,    92,    96,    12,    10,    71,    66,
      90,    13,    14,    12,    38,     0,    10,     0,    13,    14,
      92,     0,    12,     1,     2,     3,     4,    13,    14,     5,
      49,    50,    51,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,    12,     0,     0,     0,     0,    13,    14
  };

  const signed char
   Parser ::yycheck_[] =
  {
       0,    48,     0,    10,    51,    16,     3,     4,     5,     6,
      10,    11,     9,    10,    11,    12,    16,    38,    39,     3,
       4,     5,     6,    42,    43,     9,    29,    24,    35,    40,
      27,    50,    25,    33,    31,    35,    33,    53,    38,    37,
      40,    38,    39,    27,    26,    49,     3,     4,     5,     6,
      32,    58,     9,    10,    11,    12,    92,    52,    58,    95,
      49,    50,    51,    52,   100,    25,    26,    24,    47,    48,
      27,    10,    11,    12,    31,    32,    33,    84,    25,    26,
      87,    38,    39,     9,    84,    92,    86,    87,    95,    55,
      56,    57,    92,   100,    28,    95,     3,     4,     5,     6,
     100,    29,     9,     3,     4,     5,     6,    53,    54,     9,
      30,    49,     3,     4,     5,     6,    49,    86,     9,    11,
      27,    38,    45,    34,    31,    32,    33,    27,    46,    37,
      30,    38,    39,    33,     9,    -1,    27,    -1,    38,    39,
      31,    -1,    33,     3,     4,     5,     6,    38,    39,     9,
      44,    45,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    38,    39
  };

  const signed char
   Parser ::yystos_[] =
  {
       0,     3,     4,     5,     6,     9,    10,    11,    12,    24,
      27,    31,    33,    38,    39,    55,    56,    57,    58,    59,
      60,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    77,    78,    79,    80,    29,    75,    76,    79,    66,
      56,     0,    37,    57,    25,    53,    52,    48,    51,    44,
      45,    46,    47,    38,    39,    42,    43,    50,    49,     9,
      61,    62,    77,    74,    77,    66,    75,    62,    28,    32,
      68,    69,    70,    70,    71,    71,    71,    71,    72,    72,
      73,    73,    73,    66,    29,    25,    26,    49,    30,    25,
      30,    61,    31,    63,    66,    49,    32,    63,    64,    63,
      26,    32,    63
  };

  const signed char
   Parser ::yyr1_[] =
  {
       0,    54,    55,    55,    56,    56,    57,    57,    57,    58,
      59,    60,    60,    61,    61,    61,    62,    62,    63,    63,
      63,    64,    64,    65,    66,    66,    67,    67,    68,    68,
      69,    69,    69,    70,    70,    70,    70,    70,    71,    71,
      71,    72,    72,    72,    72,    73,    73,    74,    74,    74,
      75,    76,    76,    77,    77,    78,    78,    78,    78,    79,
      79,    79,    80,    80,    80
  };

  const signed char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     2,     1,     2,     1,     1,     1,     3,
       2,     3,     4,     1,     3,     5,     1,     3,     1,     3,
       2,     1,     3,     3,     1,     1,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     2,     1,     1,     3,
       3,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const  Parser ::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "INT_CONST",
  "LL_CONST", "FLOAT_CONST", "STR_CONST", "ERR_TOKEN", "SLASH_COMMENT",
  "IDENT", "INT", "FLOAT", "VOID", "IF", "ELSE", "FOR", "WHILE",
  "CONTINUE", "BREAK", "SWITCH", "CASE", "GOTO", "DO", "RETURN", "CONST",
  "SEMICOLON", "COMMA", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET",
  "LBRACE", "RBRACE", "NOT", "BITOR", "BITAND", "DOT", "END", "PLUS",
  "MINUS", "UPLUS", "UMINUS", "STAR", "SLASH", "GT", "GE", "LT", "LE",
  "EQ", "ASSIGN", "MOD", "NEQ", "AND", "OR", "$accept", "PROGRAM",
  "STMT_LIST", "STMT", "BLOCK_STMT", "EXPR_STMT", "VAR_DECL_STMT", "DEF",
  "DEF_LIST", "INITIALIZER", "INITIALIZER_LIST", "ASSIGN_EXPR", "EXPR",
  "LOGICAL_OR_EXPR", "LOGICAL_AND_EXPR", "EQUALITY_EXPR",
  "RELATIONAL_EXPR", "ADDSUB_EXPR", "MULDIV_EXPR", "UNARY_EXPR",
  "BASIC_EXPR", "ARRAY_DIMESION_EXPR", "ARRAY_DIMESION_EXPR_LIST",
  "LEFT_VAL_EXPR", "CONST_EXPR", "TYPE", "UNARY_OP", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
   Parser ::yyrline_[] =
  {
       0,   124,   124,   128,   134,   138,   145,   148,   151,   157,
     163,   168,   171,   176,   179,   182,   191,   195,   202,   205,
     208,   214,   218,   225,   231,   234,   240,   243,   249,   252,
     258,   261,   264,   270,   273,   276,   279,   282,   288,   291,
     294,   300,   303,   306,   309,   315,   318,   324,   327,   330,
     336,   342,   346,   353,   357,   364,   367,   370,   373,   379,
     382,   385,   391,   394,   397
  };

  void
   Parser ::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
   Parser ::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 4 "parser/yacc.y"
} //  Yacc 
#line 2021 "parser/yacc.cpp"

#line 402 "parser/yacc.y"


void Yacc::Parser::error(const Yacc::location& location, const std::string& message)
{
    std::cerr << "msg: " << message << ", error happened at: " << location << std::endl;
}
