#include "operator_node.h"

void operator_node::visit(global_symbol_table& tables, instructions& ins, error_list& el, token& oper)
{
	oper = leaf_token;
}
