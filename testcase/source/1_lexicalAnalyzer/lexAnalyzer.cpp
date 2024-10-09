#include <bits/stdc++.h>
#include <parse/token.h>
#include <yacc.h>
using namespace std;

#define STR_PW 30
#define INT_PW 8
#define MIN_GAP 5
#define STR_REAL_WIDTH (STR_PW - MIN_GAP)

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

    TokenParser parser(*inStream);

    *outStream << left;
    *outStream << setw(STR_PW) << "Token" << setw(STR_PW) << "Lexeme" << setw(STR_PW) << "Property"
               << setw(INT_PW) << "Line" << setw(INT_PW) << "Column" << endl;

    for (auto& token : parser.tokens)
    {
        *outStream << setw(STR_PW) << truncateString(getName(get<0>(token)), STR_REAL_WIDTH)  // Token
                   << setw(STR_PW) << truncateString(get<1>(token), STR_REAL_WIDTH)           // Lexeme
                   << setw(STR_PW) << truncateString(get<2>(token), STR_REAL_WIDTH)           // Property
                   << setw(INT_PW) << get<3>(token)                                           // Line
                   << setw(INT_PW) << get<4>(token)                                           // Column
                   << endl;
        if (get<0>(token) == ERR_TOKEN)
        {
            cerr << "Error token: " << get<1>(token) << " at line " << get<3>(token) << ", column " << get<4>(token)
                 << endl;
            exit(1);
        }
    }

    if (file.is_open()) file.close();
    if (outFile.is_open()) outFile.close();

    return 0;
}