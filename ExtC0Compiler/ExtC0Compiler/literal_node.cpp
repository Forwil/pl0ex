#include "literal_node.h"
#include <cassert>

#define _SHOULD_NOT_HAPPEN 0

void literal_node::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& sym)
{
	switch(leaf_token.get_type())
	{
	case token::CHAR_CONST:
		sym = symbol::literal_symbol(symbol::CHAR, leaf_token.get_value());
		break;
	case token::UNSIGNED_INTEGER_CONST:
	case token::SIGNED_INTEGER_CONST:
		sym = symbol::literal_symbol(symbol::INT, leaf_token.get_value());
		break;
	case token::SIGNED_REAL_CONST:
		sym = symbol::literal_symbol(symbol::FLOAT, leaf_token.get_value());
		break;
	case token::STRING_CONST:
		sym = symbol::literal_symbol(symbol::STRING, leaf_token.get_value());
		break;
	default:
		assert(_SHOULD_NOT_HAPPEN);
	}
	tables.add_literal(sym);
}

#undef _SHOULD_NOT_HAPPEN