#include <parser/driver.h>
#include <bits/stdc++.h>

using namespace Yacc;
using namespace std;

using type = Parser::symbol_type;
using kind = Parser::symbol_kind;

Driver::Driver(std::istream* is, std::ostream* os)
    : _scanner(*this), _parser(_scanner, *this), inStream(is), outStream(os)
{}
Driver::~Driver() {}

void Driver::setStreams(std::istream* is, std::ostream* os)
{
    inStream  = is;
    outStream = os;
}

int Driver::lexical_parse()
{
    _scanner.switch_streams(*inStream, *outStream);

    while (true)
    {
        type token = _scanner.nextToken();
        if (token.kind() == kind::S_END) { break; }

        Token result;
        result.token_name = token.name();
        result.line       = token.location.begin.line;
        result.column     = token.location.begin.column - 1;
        result.lexeme     = _scanner.YYText();

        switch (token.kind())
        {
            case kind::S_INT_CONST: result.value = token.value.as<int>(); break;
            case kind::S_LL_CONST: result.value = token.value.as<long long>(); break;
            case kind::S_FLOAT_CONST: result.value = token.value.as<float>(); break;
            case kind::S_IDENT:
            case kind::S_SLASH_COMMENT:
            case kind::S_ERR_TOKEN:
            case kind::S_STR_CONST: result.value = token.value.as<std::shared_ptr<std::string>>(); break;
            default: break;
        }

        tokens.push_back(result);
    }

    return 0;
}

ASTree* Driver::parse()
{
    _scanner.switch_streams(*inStream, *outStream);
    _parser.parse();
    return ast;
}