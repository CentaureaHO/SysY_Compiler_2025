#ifndef DCE_H
#define DCE_H
#include "llvm/pass.h"
#include "def_use.h"

using namespace LLVMIR;
using namespace std;

class DCEPass : public Pass
{
  private:
    DefUseAnalysisPass* DefUse;

    void DceInSingleCFG(CFG* C);

  public:
    DCEPass(IR* ir, DefUseAnalysisPass* defuse) : Pass(ir) { this->DefUse = defuse; }
    void Execute();
};

#endif