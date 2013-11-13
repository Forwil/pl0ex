#ifndef _COMPILER_TYPE_NODE
#define _COMPILER_TYPE_NODE
#include "leaf_node.h"
#include "instructions.h"
#include "global_symbol_table.h"
#include "symbol.h"
class type_node:public leaf_node
{
public:
	/*
		返回类型标识符
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, symbol::value_type&);
protected:
	type_node(){}
};

#endif