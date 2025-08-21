#ifndef __BACKEND_RV64_PASSES_LOOP_FIND_H__
#define __BACKEND_RV64_PASSES_LOOP_FIND_H__

#include "../../base_pass.h"
#include "../rv64_function.h"
#include "../rv64_loop.h"
#include <vector>
#include <set>
#include <map>

namespace Backend::RV64::Passes
{
    class LoopFindPass : public BasePass
    {
      public:
        explicit LoopFindPass(std::vector<Function*>& functions);
        ~LoopFindPass() override = default;

        bool        run() override;
        const char* getName() const override { return "MachineLoopFind"; }

      private:
        std::vector<Function*>& functions_;

        void         findLoopsInFunction(Function* func);
        void         findBackEdges(CFG* cfg, std::vector<std::pair<Block*, Block*>>& back_edges);
        NaturalLoop* createLoopFromBackEdge(CFG* cfg, Block* latch, Block* header);
        void         buildDFSTree(CFG* cfg, std::map<Block*, int>& dfn, std::map<Block*, Block*>& parent);
        void dfsVisit(Block* block, CFG* cfg, int& time, std::map<Block*, int>& dfn, std::map<Block*, Block*>& parent,
            std::set<Block*>& visited);
    };

}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_LOOP_FIND_H__