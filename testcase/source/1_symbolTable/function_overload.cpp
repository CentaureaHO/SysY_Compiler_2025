#include "defs.h"

extern SymbolTable*                  globalTable;
extern SymbolTable*                  scopeTable;
extern unordered_map<string, string> anonToSymTempMap;

void a(int b, int c);
void a(int b, float c);
// int a(int b, int c);
void b(int b, int c);

int main()
{
    // global scope
    assert(scopeTable->getScopeLevel() == 0);

    std::string funcName = "a";

    Type*          retType1     = commonVoid;
    Type*          retType2     = commonVoid;
    Type*          retType3     = commonInt;
    vector<Type*>  aParamTypes1 = {commonInt, commonInt};
    vector<Type*>  aParamTypes2 = {commonInt, commonFloat};
    vector<Type*>  aParamTypes3 = {commonInt, commonInt};
    vector<Type*>& bParamType   = aParamTypes1;

    std::string funcName1 = FN(funcName, aParamTypes1);
    std::string funcName2 = FN(funcName, aParamTypes2);
    std::string funcName3 = FN(funcName, aParamTypes3);
    string      bFuncName = FN("b", bParamType);

    scopeTable->addFunction(funcName, retType1, aParamTypes1);
    scopeTable->addFunction(funcName, retType2, aParamTypes2);
    try
    {
        scopeTable->addFunction(funcName, retType3, aParamTypes3);
    } catch (const std::runtime_error& e)
    {
        cout << e.what() << endl;
    }
    scopeTable->addFunction("b", retType1, bParamType);

    SymbolTableEntry* func_a1 = scopeTable->findEntry(funcName1);
    SymbolTableEntry* func_a2 = scopeTable->findEntry(funcName2);
    SymbolTableEntry* func_a3 = scopeTable->findEntry(funcName3);
    SymbolTableEntry* func_b  = scopeTable->findEntry(bFuncName);

    assert(func_a1 != nullptr);
    assert(func_a2 != nullptr);
    assert(func_a3 != nullptr);
    assert(func_b != nullptr);
    assert(func_a1 == func_a3);

    cout << func_a1->toString() << " at " << func_a1 << endl;
    cout << func_a2->toString() << " at " << func_a2 << endl;
    cout << func_a3->toString() << " at " << func_a3 << endl;
    cout << func_b->toString() << " at " << func_b << endl;

    cout << "All tests passed" << endl;

    return 0;
}