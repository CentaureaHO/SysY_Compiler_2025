#include <defs/type.h>
#include <symbols/symbolTable.h>
#include <bits/stdc++.h>
using namespace std;
inline string FN(const string& a, const vector<Type*>& b) { return SymbolTable::getFuncname(a, b); }

int main()
{
    string a = scopeTable->addConstant(commonInt, "10");
    // scopeTable->addVariable("b", commonInt);
    scopeTable->addVariable("b", commonFloat);
    // scopeTable->addFunction("c", commonFloat, vector<Type*>{commonInt, commonInt});
    // scopeTable->addFunction("d", commonFloat, vector<Type*>{commonInt, commonInt});

    cout << scopeTable->findEntry(a)->toString() << endl;
    cout << scopeTable->findEntry("b")->toString() << endl;
    // cout << scopeTable->findEntry(FN("c", vector<Type*>{commonInt, commonInt}))->toString() << endl;
    // cout << scopeTable->findEntry(SymbolTable::getFuncname("d", vector<Type*>{commonInt, commonInt}))->toString()
    //  << endl;

    // scopeTable->addFunction("c", commonFloat, vector<Type*>{commonInt, commonInt, commonFloat});

    scopeTable->enterScope();
    cout << scopeTable->getScopeLevel() << endl;
    scopeTable->addConstant("a", commonInt, "20");
    scopeTable->addVariable("b", commonInt);
    cout << scopeTable->findEntry("a")->toString() << endl;
    cout << scopeTable->findEntry("b")->toString() << endl;
    // cout << scopeTable->findEntry("c(int, int)")->toString() << endl;

    scopeTable->enterScope();
    cout << scopeTable->findEntry("b")->toString() << endl;
    return 0;
}