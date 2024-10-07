#include <defs/type.h>

static voidType  commonVoidInstance;
static intType   commonIntInstance;
static floatType commonFloatInstance;

voidType*  commonVoid  = &commonVoidInstance;
intType*   commonInt   = &commonIntInstance;
floatType* commonFloat = &commonFloatInstance;