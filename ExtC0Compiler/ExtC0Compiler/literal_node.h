#ifndef _COMPILER_LITERAL_NODE
#define _COMPILER_LITERAL_NODE
#include "leaf_node.h"
#include "instructions.h"
#include "global_symbol_table.h"
#include "symbol.h"

class literal_node: public leaf_node
{
public:
	/*
		��symbol��װһ������ֵ
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
protected:
	literal_node() {};
};

#endif
