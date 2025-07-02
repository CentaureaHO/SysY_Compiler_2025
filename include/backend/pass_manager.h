#ifndef __BACKEND_PASS_MANAGER_H__
#define __BACKEND_PASS_MANAGER_H__

#include <memory>
#include <vector>
#include "base_pass.h"
#include "base_backend.h"

namespace Backend
{

    class PassManager
    {
      public:
        PassManager()  = default;
        ~PassManager() = default;

        void   addPass(std::unique_ptr<BasePass> pass);
        bool   executeAll();
        void   clear();
        size_t getPassCount() const { return passes_.size(); }

      private:
        std::vector<std::unique_ptr<BasePass>> passes_;

        PassManager(const PassManager&)            = delete;
        PassManager& operator=(const PassManager&) = delete;
    };

}  // namespace Backend

#endif  // __BACKEND_PASS_MANAGER_H__
