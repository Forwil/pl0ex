#include "type_node.h"
#include <cassert>

#define _SHOULD_NOT_HAPPEN 0

void type_node::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol::value_type& type)
{
	switch(leaf_token.get_type())
	{
	case token::CHAR:
		type = symbol::CHAR;
		break;
	case token::INT:
		type = symbol::INT;
		break;
	case token::FLOAT:
		type = symbol::FLOAT;
		break;
	default:
		assert(_SHOULD_NOT_HAPPEN);
	}
}

#undef _SHOULD_NOT_HAPPEN
