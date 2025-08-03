#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_RV64_MAKEDOM_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_RV64_MAKEDOM_H__

#include "../../../base_pass.h"
#include <vector>
#include <memory>

namespace Backend::RV64
{
    class Function;
}

namespace Backend::RV64::Passes::Optimize
{
    class RV64MakeDomTreePass : public Backend::BasePass
    {
      public:
        explicit RV64MakeDomTreePass(std::vector<Backend::RV64::Function*>& functions);
        ~RV64MakeDomTreePass() override = default;

        bool        run() override;
        const char* getName() const override { return "RV64MakeDomTree"; }

      private:
        std::vector<Backend::RV64::Function*>* functions_;

        void buildDominanceTree(Backend::RV64::Function* function);
        void buildPostDominanceTree(Backend::RV64::Function* function);
    };
}  // namespace Backend::RV64::Passes::Optimize

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_RV64_MAKEDOM_H__
