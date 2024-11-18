#ifndef __COMMON_TYPE_TYPECALC_H__
#define __COMMON_TYPE_TYPECALC_H__

#include <common/type/type_defs.h>

NodeAttribute Semantic(NodeAttribute a, OpCode op);
NodeAttribute Semantic(NodeAttribute a, NodeAttribute b, OpCode op);

#endif