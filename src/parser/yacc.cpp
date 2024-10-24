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
  case 2: // PROGRAM: STMT
#line 119 "parser/yacc.y"
         {
        std::cout << "program: STMT " << std::endl;
        yystack_[0].value.as < StmtNode* > ()->printAST(&std::cout, 0);
        yylhs.value.as < ASTree* > () = new ASTree();
        driver.setAST(yylhs.value.as < ASTree* > ());
        // delete $1;
    }
#line 928 "parser/yacc.cpp"
    break;

  case 3: // PROGRAM: PROGRAM END
#line 126 "parser/yacc.y"
                  {
        YYACCEPT;
    }
#line 936 "parser/yacc.cpp"
    break;

  case 4: // STMT: EXPR_STMT
#line 132 "parser/yacc.y"
              {
        yylhs.value.as < StmtNode* > () = yystack_[0].value.as < StmtNode* > ();
    }
#line 944 "parser/yacc.cpp"
    break;

  case 5: // STMT: VAR_DECL_STMT
#line 135 "parser/yacc.y"
                    {
        yylhs.value.as < StmtNode* > () = yystack_[0].value.as < StmtNode* > ();
    }
#line 952 "parser/yacc.cpp"
    break;

  case 6: // EXPR_STMT: EXPR SEMICOLON
#line 141 "parser/yacc.y"
                   {
        yylhs.value.as < StmtNode* > () = new ExprStmt(yystack_[1].value.as < ExprNode* > ());
    }
#line 960 "parser/yacc.cpp"
    break;

  case 7: // VAR_DECL_STMT: TYPE DEF_LIST SEMICOLON
#line 146 "parser/yacc.y"
                            {
        yylhs.value.as < StmtNode* > () = new VarDeclStmt(yystack_[2].value.as < Type* > (), yystack_[1].value.as < std::vector<DefNode*>* > ());
    }
#line 968 "parser/yacc.cpp"
    break;

  case 8: // VAR_DECL_STMT: CONST TYPE DEF_LIST SEMICOLON
#line 149 "parser/yacc.y"
                                    {
        yylhs.value.as < StmtNode* > () = new VarDeclStmt(yystack_[2].value.as < Type* > (), yystack_[1].value.as < std::vector<DefNode*>* > (), true);
    }
#line 976 "parser/yacc.cpp"
    break;

  case 9: // DEF: LEFT_VAL_EXPR
#line 154 "parser/yacc.y"
                  {
        yylhs.value.as < DefNode* > () = new DefNode(yystack_[0].value.as < ExprNode* > (), nullptr);
    }
#line 984 "parser/yacc.cpp"
    break;

  case 10: // DEF: LEFT_VAL_EXPR ASSIGN INITIALIZER
#line 157 "parser/yacc.y"
                                       {
        yylhs.value.as < DefNode* > () = new DefNode(yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < InitNode* > ());
    }
#line 992 "parser/yacc.cpp"
    break;

  case 11: // DEF_LIST: DEF
#line 162 "parser/yacc.y"
        {
        yylhs.value.as < std::vector<DefNode*>* > () = new std::vector<DefNode*>();
        yylhs.value.as < std::vector<DefNode*>* > ()->push_back(yystack_[0].value.as < DefNode* > ());
    }
#line 1001 "parser/yacc.cpp"
    break;

  case 12: // DEF_LIST: DEF_LIST COMMA DEF
#line 166 "parser/yacc.y"
                         {
        yylhs.value.as < std::vector<DefNode*>* > () = yystack_[2].value.as < std::vector<DefNode*>* > ();
        yylhs.value.as < std::vector<DefNode*>* > ()->push_back(yystack_[0].value.as < DefNode* > ());
    }
#line 1010 "parser/yacc.cpp"
    break;

  case 13: // INITIALIZER: EXPR
#line 173 "parser/yacc.y"
         {
        yylhs.value.as < InitNode* > () = new InitSingle(yystack_[0].value.as < ExprNode* > ());
    }
#line 1018 "parser/yacc.cpp"
    break;

  case 14: // INITIALIZER: LBRACE INITIALIZER_LIST RBRACE
#line 176 "parser/yacc.y"
                                     {
        yylhs.value.as < InitNode* > () = new InitMulti(yystack_[1].value.as < std::vector<InitNode*>* > ());
    }
#line 1026 "parser/yacc.cpp"
    break;

  case 15: // INITIALIZER_LIST: INITIALIZER
#line 182 "parser/yacc.y"
                { 
        yylhs.value.as < std::vector<InitNode*>* > () = new std::vector<InitNode*>();
        yylhs.value.as < std::vector<InitNode*>* > ()->push_back(yystack_[0].value.as < InitNode* > ());
    }
#line 1035 "parser/yacc.cpp"
    break;

  case 16: // INITIALIZER_LIST: INITIALIZER_LIST COMMA INITIALIZER
#line 186 "parser/yacc.y"
                                         {
        yylhs.value.as < std::vector<InitNode*>* > () = yystack_[2].value.as < std::vector<InitNode*>* > ();
        yylhs.value.as < std::vector<InitNode*>* > ()->push_back(yystack_[0].value.as < InitNode* > ());
    }
#line 1044 "parser/yacc.cpp"
    break;

  case 17: // ASSIGN_EXPR: LEFT_VAL_EXPR ASSIGN EXPR
#line 193 "parser/yacc.y"
                              {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Assign, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1052 "parser/yacc.cpp"
    break;

  case 18: // EXPR: LOGICAL_OR_EXPR
#line 199 "parser/yacc.y"
                    {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1060 "parser/yacc.cpp"
    break;

  case 19: // EXPR: ASSIGN_EXPR
#line 202 "parser/yacc.y"
                  {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1068 "parser/yacc.cpp"
    break;

  case 20: // LOGICAL_OR_EXPR: LOGICAL_AND_EXPR
#line 208 "parser/yacc.y"
                     {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1076 "parser/yacc.cpp"
    break;

  case 21: // LOGICAL_OR_EXPR: LOGICAL_OR_EXPR OR LOGICAL_AND_EXPR
#line 211 "parser/yacc.y"
                                          {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Or, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1084 "parser/yacc.cpp"
    break;

  case 22: // LOGICAL_AND_EXPR: EQUALITY_EXPR
#line 217 "parser/yacc.y"
                  {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1092 "parser/yacc.cpp"
    break;

  case 23: // LOGICAL_AND_EXPR: LOGICAL_AND_EXPR AND EQUALITY_EXPR
#line 220 "parser/yacc.y"
                                         {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::And, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1100 "parser/yacc.cpp"
    break;

  case 24: // EQUALITY_EXPR: RELATIONAL_EXPR
#line 226 "parser/yacc.y"
                    {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1108 "parser/yacc.cpp"
    break;

  case 25: // EQUALITY_EXPR: EQUALITY_EXPR EQ RELATIONAL_EXPR
#line 229 "parser/yacc.y"
                                       {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Eq, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1116 "parser/yacc.cpp"
    break;

  case 26: // EQUALITY_EXPR: EQUALITY_EXPR NEQ RELATIONAL_EXPR
#line 232 "parser/yacc.y"
                                        {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Neq, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1124 "parser/yacc.cpp"
    break;

  case 27: // RELATIONAL_EXPR: ADDSUB_EXPR
#line 238 "parser/yacc.y"
                {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1132 "parser/yacc.cpp"
    break;

  case 28: // RELATIONAL_EXPR: RELATIONAL_EXPR GT ADDSUB_EXPR
#line 241 "parser/yacc.y"
                                     {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Gt, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1140 "parser/yacc.cpp"
    break;

  case 29: // RELATIONAL_EXPR: RELATIONAL_EXPR GE ADDSUB_EXPR
#line 244 "parser/yacc.y"
                                     {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Ge, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1148 "parser/yacc.cpp"
    break;

  case 30: // RELATIONAL_EXPR: RELATIONAL_EXPR LT ADDSUB_EXPR
#line 247 "parser/yacc.y"
                                     {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Lt, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1156 "parser/yacc.cpp"
    break;

  case 31: // RELATIONAL_EXPR: RELATIONAL_EXPR LE ADDSUB_EXPR
#line 250 "parser/yacc.y"
                                     {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Le, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1164 "parser/yacc.cpp"
    break;

  case 32: // ADDSUB_EXPR: MULDIV_EXPR
#line 256 "parser/yacc.y"
                {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1172 "parser/yacc.cpp"
    break;

  case 33: // ADDSUB_EXPR: ADDSUB_EXPR PLUS MULDIV_EXPR
#line 259 "parser/yacc.y"
                                   {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Add, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1180 "parser/yacc.cpp"
    break;

  case 34: // ADDSUB_EXPR: ADDSUB_EXPR MINUS MULDIV_EXPR
#line 262 "parser/yacc.y"
                                    {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Sub, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1188 "parser/yacc.cpp"
    break;

  case 35: // MULDIV_EXPR: UNARY_EXPR
#line 268 "parser/yacc.y"
               {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1196 "parser/yacc.cpp"
    break;

  case 36: // MULDIV_EXPR: MULDIV_EXPR STAR UNARY_EXPR
#line 271 "parser/yacc.y"
                                  {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Mul, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1204 "parser/yacc.cpp"
    break;

  case 37: // MULDIV_EXPR: MULDIV_EXPR SLASH UNARY_EXPR
#line 274 "parser/yacc.y"
                                   {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Div, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1212 "parser/yacc.cpp"
    break;

  case 38: // MULDIV_EXPR: MULDIV_EXPR MOD UNARY_EXPR
#line 277 "parser/yacc.y"
                                 {
        yylhs.value.as < ExprNode* > () = new BinaryExpr(OpCode::Mod, yystack_[2].value.as < ExprNode* > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1220 "parser/yacc.cpp"
    break;

  case 39: // UNARY_EXPR: BASIC_EXPR
#line 283 "parser/yacc.y"
               {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1228 "parser/yacc.cpp"
    break;

  case 40: // UNARY_EXPR: UNARY_OP BASIC_EXPR
#line 286 "parser/yacc.y"
                          {
        yylhs.value.as < ExprNode* > () = new UnaryExpr(yystack_[1].value.as < OpCode > (), yystack_[0].value.as < ExprNode* > ());
    }
#line 1236 "parser/yacc.cpp"
    break;

  case 41: // BASIC_EXPR: CONST_EXPR
#line 292 "parser/yacc.y"
               {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1244 "parser/yacc.cpp"
    break;

  case 42: // BASIC_EXPR: LEFT_VAL_EXPR
#line 295 "parser/yacc.y"
                    {
        yylhs.value.as < ExprNode* > () = yystack_[0].value.as < ExprNode* > ();
    }
#line 1252 "parser/yacc.cpp"
    break;

  case 43: // BASIC_EXPR: LPAREN EXPR RPAREN
#line 298 "parser/yacc.y"
                         {
        yylhs.value.as < ExprNode* > () = yystack_[1].value.as < ExprNode* > ();
    }
#line 1260 "parser/yacc.cpp"
    break;

  case 44: // ARRAY_DIMESION_EXPR: LBRACKET EXPR RBRACKET
#line 304 "parser/yacc.y"
                           {
        yylhs.value.as < ExprNode* > () = yystack_[1].value.as < ExprNode* > ();
    }
#line 1268 "parser/yacc.cpp"
    break;

  case 45: // ARRAY_DIMESION_EXPR_LIST: ARRAY_DIMESION_EXPR
#line 310 "parser/yacc.y"
                        {
        yylhs.value.as < std::vector<ExprNode*>* > () = new std::vector<ExprNode*>();
        yylhs.value.as < std::vector<ExprNode*>* > ()->push_back(yystack_[0].value.as < ExprNode* > ());
    }
#line 1277 "parser/yacc.cpp"
    break;

  case 46: // ARRAY_DIMESION_EXPR_LIST: ARRAY_DIMESION_EXPR_LIST ARRAY_DIMESION_EXPR
#line 314 "parser/yacc.y"
                                                   {
        yylhs.value.as < std::vector<ExprNode*>* > () = yystack_[1].value.as < std::vector<ExprNode*>* > ();
        yylhs.value.as < std::vector<ExprNode*>* > ()->push_back(yystack_[0].value.as < ExprNode* > ());
    }
#line 1286 "parser/yacc.cpp"
    break;

  case 47: // LEFT_VAL_EXPR: IDENT
#line 321 "parser/yacc.y"
          {
        Symbol::Entry* entry = Symbol::Entry::getEntry(*yystack_[0].value.as < std::shared_ptr<std::string> > ());
        yylhs.value.as < ExprNode* > () = new LeftValueExpr(entry, nullptr, -1);
    }
#line 1295 "parser/yacc.cpp"
    break;

  case 48: // LEFT_VAL_EXPR: IDENT ARRAY_DIMESION_EXPR_LIST
#line 325 "parser/yacc.y"
                                     {
        Symbol::Entry* entry = Symbol::Entry::getEntry(*yystack_[1].value.as < std::shared_ptr<std::string> > ());
        yylhs.value.as < ExprNode* > () = new LeftValueExpr(entry, yystack_[0].value.as < std::vector<ExprNode*>* > (), -1);
    }
#line 1304 "parser/yacc.cpp"
    break;

  case 49: // CONST_EXPR: INT_CONST
#line 332 "parser/yacc.y"
              {
        yylhs.value.as < ExprNode* > () = new ConstExpr(yystack_[0].value.as < int > ());
    }
#line 1312 "parser/yacc.cpp"
    break;

  case 50: // CONST_EXPR: LL_CONST
#line 335 "parser/yacc.y"
               {
        yylhs.value.as < ExprNode* > () = new ConstExpr(yystack_[0].value.as < long long > ());
    }
#line 1320 "parser/yacc.cpp"
    break;

  case 51: // CONST_EXPR: FLOAT_CONST
#line 338 "parser/yacc.y"
                  {
        yylhs.value.as < ExprNode* > () = new ConstExpr(yystack_[0].value.as < float > ());
    }
#line 1328 "parser/yacc.cpp"
    break;

  case 52: // CONST_EXPR: STR_CONST
#line 341 "parser/yacc.y"
                {
        yylhs.value.as < ExprNode* > () = new ConstExpr(yystack_[0].value.as < std::shared_ptr<std::string> > ());
    }
#line 1336 "parser/yacc.cpp"
    break;

  case 53: // TYPE: INT
#line 347 "parser/yacc.y"
        {
        yylhs.value.as < Type* > () = intType;
    }
#line 1344 "parser/yacc.cpp"
    break;

  case 54: // TYPE: FLOAT
#line 350 "parser/yacc.y"
            {
        yylhs.value.as < Type* > () = floatType;
    }
#line 1352 "parser/yacc.cpp"
    break;

  case 55: // TYPE: VOID
#line 353 "parser/yacc.y"
           {
        yylhs.value.as < Type* > () = voidType;
    }
#line 1360 "parser/yacc.cpp"
    break;

  case 56: // UNARY_OP: PLUS
#line 359 "parser/yacc.y"
         {
        yylhs.value.as < OpCode > () = OpCode::Add;
    }
#line 1368 "parser/yacc.cpp"
    break;

  case 57: // UNARY_OP: MINUS
#line 362 "parser/yacc.y"
            {
        yylhs.value.as < OpCode > () = OpCode::Sub;
    }
#line 1376 "parser/yacc.cpp"
    break;

  case 58: // UNARY_OP: NOT
#line 365 "parser/yacc.y"
          {
        yylhs.value.as < OpCode > () = OpCode::Not;
    }
#line 1384 "parser/yacc.cpp"
    break;


#line 1388 "parser/yacc.cpp"

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


  const signed char  Parser ::yypact_ninf_ = -53;

  const signed char  Parser ::yytable_ninf_ = -1;

  const signed char
   Parser ::yypact_[] =
  {
      10,   -53,   -53,   -53,   -53,    11,   -53,   -53,   -53,    -4,
      60,   -53,   -53,   -53,     2,   -53,   -53,   -53,   -53,    17,
      -1,     3,   -10,    26,   -21,   -38,   -53,   -53,    32,   -53,
      73,    20,    60,   -53,    11,    73,    57,   -53,   -53,   -53,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,   -53,    25,    40,   -53,   -53,    64,
     -53,    35,   -53,     3,   -10,    26,    26,   -21,   -21,   -21,
     -21,   -38,   -38,   -53,   -53,   -53,   -53,   -53,    73,    53,
     -53,   -53,   -53,    53,   -53,   -53,   -53,   -23,    53,   -53,
     -53
  };

  const signed char
   Parser ::yydefact_[] =
  {
       0,    49,    50,    51,    52,    47,    53,    54,    55,     0,
       0,    58,    56,    57,     0,     2,     4,     5,    19,     0,
      18,    20,    22,    24,    27,    32,    35,    39,    42,    41,
       0,     0,     0,    45,    48,     0,     0,     1,     3,     6,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    11,     0,     9,    40,    42,     0,
      46,     0,    43,    21,    23,    25,    26,    28,    29,    30,
      31,    33,    34,    36,    37,    38,    17,     7,     0,     0,
      44,     8,    12,     0,    10,    13,    15,     0,     0,    14,
      16
  };

  const signed char
   Parser ::yypgoto_[] =
  {
     -53,   -53,   -53,   -53,   -53,    12,    61,   -52,   -53,   -53,
       1,   -53,    55,    56,   -15,    30,    19,    -6,    69,    67,
     -53,     0,   -53,    93,   -53
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
       0,    14,    15,    16,    17,    54,    55,    84,    87,    18,
      85,    20,    21,    22,    23,    24,    25,    26,    27,    33,
      34,    58,    29,    30,    31
  };

  const signed char
   Parser ::yytable_[] =
  {
      28,    19,    37,    88,    50,    51,     6,     7,     8,    89,
      28,    36,    52,     1,     2,     3,     4,    48,    49,     5,
       6,     7,     8,     1,     2,     3,     4,    65,    66,     5,
      56,    86,    28,    59,     9,    56,    90,    10,    42,    38,
      32,    43,    39,    11,    73,    74,    75,    10,    12,    13,
      77,    78,    40,    28,    76,    41,     1,     2,     3,     4,
      81,    78,     5,     1,     2,     3,     4,    71,    72,     5,
      44,    45,    46,    47,    67,    68,    69,    70,    56,    28,
      10,    53,     5,    28,    83,    62,    11,    10,    28,    79,
      82,    12,    13,    11,    80,    63,    61,    64,    12,    13,
      57,    60,    35
  };

  const signed char
   Parser ::yycheck_[] =
  {
       0,     0,     0,    26,    42,    43,    10,    11,    12,    32,
      10,    10,    50,     3,     4,     5,     6,    38,    39,     9,
      10,    11,    12,     3,     4,     5,     6,    42,    43,     9,
      30,    83,    32,    32,    24,    35,    88,    27,    48,    37,
      29,    51,    25,    33,    50,    51,    52,    27,    38,    39,
      25,    26,    53,    53,    53,    52,     3,     4,     5,     6,
      25,    26,     9,     3,     4,     5,     6,    48,    49,     9,
      44,    45,    46,    47,    44,    45,    46,    47,    78,    79,
      27,    49,     9,    83,    31,    28,    33,    27,    88,    49,
      78,    38,    39,    33,    30,    40,    35,    41,    38,    39,
      31,    34,     9
  };

  const signed char
   Parser ::yystos_[] =
  {
       0,     3,     4,     5,     6,     9,    10,    11,    12,    24,
      27,    33,    38,    39,    55,    56,    57,    58,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    75,    76,
      77,    78,    29,    73,    74,    77,    64,     0,    37,    25,
      53,    52,    48,    51,    44,    45,    46,    47,    38,    39,
      42,    43,    50,    49,    59,    60,    75,    72,    75,    64,
      73,    60,    28,    66,    67,    68,    68,    69,    69,    69,
      69,    70,    70,    71,    71,    71,    64,    25,    26,    49,
      30,    25,    59,    31,    61,    64,    61,    62,    26,    32,
      61
  };

  const signed char
   Parser ::yyr1_[] =
  {
       0,    54,    55,    55,    56,    56,    57,    58,    58,    59,
      59,    60,    60,    61,    61,    62,    62,    63,    64,    64,
      65,    65,    66,    66,    67,    67,    67,    68,    68,    68,
      68,    68,    69,    69,    69,    70,    70,    70,    70,    71,
      71,    72,    72,    72,    73,    74,    74,    75,    75,    76,
      76,    76,    76,    77,    77,    77,    78,    78,    78
  };

  const signed char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     2,     1,     1,     2,     3,     4,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     1,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     1,     1,     3,     3,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
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
  "EQ", "ASSIGN", "MOD", "NEQ", "AND", "OR", "$accept", "PROGRAM", "STMT",
  "EXPR_STMT", "VAR_DECL_STMT", "DEF", "DEF_LIST", "INITIALIZER",
  "INITIALIZER_LIST", "ASSIGN_EXPR", "EXPR", "LOGICAL_OR_EXPR",
  "LOGICAL_AND_EXPR", "EQUALITY_EXPR", "RELATIONAL_EXPR", "ADDSUB_EXPR",
  "MULDIV_EXPR", "UNARY_EXPR", "BASIC_EXPR", "ARRAY_DIMESION_EXPR",
  "ARRAY_DIMESION_EXPR_LIST", "LEFT_VAL_EXPR", "CONST_EXPR", "TYPE",
  "UNARY_OP", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
   Parser ::yyrline_[] =
  {
       0,   119,   119,   126,   132,   135,   141,   146,   149,   154,
     157,   162,   166,   173,   176,   182,   186,   193,   199,   202,
     208,   211,   217,   220,   226,   229,   232,   238,   241,   244,
     247,   250,   256,   259,   262,   268,   271,   274,   277,   283,
     286,   292,   295,   298,   304,   310,   314,   321,   325,   332,
     335,   338,   341,   347,   350,   353,   359,   362,   365
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
#line 1926 "parser/yacc.cpp"

#line 370 "parser/yacc.y"


void Yacc::Parser::error(const Yacc::location& location, const std::string& message)
{
    std::cerr << "msg: " << message << ", error happened at: " << location << std::endl;
}
