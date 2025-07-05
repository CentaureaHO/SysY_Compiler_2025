#ifndef DEF_USE_H
#define DEF_USE_H

#include "llvm/pass.h"

using namespace LLVMIR;
using namespace std;
class DefUseAnalysisPass : public Pass
{
  public:
    DefUseAnalysisPass(LLVMIR::IR* ir) : Pass(ir) {}
    std::map<int, Instruction*>&           GetDefMap(CFG* C) { return IRDefMaps[C]; }
    std::map<int, std::set<Instruction*>>& GetUseMap(CFG* C) { return IRUseMaps[C]; }
    std::map<Instruction*, int>&           GetDef2idmap(CFG* C) { return IRDef2bbid[C]; }
    void                                  Execute();

  private:
    void GetDefUseInSingleCFG(CFG* C);

    //<key1:CFG*, value1:
    //      <key2: 寄存器编号, value2: 定义该寄存器的指令 > >
    std::map<CFG*, std::map<int, Instruction*>> IRDefMaps;

    // 记录每个CFG对应的use
    //<key1:CFG*, value2:
    //      <key2: 寄存器标号, value2: 使用该寄存器的指令集合 > >
    std::map<CFG*, std::map<int, std::set<Instruction*>>> IRUseMaps;

    std::map<CFG*, std::map<Instruction*, int>> IRDef2bbid;

    // 首先我们添加一个Set，用来记录一个CFG的函数参数变量
    //  ->  似乎没必要作为成员变量，太占空间了，直接作为函数内的似乎就可以了

    // 加个函数处理有没有resultreg
    // 返回值为寄存器号，如果没有，那么返回-1
    // int GetResultReg(Instruction I);
    // 没用，因为不好处理算术，所以还是需要加虚函数

    // 添加更多的成员变量和函数来完成def-use分析
};

#endif