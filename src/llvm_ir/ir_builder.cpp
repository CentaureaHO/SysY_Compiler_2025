#include <llvm_ir/ir_builder.h>
#include <llvm_ir/defs.h>
#include <common/type/symtab/semantic_table.h>
#include <common/type/symtab/symbol_table.h>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

IRTable::IRTable() : symTab(nullptr), regMap({}), formalArrTab({}) {}

IR::IR() : global_def({}), function_declare({}), function_block_map({}) {}
IR::~IR()
{
    for (auto& inst : global_def)
    {
        delete inst;
        inst = nullptr;
    }
    for (auto& inst : function_declare)
    {
        delete inst;
        inst = nullptr;
    }
    for (auto it = function_block_map.begin(); it != function_block_map.end(); ++it)
    {
        FuncDefInst* func   = it->first;
        auto&        blocks = it->second;
        delete func;
        func = nullptr;
        for (auto& [label, block] : blocks)
        {
            delete block;
            block = nullptr;
        }
    }
}

void IR::registerLibraryFunctions()
{
    static bool registered = false;
    if (registered) return;

    // int getint();
    function_declare.emplace_back(new FuncDeclareInst(DT::I32, "getint"));

    // int getch();
    function_declare.emplace_back(new FuncDeclareInst(DT::I32, "getch"));

    // int getarray(int a[]);
    function_declare.emplace_back(new FuncDeclareInst(DT::I32, "getarray", {DT::PTR}));

    // float getfloat();
    function_declare.emplace_back(new FuncDeclareInst(DT::F32, "getfloat"));

    // int getfarray(float a[]);
    function_declare.emplace_back(new FuncDeclareInst(DT::I32, "getfarray", {DT::PTR}));

    // void putint(int a);
    function_declare.emplace_back(new FuncDeclareInst(DT::VOID, "putint", {DT::I32}));

    // void putch(int a);
    function_declare.emplace_back(new FuncDeclareInst(DT::VOID, "putch", {DT::I32}));

    // void putarray(int n, int a[]);
    function_declare.emplace_back(new FuncDeclareInst(DT::VOID, "putarray", {DT::I32, DT::PTR}));

    // void putfloat(float a);
    function_declare.emplace_back(new FuncDeclareInst(DT::VOID, "putfloat", {DT::F32}));

    // void putfarray(int n, float a[]);
    function_declare.emplace_back(new FuncDeclareInst(DT::VOID, "putfarray", {DT::I32, DT::PTR}));

    // llvm memset
    function_declare.emplace_back(
        new FuncDeclareInst(DT::VOID, "llvm.memset.p0.i32", {DT::PTR, DT::I8, DT::I32, DT::I1}));

    registered = true;
}

void IR::enterFunc(FuncDefInst* func) { function_block_map[func] = {}; }

IRBlock* IR::createBlock(FuncDefInst* func, int label)
{
    auto block                      = new IRBlock(label);
    function_block_map[func][label] = block;
    return block;
}
IRBlock* IR::getBlock(FuncDefInst* func, int label) { return function_block_map[func][label]; }

void IR::printIR(ostream& s)
{
    for (auto& inst : function_declare) inst->printIR(s);

    s << "\n";

    for (auto& inst : global_def) inst->printIR(s);

    s << "\n";

    for (auto it = function_block_map.begin(); it != function_block_map.end(); ++it)
    {
        auto& [func, blocks] = *it;
        func->printIR(s);

        s << "{\n";
        for (auto& [label, block] : blocks) block->printIR(s);
        s << "}\n";

        if (next(it) != function_block_map.end()) s << "\n";
    }
}