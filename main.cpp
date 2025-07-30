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
// CSE
#include "llvm/cse.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include "llvm/memdep/memdep.h"
// Branch CSE
#include "llvm/branch_cse.h"
// Loop Analysis and Simplification
#include "optimize/llvm/loop/loop_find.h"
#include "optimize/llvm/loop/loop_simplify.h"
#include "optimize/llvm/loop/lcssa.h"
#include "optimize/llvm/loop/licm.h"
#include "optimize/llvm/loop/loop_rotate.h"
#include "optimize/llvm/function_inline.h"
// Unify Return
#include "optimize/llvm/unify_return.h"
// Tail Recursion Elimination
#include "optimize/llvm/tail_recursion.h"
// Phi Precursor verify
#include "optimize/llvm/verify/phi_precursor.h"
// TSCCP
#include "optimize/llvm/t_sccp.h"
// Strength Reduction
#include "optimize/llvm/strength_reduction/const_branch_reduce.h"
#include "optimize/llvm/strength_reduction/arith_inst_reduce.h"
#include "optimize/llvm/strength_reduction/gep_strength_reduce.h"
// SCEV Analysis
#include "optimize/llvm/loop/scev_analysis.h"
// IndVars Simplify
#include "optimize/llvm/loop/indvars_simplify.h"
// Constant Loop Unroll
// #include "optimize/llvm/loop/constant_loop_unroll.h"
// GVN GCM
#include "optimize/llvm/gvn_gcm/gcm.h"
// Blockid Set
#include "optimize/llvm/setid.h"
//loop_strength_reduce
#include "llvm/loop/loop_strength_reduce.h"

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

#ifndef LOCAL_TEST
    optimizeLevel = 1;
#endif

    // 添加优化
    // StructuralTransform：可能改变 IR 块结构，需要重新构建 CFG 和 DomTree
    // Transform：可能改变 IR 结构，但不会改变 IR 块结构，不需要重新构建 CFG 和 DomTree
    // Analysis：仅分析，不改变 IR 结构，不需要重新构建 CFG 和 DomTree
    if (optimizeLevel)
    {
        // 构建CFG
        MakeCFGPass     makecfg(&builder);
        MakeDomTreePass makedom(&builder);
        MakeDomTreePass makeredom(&builder);

        Verify::PhiPrecursorPass phiPrecursor(&builder);
        makecfg.Execute();
        makedom.Execute();

        Transform::TailRecursionPass tailRecursion(&builder);
        tailRecursion.Execute();

        makecfg.Execute();
        makedom.Execute();

        Transform::UnifyReturnPass unifyReturn(&builder);
        unifyReturn.Execute();

        Mem2Reg mem2reg(&builder);
        mem2reg.Execute();

        // Loop Analysis and Simplification
        Analysis::LoopAnalysisPass loopAnalysis(&builder);
        loopAnalysis.Execute();  // inlinepass 需要，先执行一次
                                 // inlinepass 会改变程序结构，因此规范化与 SSA 形式在 inlinepass 后执行
        StructuralTransform::LoopSimplifyPass loopSimplify(&builder);
        // loopSimplify.Execute();
        // Loop Closed SSA Form - ensures loop-defined variables used outside the loop are passed through PHI nodes
        // at loop exits
        StructuralTransform::LCSSAPass lcssa(&builder);
        // lcssa.Execute();
        // std::cout << "Before Function Inline" << std::endl;
        // phiPrecursor.Execute();
        Transform::FunctionInlinePass inlinePass(&builder);
        inlinePass.Execute();

        makecfg.Execute();
        makedom.Execute();
        // std::cout << "After Function Inline" << std::endl;
        // phiPrecursor.Execute();

        StructuralTransform::LoopRotatePass loopRotate(&builder);

        loopAnalysis.Execute();
        loopSimplify.Execute();
        lcssa.Execute();
        loopRotate.Execute();
        // 已修复
        makecfg.Execute();
        makedom.Execute();

        Analysis::AliasAnalyser aa(&builder);
        aa.run();
        StructuralTransform::LICMPass licm(&builder, &aa);
        licm.Execute();

        makecfg.Execute();
        makedom.Execute();
        aa.run();
        Analysis::MemoryDependenceAnalyser md(&builder, &aa);
        md.run();
        Transform::CSEPass cse(&builder, &aa, &md);
        cse.Execute();  // 如果注释掉这里 lcssa 会出错，但我不知道为什么;
                        // 更新：这里可以神奇的修好 function inline 的 phi 重命名问题，在特定测试用例下
                        //      function inline 已经修复，因此现在它并不影响 lcssa 的执行
        StructuralTransform::BranchCSEPass branchCSE(&builder);
        branchCSE.Execute();

        makecfg.Execute();
        makedom.Execute();
        // TSCCP - Sparse Conditional Constant Propagation
        Transform::TSCCPPass tsccp(&builder, &aa);
        tsccp.Execute();

        makecfg.Execute();
        makedom.Execute();

        loopAnalysis.Execute();
        loopSimplify.Execute();
        lcssa.Execute();
        loopRotate.Execute();
        makecfg.Execute();
        makedom.Execute();

        Transform::ConstBranchReduce constBranchReduce(&builder);
        constBranchReduce.Execute();

        makecfg.Execute();
        makedom.Execute();

        // DCE
        DefUseAnalysisPass DCEDefUse(&builder);
        DCEDefUse.Execute();
        DCEPass dce(&builder, &DCEDefUse);
        dce.Execute();
        // std::cout << "DCE completed" << std::endl;

        // ADCE
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
        // GCM
        DefUseAnalysisPass GCMDefUse(&builder);
        GCMDefUse.Execute();
        MakeDomTreePass GCMmakeredom(&builder);
        GCMmakeredom.Execute(true);
        GCMmakeredom.Execute(false);
        // Used to set all instructions with the block they are in.
        SetIdAnalysis setIdAnalysis(&builder);
        setIdAnalysis.Execute();
        GCM gcm(&builder, &GCMDefUse);
        gcm.Execute();

        makecfg.Execute();
        makedom.Execute();
        makeredom.Execute(true);

        aa.run();
        licm.Execute();
        md.run();
        makecfg.Execute();
        makedom.Execute();
        makeredom.Execute(true);

        Transform::ArithInstReduce arithInstReduce(&builder);
        arithInstReduce.Execute();

        makecfg.Execute();
        makedom.Execute();

        Transform::GEPStrengthReduce gepStrengthReduce(&builder);
        gepStrengthReduce.Execute();

        makecfg.Execute();
        makedom.Execute();
        loopAnalysis.Execute();
        loopSimplify.Execute();
        loopRotate.Execute();

        for (const auto& [func_def, cfg] : builder.cfg)
        {
            std::cout << "Function: " << func_def->func_name << std::endl;
            if (!cfg || !cfg->LoopForest) continue;
            for (auto* loop : cfg->LoopForest->loop_set) loop->printLoopInfo();
        }

        tsccp.Execute();

        Analysis::SCEVAnalyser scevAnalyser(&builder);
        scevAnalyser.run();
        scevAnalyser.printAllResults();

        Transform::StrengthReducePass lsr(&builder,&scevAnalyser) ;
        lsr.Execute();
        DCEDefUse.Execute();
        dce.Execute();
        scevAnalyser.run();



        if (optimizeLevel >= 2)
        {
            // IndVars Simplify - 归纳变量简化
            Transform::IndVarsSimplifyPass indVarsPass(&builder, &scevAnalyser);
            indVarsPass.Execute();

            // 重新构建CFG和支配树，因为IndVars可能改变了循环结构
            makecfg.Execute();
            makedom.Execute();
            loopAnalysis.Execute();
            loopSimplify.Execute();
            loopRotate.Execute();
            scevAnalyser.run();
            scevAnalyser.printAllResults();
        }
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
