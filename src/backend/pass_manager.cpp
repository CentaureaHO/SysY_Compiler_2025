#include <backend/pass_manager.h>

namespace Backend
{

    void PassManager::addPass(std::unique_ptr<BasePass> pass) { passes_.push_back(std::move(pass)); }

    bool PassManager::executeAll()
    {
        bool modified = false;
        for (auto& pass : passes_)
        {
            if (pass->run()) { modified = true; }
        }
        return modified;
    }

    void PassManager::clear() { passes_.clear(); }

}  // namespace Backend
