#ifndef __BACKEND_RV64_PASS_SET_GENERATOR_H__
#define __BACKEND_RV64_PASS_SET_GENERATOR_H__

#include "../pass_manager.h"
#include <llvm_ir/ir_builder.h>
#include <memory>
#include <vector>
#include <iostream>

namespace Backend::RV64
{
    class Function;
}

namespace LLVMIR
{
    class Instruction;
}

namespace Backend::RV64
{

    class PassSetGenerator
    {
      public:
        static std::vector<std::unique_ptr<Backend::BasePass>> generate(LLVMIR::IR* ir,
            std::vector<Function*>& functions, std::vector<LLVMIR::Instruction*>& glb_defs, std::ostream& out,
            int optLevel);

      private:
        PassSetGenerator() = delete;
    };

}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_PASS_SET_GENERATOR_H__
