#ifndef ADCE_H
#define ADCE_H

#include "llvm/pass.h"
#include "def_use.h"
#include "cdg.h"
#include "dom_analyzer.h"
#include "llvm_ir/ir_builder.h"

using namespace LLVMIR;

class ADCEPass : public Pass
{
  private:
    DefUseAnalysisPass* DefUse;

    // CDG Analysis
    CDGAnalyzer* CDG;

    // 在这个函数，对一个CFG进行dce处理
    void ADceInSingleCFG(CFG* C);

  public:
    ADCEPass(IR* ir, DefUseAnalysisPass* defuse, CDGAnalyzer* cdg) : Pass(ir)
    {
        DefUse = defuse;
        CDG    = cdg;
    }
    void Execute();
};

#endif