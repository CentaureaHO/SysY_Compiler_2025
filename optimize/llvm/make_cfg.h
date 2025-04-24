#ifndef MAKE_CFG_H
#define MAKE_CFG_H

#include "llvm/pass.h"


class MakeCFGPass :public Pass{
public:
    MakeCFGPass(LLVMIR::IR* ir): Pass(ir){}
    void Execute();
};

#endif