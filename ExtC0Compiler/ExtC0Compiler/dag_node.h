#ifndef _COMPILER_DAG_NODE
#define _COMPILER_DAG_NODE

#include "quaternion.h"

class dag_node
{
public:
	dag_node(quaternion::operation, unsigned int, unsigned int);
	dag_node(quaternion::operation, unsigned int);
	dag_node(symbol*);
	bool is_leaf() const;
	bool has_two_children() const;
	bool is_enabled() const;
	void set_enable(bool);
	quaternion::operation get_operation() const;
	unsigned int get_left() const;
	unsigned int get_right() const;
	symbol* get_symbol() const;
private:
	bool leaf;
	bool two_children;
	bool enabled;
	quaternion::operation oper;
	symbol* sym;
	unsigned int left;
	unsigned int right;
};

#endif