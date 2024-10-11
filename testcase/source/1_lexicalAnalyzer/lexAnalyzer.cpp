#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>

#include <parser/driver.h>

#define STR_PW 30
#define INT_PW 8
#define MIN_GAP 5
#define STR_REAL_WIDTH (STR_PW - MIN_GAP)

using namespace Yacc;
using namespace std;

string truncateString(const string& str, size_t width)
{
    if (str.length() > width) return str.substr(0, width - 3) + "...";
    return str;
}

string getOutputFileName(const string& inputFile)
{
    string outputFile = inputFile;
    size_t pos        = outputFile.find(".sy");
    if (pos != string::npos) { outputFile = outputFile.substr(0, pos); }
    outputFile += ".token";
    return outputFile;
}

int main(int argc, char* argv[])
{
    istream* inStream  = &cin;
    ostream* outStream = &cout;

    ifstream file;
    ofstream outFile;

    if (argc > 1)
    {
        string inputFile = argv[1];
        file.open(inputFile);

        if (!file)
        {
            cerr << "Error: Cannot open input file " << inputFile << endl;
            return 1;
        }

        inStream = &file;

        string outputFile = getOutputFileName(inputFile);
        outFile.open(outputFile);

        if (!outFile)
        {
            cerr << "Error: Cannot open output file " << outputFile << endl;
            return 1;
        }

        outStream = &outFile;
    }

    Driver driver(inStream, outStream);
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
            token.token_name == "FLOAT_CONST" || token.token_name == "IDENT" || token.token_name == "SLASH_COMMENT" ||
            token.token_name == "ERR_TOKEN")
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

    if (file.is_open()) file.close();
    if (outFile.is_open()) outFile.close();

    return 0;
}