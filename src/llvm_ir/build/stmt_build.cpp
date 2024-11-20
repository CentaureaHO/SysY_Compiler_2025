#include <ast/statement.h>
#include <ast/expression.h>
#include <llvm_ir/ir_builder.h>
#include <common/type/symtab/semantic_table.h>
#include <iostream>
using namespace std;
using namespace LLVMIR;

extern SemanticTable::Table* semTable;

extern IRTable irgen_table;
extern IR      builder;

extern FuncDefInst* cur_func;
extern Type*        ret_type;

extern int cur_label;
extern int loop_start_label;  // continue;
extern int loop_end_label;    // break;

extern int max_label;
extern int max_reg;

void StmtNode::genIRCode() { cerr << "StmtNode genIRCode not implemented" << endl; }

void ::ExprStmt::genIRCode() { cerr << "ExprStmt genIRCode not implemented" << endl; }

void VarDeclStmt::genIRCode()
{
    // cerr << "VarDeclStmt genIRCode not implemented" << endl;
}

void BlockStmt::genIRCode() { cerr << "BlockStmt genIRCode not implemented" << endl; }

void FuncDeclStmt::genIRCode() { cerr << "FuncDeclStmt genIRCode not implemented" << endl; }

void ReturnStmt::genIRCode() { cerr << "ReturnStmt genIRCode not implemented" << endl; }

void WhileStmt::genIRCode() { cerr << "WhileStmt genIRCode not implemented" << endl; }

void IfStmt::genIRCode() { cerr << "IfStmt genIRCode not implemented" << endl; }

void ForStmt::genIRCode() { cerr << "ForStmt genIRCode not implemented" << endl; }

void BreakStmt::genIRCode() { cerr << "BreakStmt genIRCode not implemented" << endl; }

void ContinueStmt::genIRCode() { cerr << "ContinueStmt genIRCode not implemented" << endl; }