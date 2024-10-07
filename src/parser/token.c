#include <parser/token.h>

const char* getName(int token)
{
    switch (token)
    {
#define X(a, b) \
    case b: return #a;
        DECLARE
#undef X
        default: return "Unknown token";
    }
}