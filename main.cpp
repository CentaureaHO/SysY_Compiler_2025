#include "llvm_ir/defs.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <cstring>
#include <ostream>
#include <string>
#include <parser/driver.h>
#include <common/type/symtab/symbol_table.h>
#include <common/type/symtab/semantic_table.h>
#include <llvm_ir/ir_builder.h>
#include <backend/factory.h>

// llvmIR Optimizers
// MEM2REG
#include "llvm/cdg.h"
#include "llvm/def_use.h"
#include "llvm/make_cfg.h"
#include "llvm/make_domtree.h"
#include "llvm/mem2reg.h"
// DCE
#include "llvm/dce.h"
// ADCE
#include "llvm/adce.h"

#define STR_PW 30
#define INT_PW 8
#define MIN_GAP 5
#define STR_REAL_WIDTH (STR_PW - MIN_GAP)

using namespace std;
using namespace Yacc;
using namespace Symbol;
using namespace LLVMIR;

extern vector<string> semanticErrMsgs;

extern IR builder;
size_t    errCnt = 0;

bool no_reg_alloc = false;

string truncateString(const string& str, size_t width)
{
    if (str.length() > width) return str.substr(0, width - 3) + "...";
    return str;
}

int main(int argc, char** argv)
{
    string   inputFile     = "";
    string   outputFile    = "a.out";
    string   step          = "-llvm";
    int      optimizeLevel = 0;
    ostream* outStream     = &cout;
    ofstream outFile;

    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];

        if (arg == "-lexer" || arg == "-parser" || arg == "-llvm" || arg == "-S") { step = arg; }
        else if (arg == "-o")
        {
            if (i + 1 < argc)
                outputFile = argv[++i];
            else
            {
                cerr << "Error: -o option requires a filename" << endl;
                return 1;
            }
        }
        else if (arg == "-O" || arg == "-O1") { optimizeLevel = 1; }
        else if (arg == "-O0") { optimizeLevel = 0; }
        else if (arg == "-O2") { optimizeLevel = 2; }
        else if (arg == "-O3") { optimizeLevel = 3; }
        else if (arg == "-no-reg-alloc") { no_reg_alloc = true; }
        else if (arg[0] != '-') { inputFile = arg; }
        else
        {
            cerr << "Unknown option: " << arg << endl;
            return 1;
        }
    }

    if (inputFile.empty())
    {
        cerr << "Error: No input file specified" << endl;
        cerr << "Usage: " << argv[0] << " [-lexer|-parser|-llvm|-S] [-o output_file] input_file [-O]" << endl;
        return 1;
    }

    if (!outputFile.empty())
    {
        outFile.open(outputFile);
        if (!outFile)
        {
            cerr << "Cannot open output file " << outputFile << endl;
            return 1;
        }
        outStream = &outFile;
    }

    cout << "Input file: " << inputFile << endl;
    cout << "Step: " << step << endl;
    cout << "Output: " << (outputFile.empty() ? "standard output" : outputFile) << endl;
    cout << "Optimize level: " << optimizeLevel << endl;

    ifstream in(inputFile);
    if (!in)
    {
        cerr << "Cannot open input file " << inputFile << endl;
        if (outFile.is_open()) outFile.close();
        return 1;
    }
    istream* inStream = &in;

    Driver driver(inStream, outStream);

    if (step == "-lexer")
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
                // TODO: Fix std::variant compatibility issue with older compilers
                *outStream << setw(STR_PW) << "TODO_VALUE";
            }
            else { *outStream << setw(STR_PW) << " "; }

            *outStream << setw(INT_PW) << token.line << setw(INT_PW) << token.column << endl;
        }

        if (in.is_open()) in.close();
        if (outFile.is_open()) outFile.close();

        return 0;
    }

    ASTree* ast = driver.parse();
    if (errCnt)
    {
        *outStream << errCnt << " errors found during parsing, exiting..." << endl;
        if (in.is_open()) in.close();
        if (outFile.is_open()) outFile.close();

        return 2;
    }

    if (step == "-parser")
    {
        if (ast) { ast->printAST(outStream); }
        if (in.is_open()) in.close();
        if (outFile.is_open()) outFile.close();
        return 0;
    }

    semTable = new SemanticTable::Table();
    semTable->reg_funcs();
    ast->typeCheck();
    if (semanticErrMsgs.size() > 0)
    {
        cout << "\nSemantic errors found: " << endl;

        int idx = 0;
        for (auto& msg : semanticErrMsgs) cerr << ++idx << ". " << msg << endl;

        cout << "\n\nExiting..." << endl;

        if (in.is_open()) in.close();
        if (outFile.is_open()) outFile.close();

        if (semTable)
        {
            delete semTable;
            semTable = nullptr;
        }
        return 3;
    }

    ast->genIRCode();
    // 添加优化
    if (optimizeLevel)
    {
        // 构建CFG
        MakeCFGPass makecfg(&builder);
        makecfg.Execute();
        MakeDomTreePass makedom(&builder);
        makedom.Execute();
        Mem2Reg mem2reg(&builder);
        mem2reg.Execute();
        // DCE
        DefUseAnalysisPass DCEDefUse(&builder);
        DCEDefUse.Execute();
        DCEPass dce(&builder, &DCEDefUse);
        dce.Execute();
        // std::cout << "DCE completed" << std::endl;

        // ADCE
        MakeDomTreePass makeredom(&builder);
        makeredom.Execute(true);
        // std::cout << "Reversed dom tree completed" << std::endl;
        CDGAnalyzer cdg(&builder);
        cdg.Execute();
        // std::cout << "CDG completed" << std::endl;
        DefUseAnalysisPass ADCEDefUse(&builder);
        ADCEDefUse.Execute();
        ADCEPass adce(&builder, &ADCEDefUse, &cdg);
        adce.Execute();
        // std::cout << "ADCE completed" << std::endl;
    }

    if (step == "-llvm")
    {
        builder.printIR(outFile);
        if (in.is_open()) in.close();
        if (outFile.is_open()) outFile.close();
        return 0;
    }

    // RISCV backend processing for -S step
    if (step == "-S")
    {
        if (!optimizeLevel)
        {
            MakeCFGPass makecfg(&builder);
            makecfg.Execute();
        }

        auto backend =
            Backend::Factory::createBackend(Backend::Factory::TargetArch::RV64, &builder, *outStream, optimizeLevel);
        backend->run();
    }

    if (in.is_open()) in.close();
    if (outFile.is_open()) outFile.close();
    if (semTable)
    {
        delete semTable;
        semTable = nullptr;
    }
    return 0;
}
