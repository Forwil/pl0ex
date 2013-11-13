#include "identifier_node.h"
#include "error.h"
#include "error_list.h"

void identifier_node::visit(global_symbol_table& tables, instructions& ins, error_list& el, std::string& value)
{
	value = this->leaf_token.get_value();
}

void identifier_node::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& arg)
{
	arg = tables.look_up(leaf_token.get_value());
	if(arg == NULL)
		el.add_error(error::UNDEFINED_IDENTIFIER, leaf_token.get_value(), line);
}