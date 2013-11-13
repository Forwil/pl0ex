#include "dag_node.h"
#include <cassert>

#define _NOT_NULL(arg) (arg != NULL)
#define _IS_OPERATION (sym == NULL)
#define _IS_SYMBOL (leaf)

dag_node::dag_node(quaternion::operation _oper, unsigned int _left, unsigned int _right)
{
	leaf = false;
	two_children = true;
	enabled = true;
	oper = _oper;
	left = _left;
	right = _right;
	sym = NULL;
}

dag_node::dag_node(quaternion::operation _oper, unsigned int _left)
{
	leaf = false;
	two_children = false;
	enabled = true;
	oper = _oper;
	left = _left;
	sym = NULL;
}

dag_node::dag_node(symbol* _sym)
{
	assert(_NOT_NULL(_sym));
	leaf = true;
	two_children = false;
	enabled = true;
	sym = _sym;
}

bool dag_node::is_leaf() const
{
	return leaf;
}

bool dag_node::has_two_children() const 
{
	assert(_IS_OPERATION);
	return two_children;
}

bool dag_node::is_enabled() const
{
	return enabled;
}

void dag_node::set_enable(bool e)
{
	enabled = e;
}

quaternion::operation dag_node::get_operation() const 
{
	assert(_IS_OPERATION);
	return oper;
}

unsigned int dag_node::get_left() const 
{
	assert(_IS_OPERATION);
	return left;
}

unsigned int dag_node::get_right() const 
{
	assert(_IS_OPERATION);
	assert(two_children);
	return right;
}

symbol* dag_node::get_symbol() const 
{
	assert(_IS_SYMBOL);
	return sym;
}