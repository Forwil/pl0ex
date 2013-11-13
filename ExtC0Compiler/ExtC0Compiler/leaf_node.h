#ifndef _COMPILER_LEAF_NODE
#define _COMPILER_LEAF_NODE

#include "node.h"
#include "token.h"
#include <iostream>

/*
	�﷨����Ҷ�ڵ�Ļ���
	Ҷ�ڵ����һ��token���ں��������л�ʹ�õ�
*/
class leaf_node:public node
{
public:
	void set_token(token _token);
	token get_token() const;
protected:
	leaf_node(){}
	token leaf_token;
	virtual void to_string(std::ostream&, int) const;
};

#endif