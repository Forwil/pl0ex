#include "symbol_table.h"
#include "utility.h"
#include <vector>
#include <map>
#include <cassert>

#define _NOT_NULL(arg) (arg != NULL)
#define _IS_LITERAL(arg) (utility::is_literal(arg))
#define _IS_VARIABLE(arg) (utility::is_variable(arg))
#define _IS_ARGUMENT(arg) (utility::is_argument(arg))
#define _IS_CONST(arg) (utility::is_const(arg))
#define _IS_FUNCTION(arg) (utility::is_function(arg))

symbol_table::symbol_table(symbol* _scope)
{
	assert(_NOT_NULL(_scope));
	scope = _scope;
	_temp_manager = new temp_variable_manager();
	_label_manager = new label_manager();
}

bool symbol_table::defined(std::string symbol_name) const
{
	return (symbols.find(symbol_name) != symbols.end());
}

bool symbol_table::add_symbol(symbol* symbol_item)
{
	assert(_NOT_NULL(symbol_item));
	assert(!_IS_LITERAL(symbol_item));
	assert(_IS_VARIABLE(symbol_item) || _IS_ARGUMENT(symbol_item) || _IS_CONST(symbol_item) || _IS_FUNCTION(symbol_item));
	std::string id = symbol_item->get_id();
	if(!defined(id))
	{
		symbol_item->set_scope(this);
		symbols[id] = symbol_item;
		sequential.push_back(symbol_item);
		return true;
	}
	else
	{
		return false;
	}
}

bool symbol_table::find(std::string id, symbol*& sym) const
{
	if(defined(id))
	{
		sym = symbols.find(id)->second;
		return true;
	}
	else
		return false;
}

symbol* symbol_table::next_temp(symbol::value_type type)
{
	symbol* s = _temp_manager->next_temp(type);
	add_symbol(s);
	return s;
}

symbol* symbol_table::next_label()
{
	symbol* s = _label_manager->next_label();
	s->set_scope(this);
	//labels.push_back(s);
	return s;
}

void symbol_table::update_reference(symbol* sym, int time)
{
	references[sym] += time;
	if(references[sym] <= 0)
		sym->set_enable(false);
	else
		sym->set_enable(true);
	assert(references[sym]>=0);
}

std::vector<symbol*> symbol_table::get_sequential() const
{
	return sequential;
}

#undef _NOT_NULL
#undef _IS_LITERAL
#undef _IS_VARIABLE
#undef _IS_ARGUMENT
#undef _IS_CONST
#undef _IS_FUNCTION