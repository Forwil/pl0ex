#ifndef _COMPILER_OPERATOR_NODE
#define _COMPILER_OPERATOR_NODE
#include "leaf_node.h"
#include "token.h"
#include "instructions.h"
#include "global_symbol_table.h"
#include "symbol.h"

class operator_node:public leaf_node
{
public:

	/*
		·µ»Ø²Ù×÷·ûµÄtoken
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, token&);
protected:
	operator_node(){}
};

#endif