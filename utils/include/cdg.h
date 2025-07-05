#ifndef CDG_H
#define CDG_H

#include "llvm/pass.h"
#include "dom_analyzer.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/ir_builder.h"

using namespace LLVMIR;
using namespace std;

class CDGAnalyzer : public Pass
{
  private:
    void BuildCDG();
    void BuildSingleCDG(CFG* C);

    Cele::Algo::DomAnalyzer* ReDom;

    // 这是存储控制依赖图的vector
    // @brief 第一个参数表示CFG，第二个存储的vector表示：
    //      index：分支等的block    value：表示被这个index控制支配的块
    std::map<CFG*, std::vector<std::vector<IRBlock*>>> CDG;

    // 这是存储前驱的
    std::map<CFG*, std::vector<std::vector<IRBlock*>>> invCDG;

  public:
    CDGAnalyzer(IR* ir, Cele::Algo::DomAnalyzer* redom) : Pass(ir) { ReDom = redom; }
    std::vector<IRBlock*>& GetCDGSucc(CFG* C, int id) { return CDG[C][id]; }
    std::vector<IRBlock*>& GetCDGPre(CFG* C, int id) { return invCDG[C][id]; }
    void                  Execute();
};  // namespace LLVMIR

#endif