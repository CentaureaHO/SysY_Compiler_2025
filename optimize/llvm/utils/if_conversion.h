#ifndef __OPTIMIZER_LLVM_UTILS_IF_CONVERSION_H__
#define __OPTIMIZER_LLVM_UTILS_IF_CONVERSION_H__

#include "../pass.h"
#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <map>
#include <unordered_map>
#include <vector>

namespace Transform
{
    class IfConversionPass : public Pass
    {
      private:
        bool modified_;

      public:
        explicit IfConversionPass(LLVMIR::IR* ir);
        void Execute() override;
        bool wasModified() const { return modified_; }

      private:
        void processFunction(CFG* cfg);

        bool isOpcodeProhibitedForConversion(LLVMIR::IROpCode opcode) const;
        bool canConvertControlFlowToSelect(LLVMIR::IRBlock* block, LLVMIR::PhiInst* phi,
            const std::vector<LLVMIR::IRBlock*>& predecessors, CFG* cfg, LLVMIR::IRBlock*& ancient_block,
            std::vector<LLVMIR::Instruction*>& middle_block_insts) const;
        void convertControlFlowToSelect(LLVMIR::IRBlock* block, LLVMIR::PhiInst* phi, LLVMIR::IRBlock* ancient_block,
            const std::vector<LLVMIR::Instruction*>& middle_block_insts, CFG* cfg);

        void buildGraph(
            CFG* cfg, std::vector<std::vector<LLVMIR::IRBlock*>>& G, std::vector<std::vector<LLVMIR::IRBlock*>>& invG);
        std::vector<LLVMIR::IRBlock*> getPredecessors(LLVMIR::IRBlock* block, CFG* cfg) const;
        std::vector<LLVMIR::IRBlock*> getSuccessors(LLVMIR::IRBlock* block, CFG* cfg) const;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_UTILS_IF_CONVERSION_H__
