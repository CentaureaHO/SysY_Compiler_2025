#ifndef __BACKEND_BASE_PASS_H__
#define __BACKEND_BASE_PASS_H__

namespace Backend
{

    class BasePass
    {
      public:
        BasePass()          = default;
        virtual ~BasePass() = default;

        virtual bool run() = 0;

        virtual const char* getName() const = 0;

        BasePass(const BasePass&)            = delete;
        BasePass& operator=(const BasePass&) = delete;
    };

}  // namespace Backend

#endif  // __BACKEND_BASE_PASS_H__
