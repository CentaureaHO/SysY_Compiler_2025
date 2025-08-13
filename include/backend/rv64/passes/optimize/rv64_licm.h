#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_LICM_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_LICM_H__

#include "../../../base_pass.h"
#include "../../rv64_function.h"
#include "../../rv64_loop.h"
#include <vector>
#include <set>
#include <map>

namespace Backend::RV64::Passes::Optimize
{
    class LICMPass : public BasePass
    {
      public:
        explicit LICMPass(std::vector<Function*>& functions);
        ~LICMPass() override = default;

        bool        run() override;
        const char* getName() const override { return "MachineLICM"; }

      private:
        std::vector<Function*>& functions_;
        Function*               current_func;
        NaturalLoop*            curr_loop;

        uint32_t max_motion_count      = 0;
        uint32_t currloop_motion_count = 0;

        std::map<int, Block*>     inst_def_map;         ///< virtual register -> defining block
        std::set<int>             invariant_set;        ///< invariant virtual registers in current loop
        std::vector<Instruction*> invariant_inst_list;  ///< invariant instructions to hoist
        std::set<Instruction*>    invariant_inst_set;   ///< set for fast lookup

        void executeInFunction(Function* func);
        void initResultMapInCurrFunc();

        bool isNeedLicm(Instruction* inst);
        bool isInvariant(CFG* cfg, Instruction* inst, NaturalLoop* loop);
        void getInvariantInCurrLoop();
        void addPreheader();
        void licmInCurrLoop();

        int         getMaxInstMotionNumber(NaturalLoop* loop);
        std::string getLICMInstInfo(Instruction* inst);

        void processLoopRecursively(NaturalLoop* loop);
    };

}  // namespace Backend::RV64::Passes::Optimize

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_LICM_H__