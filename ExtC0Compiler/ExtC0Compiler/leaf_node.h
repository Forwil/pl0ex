#ifndef _COMPILER_LEAF_NODE
#define _COMPILER_LEAF_NODE

#include "node.h"
#include "token.h"
#include <iostream>

/*
	语法树的叶节点的基类
	叶节点包含一个token，在后续分析中会使用到
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