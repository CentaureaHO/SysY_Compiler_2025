#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include <parser/scanner.h>
#include <parser/yacc.hpp>

namespace Yacc
{
    class Driver
    {
      public:
        struct Token
        {
            std::string                                            token_name;  // name
            std::string                                            lexeme;      // lexeme
            std::variant<int, float, std::shared_ptr<std::string>> value;       // property
            int                                                    line;        // line
            int                                                    column;      // column
        };

      private:
        Scanner                  _scanner;
        Parser                   _parser;
        std::vector<Token> tokens;

        std::istream* inStream;
        std::ostream* outStream;

      public:
        Driver(std::istream* is = &std::cin, std::ostream* os = &std::cout);

        int lexical_parse();
        int parse();

        std::vector<Token> getTokens() const { return tokens; }

        void reportError(const location& loc, const std::string& message) { _parser.error(loc, message); }

        void setStreams(std::istream* is, std::ostream* os);

        virtual ~Driver();
    };
}  // namespace Yacc

#endif
