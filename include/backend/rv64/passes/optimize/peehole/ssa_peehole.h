#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_PEEHOLE_SSA_PEEHOLE_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_PEEHOLE_SSA_PEEHOLE_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include <vector>
#include <map>

namespace Backend::RV64::Passes::Optimize::Peehole
{

    class SSAPeepholePass : public BasePass
    {
      public:
        explicit SSAPeepholePass(std::vector<Function*>& functions);
        ~SSAPeepholePass() override = default;

        bool        run() override;
        const char* getName() const override { return "SSAPeehole"; }

      private:
        std::vector<Function*>& functions_;
        static constexpr int    WINDOW_LENGTH = 10;

        void optimizeFunction(Function* func);

        bool tryAddi(std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block);
        bool tryFmla(std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block);
        bool tryShxAdd(std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block);
        bool tryConstShxAdd(std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur,
            Block* block, Function* func);
        bool tryMemOffset(std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block);

        bool         inFMList(RV64InstType opcode);
        RV64InstType reverseMulOp(RV64InstType opcode);
        RV64InstType reverseAddOp(RV64InstType opcode);
    };

}  // namespace Backend::RV64::Passes::Optimize::Peehole

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_PEEHOLE_SSA_PEEHOLE_H__
