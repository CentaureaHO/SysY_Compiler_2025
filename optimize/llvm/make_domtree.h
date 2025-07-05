#ifndef MAKE_DOMTREE_H
#define MAKE_DOMTREE_H

#include "llvm/pass.h"

class MakeDomTreePass : public Pass
{
  public:
    MakeDomTreePass(LLVMIR::IR* ir) : Pass(ir) {}
    void Execute();
    void Execute(bool reserve);
};

#endif