#ifndef _COMPILER_IDENTIFIER_NODE
#define _COMPILER_IDENTIFIER_NODE
#include "leaf_node.h"
#include "instructions.h"
#include "global_symbol_table.h"
#include "error_list.h"
#include "symbol.h"
#include <string>

class identifier_node:public leaf_node
{
public:
	/*
		返回id的字符形式
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, std::string&);
	
	/*
		从符号表中查找并返回该符号，若不存在该符号，则产生符号未定义的错误
	*/
	virtual	void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
protected:
	identifier_node(){}
};

#endif
