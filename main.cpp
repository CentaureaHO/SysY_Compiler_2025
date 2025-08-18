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
#include "llvm/alias_analysis/ealias_analysis.h"
#include "llvm/cdg.h"
#include "llvm/def_use.h"
#include "llvm/elimination/unusedarr.h"
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
// GVN GCM
#include "optimize/llvm/gvn_gcm/gcm.h"
// Blockid Set
#include "optimize/llvm/setid.h"
// Global Analysis
#include "optimize/llvm/global_analysis/readonly.h"
// Array Alias Analysis
#include "optimize/llvm/alias_analysis/arralias_analysis.h"
// EDefUse Analysis
#include "optimize/llvm/defuse_analysis/edefuse.h"
// Elimination
#include "optimize/llvm/loop/loop_full_unroll.h"
#include "optimize/llvm/loop/loop_partial_unroll.h"
// loop_strength_reduce
#include "llvm/loop/loop_strength_reduce.h"
// Single Source Phi Elimination
#include "optimize/llvm/utils/single_source_phi_elimination.h"
// Constant Branch Folding
#include "optimize/llvm/utils/constant_branch_folding.h"
// DSE
#include "optimize/llvm/alias_analysis/ealias_analysis.h"
#include "optimize/llvm/dse.h"
// Trench Path
#include "optimize/llvm/trenchpath.h"
// Global Const Replace
#include "optimize/llvm/utils/global_const_replace.h"
// Eliminate Double Br Uncond
#include "optimize/llvm/utils/eliminate_double_br_uncond.h"
// If Conversion
#include "optimize/llvm/utils/if_conversion.h"
#include "optimize/llvm/utils/block_layout_optimization.h"
// Instruction Simplify
#include "optimize/llvm/utils/instruction_simplify.h"
// Min/Max Recognition
#include "optimize/llvm/utils/min_max_recognize.h"
#include "optimize/llvm/loop/pattern_recognize/memset_pattern_recognize.h"
// unused_func_elimination
#include "optimize/llvm/unused_func_elimination.h"

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

bool no_reg_alloc     = false;
bool no_select_lower  = false;
bool no_schedule_zext = false;
int  max_unroll_count = 100;

int opt_level = 0;

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
        else if (arg == "-uc")
        {
            if (i + 1 < argc)
                max_unroll_count = stoi(argv[++i]);
            else
            {
                cerr << "Error: -uc option requires a number" << endl;
                return 1;
            }
        }
        else if (arg == "-O" || arg == "-O1") { optimizeLevel = 1; }
        else if (arg == "-O0") { optimizeLevel = 0; }
        else if (arg == "-O2") { optimizeLevel = 2; }
        else if (arg == "-O3") { optimizeLevel = 3; }
        else if (arg == "-no-reg-alloc") { no_reg_alloc = true; }
        else if (arg == "-no-sel-lower") { no_select_lower = true; }
        else if (arg == "-no-sche-zext") { no_schedule_zext = true; }
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
        opt_level = optimizeLevel;

        // 构建CFG
        MakeCFGPass     makecfg(&builder);
        MakeDomTreePass makedom(&builder);
        MakeDomTreePass makeredom(&builder);

        Analysis::LoopAnalysisPass             loopAnalysis(&builder);
        StructuralTransform::LoopSimplifyPass  loopSimplify(&builder);
        StructuralTransform::LCSSAPass         lcssa(&builder);
        StructuralTransform::LoopRotatePass    loopRotate(&builder);
        Transform::EliminateDoubleBrUncondPass eliminateDoubleBr(&builder);
        Transform::InstructionSimplifyPass     instSimplify(&builder);
        Transform::IfConversionPass            ifConversion(&builder);
        Transform::BlockLayoutOptimizationPass blockLayout(&builder);
        Transform::MinMaxRecognizePass         minMaxRecognize(&builder);
        Transform::MemsetPatternRecognizePass  memsetRecognize(&builder);

        auto loopPreProcess = [&]() {
            makecfg.Execute();
            makedom.Execute();
            loopAnalysis.Execute();
            loopSimplify.Execute();
            lcssa.Execute();
            loopRotate.Execute();

            memsetRecognize.Execute();
        };

        // 在所有循环相关操作完成后才可执行
        // 原因是牛魔的 makecfg 清空 cfg 内所有信息导致循环pass得写的绕来绕去
        // 这里会破坏循环pass内部的一些假定
        // 因此这里需要将所有循环相关操作完成后才可执行
        auto cfgSimplify = [&]() {
            makecfg.Execute();
            eliminateDoubleBr.Execute();
            makecfg.Execute();
            instSimplify.Execute();
            ifConversion.Execute();
            makecfg.Execute();
            minMaxRecognize.Execute();
            makecfg.Execute();
            blockLayout.Execute();
            makecfg.Execute();
            makedom.Execute();
        };

        Verify::PhiPrecursorPass phiPrecursor(&builder);
        makecfg.Execute();
        makedom.Execute();

        Transform::GlobalConstReplacePass globalConstReplace(&builder);
        // globalConstReplace.Execute();

        Transform::TailRecursionPass tailRecursion(&builder);
        tailRecursion.Execute();

        makecfg.Execute();
        makedom.Execute();

        Transform::UnifyReturnPass unifyReturn(&builder);
        unifyReturn.Execute();

        Mem2Reg mem2reg(&builder);
        mem2reg.Execute();

        // Loop Analysis and Simplification
        loopAnalysis.Execute();  // inlinepass 需要，先执行一次
                                 // inlinepass 会改变程序结构，因此规范化与 SSA 形式在 inlinepass 后执行
        // loopSimplify.Execute();
        // Loop Closed SSA Form - ensures loop-defined variables used outside the loop are passed through PHI nodes
        // at loop exits
        // lcssa.Execute();
        // std::cout << "Before Function Inline" << std::endl;
        // phiPrecursor.Execute();
        Transform::FunctionInlinePass inlinePass(&builder);
        inlinePass.Execute();

        // 直接删除内联后没用的函数
        Transform::UnusedFuncEliminationPass unused_func_del(&builder);
        unused_func_del.Execute();

        loopPreProcess();
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

        Transform::GEPStrengthReduce gepStrengthReduce(&builder);
        gepStrengthReduce.Execute();

        // TSCCP - Sparse Conditional Constant Propagation
        Transform::TSCCPPass tsccp(&builder);
        loopPreProcess();
        tsccp.Execute();
        // std::cout << "TSCCP completed" << std::endl;

        ifConversion.Execute();
        makecfg.Execute();
        minMaxRecognize.Execute();

        loopPreProcess();
        makecfg.Execute();
        makedom.Execute();

        Transform::ConstBranchReduce constBranchReduce(&builder);
        constBranchReduce.Execute();

        makecfg.Execute();
        makedom.Execute();

        // Eliminate unused array
        Analysis::EDefUseAnalysis edefUseAnalysis(&builder);
        edefUseAnalysis.run();
        // edefUseAnalysis.print();
        UnusedArrEliminator unusedelimator(&builder, &edefUseAnalysis);
        unusedelimator.Execute();

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
        edefUseAnalysis.run();
        makedom.Execute();
        MakeDomTreePass GCMRedom(&builder);
        GCMRedom.Execute(true);
        // Used to set all instructions with the block they are in.
        SetIdAnalysis setIdAnalysis(&builder);
        setIdAnalysis.Execute();
        aa.run();
        licm.Execute();
        md.run();
        Analysis::ReadOnlyGlobalAnalysis readOnlyGlobalAnalysis(&builder, &aa);
        readOnlyGlobalAnalysis.run();
        Analysis::ArrAliasAnalysis arrAliasAnalysis(&builder);
        arrAliasAnalysis.run();
        // arrAliasAnalysis.print();
        cdg.Execute();
        // readOnlyGlobalAnalysis.print();
        GCM gcm(&builder, &edefUseAnalysis, &aa, &arrAliasAnalysis, &md, &readOnlyGlobalAnalysis, &cdg);
        gcm.Execute();
        // std::cout << "GCM completed" << std::endl;

        makecfg.Execute();
        makedom.Execute();
        makeredom.Execute(true);

        aa.run();
        licm.Execute();
        md.run();

        // eDefUse.print();
        Transform::ArithInstReduce arithInstReduce(&builder);
        arithInstReduce.Execute();

        makecfg.Execute();
        makedom.Execute();

        gepStrengthReduce.Execute();

        makecfg.Execute();
        makedom.Execute();
        makeredom.Execute(true);
        loopPreProcess();

        // for (const auto& [func_def, cfg] : builder.cfg)
        // {
        //     std::cout << "Function: " << func_def->func_name << std::endl;
        //     if (!cfg || !cfg->LoopForest) continue;
        //     for (auto* loop : cfg->LoopForest->loop_set) loop->printLoopInfo();
        // }

        loopPreProcess();
        tsccp.Execute();

        Analysis::SCEVAnalyser scevAnalyser(&builder);
        scevAnalyser.run();
        // scevAnalyser.printAllResults();

        Transform::IndVarsSimplifyPass indVarsPass(&builder, &scevAnalyser);
        // indVarsPass.Execute();

        loopPreProcess();
        scevAnalyser.run();
        // scevAnalyser.printAllResults();

        Transform::StrengthReducePass lsr(&builder, &scevAnalyser);
        lsr.Execute();
        DCEDefUse.Execute();
        dce.Execute();

        loopPreProcess();
        scevAnalyser.run();

        // Constant Loop Full Unroll
        {
            int unrolled_count = 0;
            int total_unrolled = 0;

            do {
                Transform::LoopFullUnrollPass loopUnrollPass(&builder, &scevAnalyser);
                loopUnrollPass.Execute();

                auto stats     = loopUnrollPass.getUnrollStats();
                unrolled_count = stats.second;
                total_unrolled += unrolled_count;

                loopPreProcess();
                scevAnalyser.run();

                std::cout << "Unrolled " << unrolled_count << " times" << std::endl;

            } while (unrolled_count > 0 && total_unrolled < max_unroll_count);
            makecfg.Execute();
            makedom.Execute();
            aa.run();
            loopPreProcess();
            tsccp.Execute();
        }
        // SCCP after constant full unroll
        {
            Transform::SingleSourcePhiEliminationPass singleSourcePhiElim(&builder);
            Transform::ConstantBranchFoldingPass      constantBranchFolding(&builder);
            singleSourcePhiElim.setPreserveLCSSA(true);

            bool      changed        = true;
            int       exec_cnt       = 0;
            const int MAX_ITERATIONS = 10;

            while (changed && exec_cnt < MAX_ITERATIONS)
            {
                changed = false;
                exec_cnt++;

                constantBranchFolding.Execute();
                changed |= constantBranchFolding.wasModified();

                makecfg.Execute();
                loopAnalysis.Execute();
                loopSimplify.Execute();

                singleSourcePhiElim.Execute();
                changed |= singleSourcePhiElim.wasModified();

                makecfg.Execute();
                makedom.Execute();
                aa.run();

                loopPreProcess();
                tsccp.Execute();

                ifConversion.Execute();
                makecfg.Execute();
                minMaxRecognize.Execute();

                makecfg.Execute();
                makedom.Execute();

                std::cout << "Iteration " << exec_cnt << ": " << (changed ? "modifications made" : "no changes")
                          << std::endl;
            }
        }

        makecfg.Execute();
        instSimplify.Execute();

        makecfg.Execute();
        makedom.Execute();
        aa.run();
        loopPreProcess();
        // for (auto& [func_def, cfg] : builder.cfg)
        // {
        //     if (!cfg || !cfg->LoopForest) continue;
        //     for (auto* loop : cfg->LoopForest->loop_set) loop->printLoopInfo();
        // }
        scevAnalyser.run();
        // scevAnalyser.printAllResults();

        // Partial Loop Unroll
        {
            Transform::LoopPartialUnrollPass loopPartialUnrollPass(&builder, &scevAnalyser);
            loopPartialUnrollPass.Execute();
            // builder.printIR(std::cerr);

            auto unroll_stats = loopPartialUnrollPass.getUnrollStats();
            std::cout << "Partial unroll: processed " << unroll_stats.first << " loops, unrolled "
                      << unroll_stats.second << " loops" << std::endl;
        }

        makecfg.Execute();
        makedom.Execute();
        EAliasAnalysis::EAliasAnalyser ealias_analyser(&builder);
        DSEPass dse(&builder, &ealias_analyser, &edefUseAnalysis, &arrAliasAnalysis);
        dse.Execute();

        makecfg.Execute();
        makedom.Execute();
        makeredom.Execute(true);

        aa.run();
        licm.Execute();
        md.run();

        // for (int i = 0; i < 5; ++i)
        {
            makecfg.Execute();
            loopPreProcess();
            tsccp.Execute();
            memsetRecognize.Execute();
            edefUseAnalysis.run();
            unusedelimator.Execute();
            DCEDefUse.Execute();
            dce.Execute();
        }

        makecfg.Execute();
        aa.run();
        TrenchPath trenchPath(&builder);
        trenchPath.Execute();

        // cfgSimplify();
        loopPreProcess();
        scevAnalyser.run();

        lsr.Execute();
        DCEDefUse.Execute();
        dce.Execute();
        scevAnalyser.run();

        makecfg.Execute();
        makedom.Execute();

        arithInstReduce.Execute();

        makecfg.Execute();
        makedom.Execute();
        loopAnalysis.Execute();
        tsccp.Execute();
        memsetRecognize.Execute();

        gepStrengthReduce.Execute();

        {
            Transform::SingleSourcePhiEliminationPass singleSourcePhiElim(&builder);
            singleSourcePhiElim.setPreserveLCSSA(false);
            singleSourcePhiElim.Execute();
            cfgSimplify();
        }

        makecfg.Execute();
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
        std::cout << "Middle end passed, enter backend processing..." << std::endl;

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
