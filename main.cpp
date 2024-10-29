#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <parser/driver.h>
#include <common/type/symtab/symbol_table.h>

#define STR_PW 30
#define INT_PW 8
#define MIN_GAP 5
#define STR_REAL_WIDTH (STR_PW - MIN_GAP)

using namespace std;
using namespace Yacc;
using namespace Symbol;

string truncateString(const string& str, size_t width)
{
    if (str.length() > width) return str.substr(0, width - 3) + "...";
    return str;
}

string getOutputFileName(const string& inputFile, const string& mode)
{
    string outputFile = inputFile;
    size_t pos        = outputFile.find(".sy");
    if (pos != string::npos) { outputFile = outputFile.substr(0, pos); }
    outputFile += (mode == "lexer") ? ".token" : ".ast";
    return outputFile;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <lexer|parser> <input_file>" << endl;
        return 1;
    }

    string mode = argv[1];
    if (mode != "lexer" && mode != "parser")
    {
        cerr << "Error: The first argument must be 'lexer' or 'parser'." << endl;
        return 1;
    }

    string   inputFile = argv[2];
    ifstream file(inputFile);
    if (!file)
    {
        cerr << "Error: Cannot open input file " << inputFile << endl;
        return 1;
    }
    istream* inStream = &file;

    string   outputFile = getOutputFileName(inputFile, mode);
    ofstream outFile(outputFile);
    if (!outFile)
    {
        cerr << "Error: Cannot open output file " << outputFile << endl;
        return 1;
    }
    ostream* outStream = &outFile;

    Driver driver(inStream, outStream);

    if (mode == "lexer")
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
    }
    else if (mode == "parser")
    {
        ASTree* ast = driver.parse();
        if (ast) { ast->printAST(outStream); }
    }

    if (file.is_open()) file.close();
    if (outFile.is_open()) outFile.close();

    return 0;
}