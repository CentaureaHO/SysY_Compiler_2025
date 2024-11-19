#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <parser/driver.h>
#include <common/type/symtab/symbol_table.h>
#include <common/type/symtab/semantic_table.h>

#define STR_PW 30
#define INT_PW 8
#define MIN_GAP 5
#define STR_REAL_WIDTH (STR_PW - MIN_GAP)

using namespace std;
using namespace Yacc;
using namespace Symbol;

extern vector<string>       semanticErrMsgs;
extern SemanticTable::Table semTable;
size_t                      errCnt = 0;

string truncateString(const string& str, size_t width)
{
    if (str.length() > width) return str.substr(0, width - 3) + "...";
    return str;
}

#define file_in 1
#define step_tag 2
#define o_tag 3
#define file_out 4
#define optimize_tag 5

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <input file> <step> <output file> <optimize>" << std::endl;
        return 0;
    }

    cout << "File_in: " << argv[file_in] << endl;
    cout << "Step: " << argv[step_tag] << endl;
    cout << "Output: " << argv[file_out] << endl;
    cout << "Optimize: " << argv[optimize_tag] << endl;

    ifstream in(argv[file_in]);
    if (!in)
    {
        std::cerr << "Cannot open input file " << argv[file_in] << std::endl;
        return 1;
    }
    istream* inStream = &in;

    ofstream out(argv[file_out]);
    if (!out)
    {
        std::cerr << "Cannot open output file " << argv[file_out] << std::endl;
        if (in.is_open()) in.close();
        return 1;
    }
    ostream* outStream = &out;

    Driver driver(inStream, outStream);

    if (strcmp(argv[step_tag], "-lexer") == 0)
    {
        driver.lexical_parse();
        auto tokens = driver.getTokens();

        *outStream << left;
        *outStream << setw(STR_PW) << "Token" << setw(STR_PW) << "Lexeme" << setw(STR_PW) << "Property" << setw(INT_PW)
                   << "Line" << setw(INT_PW) << "Column" << endl;

        for (auto& token : tokens)
        {
            *outStream << setw(STR_PW) << truncateString(token.token_name, STR_REAL_WIDTH) << setw(STR_PW)
                       << truncateString(token.lexeme, STR_REAL_WIDTH);

            if (token.token_name == "INT_CONST" || token.token_name == "LL_CONST" || token.token_name == "STR_CONST" ||
                token.token_name == "FLOAT_CONST" || token.token_name == "IDENT" ||
                token.token_name == "SLASH_COMMENT" || token.token_name == "ERR_TOKEN")
            {
                std::visit(
                    [&](auto&& arg) {
                        using T = std::decay_t<decltype(arg)>;
                        if constexpr (std::is_same_v<T, int>) { *outStream << setw(STR_PW) << arg; }
                        else if constexpr (std::is_same_v<T, long long>) { *outStream << setw(STR_PW) << arg; }
                        else if constexpr (std::is_same_v<T, float>) { *outStream << setw(STR_PW) << arg; }
                        else if constexpr (std::is_same_v<T, std::shared_ptr<std::string>>)
                        {
                            *outStream << setw(STR_PW) << truncateString(*arg, STR_REAL_WIDTH);
                        }
                        else { *outStream << setw(STR_PW) << "N/A"; }
                    },
                    token.value);
            }
            else { *outStream << setw(STR_PW) << " "; }

            *outStream << setw(INT_PW) << token.line << setw(INT_PW) << token.column << endl;
        }

        if (in.is_open()) in.close();
        if (out.is_open()) out.close();

        return 0;
    }

    ASTree* ast = driver.parse();
    if (errCnt)
    {
        *outStream << errCnt << " errors found during parsing, exiting..." << endl;
        if (in.is_open()) in.close();
        if (out.is_open()) out.close();

        return 2;
    }

    if (strcmp(argv[step_tag], "-parser") == 0)
    {
        if (ast) { ast->printAST(outStream); }
        if (in.is_open()) in.close();
        if (out.is_open()) out.close();
        return 0;
    }

    ast->typeCheck();
    if (semanticErrMsgs.size() > 0)
    {
        cout << "\nSemantic errors found: " << endl;

        int idx = 0;
        for (auto& msg : semanticErrMsgs) cerr << ++idx << ". " << msg << endl;

        cout << "\n\nExiting..." << endl;

        if (in.is_open()) in.close();
        if (out.is_open()) out.close();

        return 3;
    }

    // ast->genIRCode();

    if (in.is_open()) in.close();
    if (out.is_open()) out.close();
    return 0;
}