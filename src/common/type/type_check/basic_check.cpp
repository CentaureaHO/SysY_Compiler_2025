#include <ast/basic_node.h>
#include <ast/statement.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace SemanticTable;

bool           mainExists = false;
bool           inGlb      = true;
vector<string> semanticErrMsgs{};
extern Table   semTable;

void ASTree::typeCheck()
{
    semTable.symTable.enterScope();  // to 1, params

    for (auto& stmt : *stmts) stmt->typeCheck();

    if (!mainExists) semanticErrMsgs.emplace_back("No main function found");
}