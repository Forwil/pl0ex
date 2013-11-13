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
		����id���ַ���ʽ
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, std::string&);
	
	/*
		�ӷ��ű��в��Ҳ����ظ÷��ţ��������ڸ÷��ţ����������δ����Ĵ���
	*/
	virtual	void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
protected:
	identifier_node(){}
};

#endif
