#ifndef __OPTIMIZER_LLVM_CSE_H__
#define __OPTIMIZER_LLVM_CSE_H__

#include "llvm/pass.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include "llvm/memdep/memdep.h"
#include <string>
#include <vector>
#include <map>
#include <set>

namespace Optimizer
{
    struct InstCSEInfo
    {
        LLVMIR::IROpCode         opcode;
        std::vector<std::string> operand_list;
        LLVMIR::IcmpCond         icmp_cond = LLVMIR::IcmpCond::EQ;   // Default
        LLVMIR::FcmpCond         fcmp_cond = LLVMIR::FcmpCond::OEQ;  // Default

        bool operator<(const InstCSEInfo& x) const
        {
            if (opcode != x.opcode) return opcode < x.opcode;
            if (opcode == LLVMIR::IROpCode::ICMP && icmp_cond != x.icmp_cond) return icmp_cond < x.icmp_cond;
            if (opcode == LLVMIR::IROpCode::FCMP && fcmp_cond != x.fcmp_cond) return fcmp_cond < x.fcmp_cond;
            return operand_list < x.operand_list;
        }
    };

    class CSEPass : public Pass
    {
      private:
        Analyser::AliasAnalyser*            alias_analyser;
        Analyser::MemoryDependenceAnalyser* memdep_analyser;

        std::set<LLVMIR::Instruction*>                           erase_set;
        std::map<InstCSEInfo, int>                               inst_cse_map;
        std::map<InstCSEInfo, std::vector<LLVMIR::Instruction*>> load_cse_map;
        std::map<int, int>                                       reg_replace_map;
        CFG*                                                     current_cfg;

        InstCSEInfo getCSEInfo(LLVMIR::Instruction* inst);
        bool        isNotCSE(LLVMIR::Instruction* inst);
        void        init();

        void callKill(LLVMIR::Instruction* inst, std::map<InstCSEInfo, int>& call_inst_map,
            std::map<InstCSEInfo, int>& load_inst_map, std::set<LLVMIR::Instruction*>& call_inst_set,
            std::set<LLVMIR::Instruction*>& load_inst_set, CFG* cfg);

        void storeKill(LLVMIR::Instruction* inst, std::map<InstCSEInfo, int>& call_inst_map,
            std::map<InstCSEInfo, int>& load_inst_map, std::set<LLVMIR::Instruction*>& call_inst_set,
            std::set<LLVMIR::Instruction*>& load_inst_set, CFG* cfg);

        bool basicBlockCSE(LLVMIR::IRBlock* bb, std::map<int, int>& reg_replace_map,
            std::set<LLVMIR::Instruction*>& erase_set, CFG* cfg);
        void domTreeWalkCSE(CFG* cfg);
        void dfsDomTree(int bb_id);

      public:
        CSEPass(LLVMIR::IR* ir, Analyser::AliasAnalyser* aa, Analyser::MemoryDependenceAnalyser* md);
        virtual void Execute() override;
    };

}  // namespace Optimizer

#endif
