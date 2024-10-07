#include <defs/type.h>

static voidType  commonVoidInstance;
static intType   commonIntInstance;
static floatType commonFloatInstance;

static PlaceHolder placeHolderInstance;

voidType*  commonVoid  = &commonVoidInstance;
intType*   commonInt   = &commonIntInstance;
floatType* commonFloat = &commonFloatInstance;

PlaceHolder* placeHolder = &placeHolderInstance;