#include <ast/helper.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace Semantic;

extern vector<string>  semanticErrMsgs;
extern Semantic::Table semTable;

void HelperNode::typeCheck() {}

void InitNode::typeCheck() {}

void InitSingle::typeCheck() {}

void InitMulti::typeCheck() {}

void DefNode::typeCheck() {}

void FuncParamDefNode::typeCheck() {}