#include "global_symbol_table.h"
#include "utility.h"
#include <sstream>
#include <cassert>

#define _NOT_NULL(arg) (arg != NULL)
#define _IS_FUNCTION(arg) (utility::is_function(arg))
#define _IS_LITERAL(arg) (utility::is_literal(arg))
#define _DURING_PARSING (stack_symbol_table.size() != 0)

bool global_symbol_table::add_symbol(symbol::value_type type, std::string id, std::string value)
{
	symbol* s = symbol::const_symbol(type, id, value);
	return add(s);
}

bool global_symbol_table::add_symbol(symbol::value_type type, std::string id)
{
	symbol* s = symbol::variable_symbol(type, id);
	return add(s);
}

bool global_symbol_table::add_symbol(symbol::value_type type, std::string id, symbol::arguments* args, symbol*& s)
{
	s = symbol::function_symbol(type, id, args);
	return add(s);
}
//
//bool global_symbol_table::add_symbol(symbol* s)
//{
//	return add(s);
//}

bool global_symbol_table::add_argument(symbol::value_type type, std::string id, symbol::arguments* args)
{
	assert(_NOT_NULL(args));
	symbol* s = symbol::argument_symbol(type, id);
	if(add(s))
	{
		args->push_back(s);
		return true;
	}
	else
		return false;
}

void global_symbol_table::add_literal(symbol*& _literal)
{
	assert(_DURING_PARSING);
	assert(_IS_LITERAL(_literal));
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	switch(_literal->get_value_type())
	{
	case symbol::CHAR:
		{
			std::string value;
			ss << _literal->get_value();
			ss >> value;
			std::map<std::string, symbol*>::iterator iter = char_literals.find(value);
			if(iter != char_literals.end())
			{
				delete _literal;
				_literal = iter->second;
			}
			else
			{
				char_literals[value] = _literal;
			}
		}
		break;
	case symbol::INT:
		{
			int value;
			ss << _literal->get_value();
			ss >> value;
			std::map<int, symbol*>::iterator iter = int_literals.find(value);
			if(iter != int_literals.end())
			{
				delete _literal;
				_literal = iter->second;
			}
			else
			{
				int_literals[value] = _literal;
			}
		}
		break;
	case symbol::FLOAT:
		{
			float value;
			ss << _literal->get_value();
			ss >> value;
			std::map<float, symbol*>::iterator iter = float_literals.find(value);
			if(iter != float_literals.end())
			{
				delete _literal;
				_literal = iter->second;
			}
			else
			{
				float_literals[value] = _literal;
			}
		}
		break;
	case symbol::STRING:
		{
			std::map<std::string, symbol*>::iterator iter = string_literals.find(_literal->get_value());
			if(iter != string_literals.end())
			{
				delete _literal;
				_literal = iter->second;
			}
			else
			{
				string_literals[_literal->get_value()] = _literal;
			}
		}
		break;
	}
}


bool global_symbol_table::add(symbol* s)
{
	assert(_DURING_PARSING);
	return stack_symbol_table.back()->add_symbol(s);
}

void global_symbol_table::new_temp(symbol*& s, symbol::value_type type)
{
	assert(_DURING_PARSING);
	s = stack_symbol_table.back()->next_temp(type);
}

void global_symbol_table::new_label(symbol*& label)
{
	assert(_DURING_PARSING);
	label = stack_symbol_table.back()->next_label();
}

symbol* global_symbol_table::get_scope() const 
{
	assert(_DURING_PARSING);
	return stack_symbol_table.back()->get_scope();
}

symbol_table* global_symbol_table::get_table(symbol* sym) const
{
	assert(_IS_FUNCTION(sym));
	return symbol_tables.find(sym)->second;
}

std::map<symbol*, symbol_table*> global_symbol_table::get_symbol_tables() const
{
	return symbol_tables;
}

symbol* global_symbol_table::look_up(std::string id) const
{
	symbol* result;
	for(std::vector<symbol_table*>::const_reverse_iterator iter = stack_symbol_table.rbegin(); iter != stack_symbol_table.rend(); iter++)
		if((*iter)->find(id, result))
			return result;
	return NULL;
}

void global_symbol_table::new_symbol_table(symbol* scope)
{
	assert(_NOT_NULL(scope));
	symbol_table* table = new symbol_table(scope);
	symbol_tables[scope] = table;
	stack_symbol_table.push_back(table);
}

void global_symbol_table::pop_symbol_table()
{
	assert(_DURING_PARSING);
	stack_symbol_table.pop_back();
}

#undef _NOT_NULL
#undef _IS_FUNCTION
#undef _IS_LITERAL
#undef _DURING_PARSING
