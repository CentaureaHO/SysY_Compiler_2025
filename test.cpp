#include <bits/stdc++.h>
#include <memory>
#include <common/type/symtab/symbol_table.h>
#include <parser/scanner.h>
#include <parser/driver.h>
using namespace std;
using namespace Symbol;
using namespace Yacc;

int main(int argc, char* argv[])
{
    istream* inStream  = &cin;
    ostream* outStream = &cout;

    Driver driver(inStream, outStream);
    ASTree* ast = driver.parse();
    if (ast) { ast->printAST(outStream); }

    Type* intTypePtr = TypeSystem::getPointerType(llType);
    cout << intTypePtr->getTypeName() << endl;

    Type* intpArr = TypeSystem::getArrayType(intTypePtr, 10);
    cout << intpArr->getTypeName() << endl;
}