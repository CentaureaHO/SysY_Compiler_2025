#ifndef PASS_H
#define PASS_H

#include "llvm_ir/ir_builder.h"

class Pass
{
  public:
    LLVMIR::IR*  ir;
    virtual void Execute() = 0;
    Pass(LLVMIR::IR* ir) { this->ir = ir; }
};

#endif