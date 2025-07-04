#ifndef MEM2REG_H
#define MEM2REG_H

#include "llvm/pass.h"

class Mem2Reg :public Pass{
public:
    std::set<int> alloca_regs{};//要mem2reg的寄存器号
    std::map<int,int> replaces;
    std::map<LLVMIR::PhiInst*,int> new_phis;//phi到的新寄存器号
    std::map<LLVMIR::Instruction*,int> todel;//标记要删除的指令
    
    Mem2Reg(LLVMIR::IR* ir): Pass(ir){}
    void Execute();
    void Mem2Reg_func(CFG* cfg);//对特定函数做mem2reg
    void InsertPhi(CFG* cfg); //插入phi
    void Rename(CFG* cfg);//变量重命名
    bool CanMem2Reg(LLVMIR::Instruction* alloca_inst);
    void Mem2RegUseDefInSameBlock(CFG* C,std::set<int> &vset,int block_id);
};

#endif