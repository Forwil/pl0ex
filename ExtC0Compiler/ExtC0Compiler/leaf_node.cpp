#include "leaf_node.h"

void leaf_node::set_token(token _token)
{
	leaf_token = _token;
}

token leaf_node::get_token() const 
{
	return leaf_token;
}

void leaf_node::to_string(std::ostream& out, int indent) const
{
	for(int i = 0; i < indent; i++) out << "  ";
	out << this->get_token();
}