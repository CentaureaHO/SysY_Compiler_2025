#ifndef __PARSER_SCANNER_H__
#define __PARSER_SCANNER_H__

#ifndef yyFlexLexerOnce
#undef yyFlexLexer
#define yyFlexLexer Yacc_FlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL Yacc::Parser::symbol_type Yacc::Scanner::nextToken()

#include <parser/yacc.hpp>

namespace Yacc
{
    class Driver;

    class Scanner : public yyFlexLexer
    {
      private:
        Driver& _driver;

      public:
        Scanner(Yacc::Driver& driver) : _driver(driver) {}
        virtual ~Scanner() {}

        virtual Yacc::Parser::symbol_type nextToken();
    };
}  // namespace Yacc

#endif