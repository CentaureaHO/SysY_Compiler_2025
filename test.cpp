#include <bits/stdc++.h>
#include <memory>
#include <common/type/ast/symbol_table.h>
using namespace std;
using namespace Symbol;

int main()
{
    Table table;
    table.setAsGlobal();

    Entry* glb_a = Entry::getEntry("a");  // voidType
    Entry* glb_b = Entry::getEntry("b");  // intType
    Entry* glb_c = Entry::getEntry("c");  // floatType

    Entry* param_a = Entry::getEntry("a");  // floatType
    Entry* param_b = Entry::getEntry("b");  // floatType

    Entry* loc_a = Entry::getEntry("a");      // intType
    Entry* loc_b = Entry::getEntry("loc_b");  // voidType
    Entry* loc_c = Entry::getEntry("c");      // intType

    VarAttribute* glb_a_attr = new VarAttribute(voidType);
    int           ret        = table.addSymbol(glb_a, glb_a_attr);
    cout << ret << endl;
    VarAttribute* glb_b_attr = new VarAttribute(intType);
    ret                      = table.addSymbol(glb_b, glb_b_attr);
    cout << ret << endl;
    VarAttribute* glb_c_attr = new VarAttribute(floatType);
    ret                      = table.addSymbol(glb_c, glb_c_attr);
    cout << ret << endl;

    table.enterScope();

    VarAttribute* param_a_attr = new VarAttribute(floatType);
    ret                        = table.addSymbol(param_a, param_a_attr);
    cout << ret << endl;
    VarAttribute* param_b_attr = new VarAttribute(floatType);
    ret                        = table.addSymbol(param_b, param_b_attr);
    cout << ret << endl;

    table.enterScope();
    VarAttribute* loc_a_attr = new VarAttribute(intType);
    ret                      = table.addSymbol(loc_a, loc_a_attr);
    cout << ret << endl;
    VarAttribute* loc_b_attr = new VarAttribute(voidType);
    ret                      = table.addSymbol(loc_b, loc_b_attr);
    cout << ret << endl;
    VarAttribute* loc_c_attr = new VarAttribute(intType);
    ret                      = table.addSymbol(loc_c, loc_c_attr);
    cout << ret << endl;

    cout << table.getSymbol(loc_b)->getType()->getTypeName() << endl;
}