#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
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
    string mode = "parser";
    // cin >> mode;

    string inputFile = "test.in";
    // cin >> inputFile;
    ifstream file(inputFile);
    if (!file)
    {
        cerr << "Error: Cannot open input file " << inputFile << endl;
        return 1;
    }
    istream* inStream = &file;

    string outputFile = getOutputFileName(inputFile, mode);
    outputFile        = "test.out";
    // cin >> outputFile;
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

        // 以下为调试暂用代码
        ast->typeCheck();
        for (auto& msgs : semanticErrMsgs) { *outStream << msgs << endl; }

        cout << "Global variables: " << endl;
        for (auto& [entry, attr] : semTable.glbSymMap)
        {
            cout << (attr.isConst ? "const " : "var ") << attr.type->getTypeName() << " ";
            cout << entry->getName();
            if (attr.dims.size() > 0)
            {
                cout << "[";
                for (auto& dim : attr.dims) { cout << dim << " "; }
                cout << "]";
            }

            cout << " = ";
            if (attr.initVals.size() > 1) { cout << "{"; }

            for (auto& val : attr.initVals)
            {
                if (attr.type->getKind() == TypeKind::Float) { cout << get<float>(val) << " "; }
                else if (attr.type->getKind() == TypeKind::Int) { cout << get<int>(val) << " "; }
                else if (attr.type->getKind() == TypeKind::LL) { cout << get<long long>(val) << " "; }
            }

            if (attr.initVals.size() > 1) { cout << "}"; }

            cout << endl;
        }
    }

    if (file.is_open()) file.close();
    if (outFile.is_open()) outFile.close();

    return 0;
}