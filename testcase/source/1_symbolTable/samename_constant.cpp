#include "defs.h"

int main()
{
    string name1 = "a";
    scopeTable->addConstant(commonInt, "1");
    cout << anonToSymTempMap["1"] << endl;

    string name2 = "int_Const_0";
    scopeTable->addVariable(name2, commonInt);

#ifdef PLACE_HOLDER
    cout << "PLACE_HOLDER is defined" << endl;
#endif
}