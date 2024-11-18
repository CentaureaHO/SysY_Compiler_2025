#include <ast/basic_node.h>
#include <ast/statement.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace Semantic;

bool            mainExists = false;
vector<string>  semanticErrMsgs{};
Semantic::Table semTable;

void ASTree::typeCheck()
{
    for (auto& stmt : *stmts) stmt->typeCheck();

    if (!mainExists) semanticErrMsgs.emplace_back("No main function found");
}