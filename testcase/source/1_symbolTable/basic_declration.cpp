#include "defs.h"

extern SymbolTable*                  globalTable;
extern SymbolTable*                  scopeTable;
extern unordered_map<string, string> anonToSymTempMap;

int   a;
int   z;
float b = 3.14;

void c(int a, float f)
{
    // int a = 10; // forbidden: redeclaration of parameter
    float b = 10;
    int   c;  // c1
    float d = b;
    {
        float c;  // c2
        int   e = z;
    }
}

int main()
{
    // scope = global
    assert(scopeTable->getScopeLevel() == 0);

    scopeTable->addVariable("a", commonInt);       // int a;
    scopeTable->addVariable("z", commonInt);       // int z;
    scopeTable->addVariable("b", commonFloat);     // float b
    scopeTable->addConstant(commonFloat, "3.14");  // = 3.14;
    vector<Type*> cParamTypes = {commonInt, commonFloat};
    scopeTable->addFunction("c", commonVoid, cParamTypes);  // void c(int a, float b)

    list<SymbolTableEntry*> entries;

    SymbolTableEntry* glb_a = scopeTable->findEntry("a");
    entries.push_back(glb_a);
    SymbolTableEntry* glb_z = scopeTable->findEntry("z");
    entries.push_back(glb_z);
    SymbolTableEntry* glb_b = scopeTable->findEntry("b");
    entries.push_back(glb_b);
    SymbolTableEntry* param_3p14 = scopeTable->findEntry(anonToSymTempMap["3.14"]);
    entries.push_back(param_3p14);
    SymbolTableEntry* func_c = scopeTable->findEntry(FN("c", cParamTypes));
    entries.push_back(func_c);

    for (auto entry : entries)
    {
        assert(entry->getType() != nullptr);
        cout << entry->toString() << " at " << entry << endl;
    }
    entries.clear();

    // scope = c(int, float) params
    cout << "\nEntering c(int, float) params" << endl;
    scopeTable->enterScope();
    assert(scopeTable->getScopeLevel() == 1);

    scopeTable->addVariable("a", commonInt);    // int a;
    scopeTable->addVariable("f", commonFloat);  // float b

    SymbolTableEntry* param_c_a = scopeTable->findEntry("a");
    SymbolTableEntry* param_c_f = scopeTable->findEntry("f");
    entries.push_back(param_c_a);
    entries.push_back(param_c_f);

    for (auto entry : entries)
    {
        assert(entry != nullptr);
        cout << entry->toString() << " at " << entry << endl;
    }
    entries.clear();
    assert(param_c_a != glb_a);

    // scope = c(int, float) body
    cout << "\nEntering c(int, float) body" << endl;
    scopeTable->enterScope();
    assert(scopeTable->getScopeLevel() >= 2);

    try
    {
        scopeTable->addVariable("a", commonInt);  // int a = 10;
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    scopeTable->addVariable("b", commonFloat);              // float b
    scopeTable->addConstant(commonInt, "10");               // = 10;
    scopeTable->addVariable("c", commonInt);                // int c;   // c1 here
    scopeTable->addVariable("d", commonFloat);              // float d
    SymbolTableEntry* eq_d_b = scopeTable->findEntry("b");  // = b;
    assert(eq_d_b != nullptr);
    assert(eq_d_b != glb_b);

    SymbolTableEntry* c_body_b = scopeTable->findEntry("b");
    entries.push_back(c_body_b);
    SymbolTableEntry* c_body_10 = scopeTable->findEntry(anonToSymTempMap["10"]);
    entries.push_back(c_body_10);
    SymbolTableEntry* c_body_c1 = scopeTable->findEntry("c");
    entries.push_back(c_body_c1);
    SymbolTableEntry* c_body_d = scopeTable->findEntry("d");
    entries.push_back(c_body_d);

    for (auto entry : entries)
    {
        assert(entry != nullptr);
        cout << entry->toString() << " at " << entry << endl;
    }
    entries.clear();

    // scope = c(int, float) body inner
    cout << "\nEntering c(int, float) body inner" << endl;
    scopeTable->enterScope();
    assert(scopeTable->getScopeLevel() >= 2);

    scopeTable->addVariable("c", commonFloat);              // float c;   // c2 here
    scopeTable->addVariable("e", commonInt);                // int e
    SymbolTableEntry* eq_e_z = scopeTable->findEntry("z");  // = z;
    assert(eq_e_z != nullptr);
    assert(eq_e_z == glb_z);

    SymbolTableEntry* c_body_inner_c2 = scopeTable->findEntry("c");
    entries.push_back(c_body_inner_c2);
    SymbolTableEntry* c_body_inner_e = scopeTable->findEntry("e");
    entries.push_back(c_body_inner_e);

    for (auto entry : entries)
    {
        assert(entry != nullptr);
        cout << entry->toString() << " at " << entry << endl;
    }
    entries.clear();

    // scope = c(int, float) body
    cout << "\nExiting c(int, float) body inner" << endl;
    scopeTable->exitScope();
    assert(scopeTable->getScopeLevel() == 2);

    // scope = c(int, float) params
    cout << "\nExiting c(int, float) body" << endl;
    scopeTable->exitScope();
    assert(scopeTable->getScopeLevel() == 1);

    // scope = global
    cout << "\nExiting c(int, float) params" << endl;
    scopeTable->exitScope();
    assert(scopeTable->getScopeLevel() == 0);

    SymbolTableEntry* glb_a2 = scopeTable->findEntry("a");
    entries.push_back(glb_a2);
    SymbolTableEntry* glb_z2 = scopeTable->findEntry("z");
    entries.push_back(glb_z2);
    SymbolTableEntry* glb_b2 = scopeTable->findEntry("b");
    entries.push_back(glb_b2);

    cout << "\nGlobal scope" << endl;

    for (auto entry : entries)
    {
        assert(entry != nullptr);
        cout << entry->toString() << " at " << entry << endl;
    }
    entries.clear();

    assert(glb_a == glb_a2);
    assert(glb_z == glb_z2);
    assert(glb_b == glb_b2);

    cout << "All tests passed" << endl;

    return 0;
}