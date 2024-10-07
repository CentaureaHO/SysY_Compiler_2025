#include <bits/stdc++.h>
#include <defs/type.h>
#include <symbols/symbolTable.h>
using namespace std;

inline string FN(const string& a, const vector<Type*>& b) { return SymbolTable::getFuncname(a, b); }