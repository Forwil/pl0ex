#include "symbol.h"
#include "symbol_table.h"
#include <sstream>
#include <cstring>
#include <cassert>

#define _HAS_ID (s_type == VARIABLE || s_type == ARGUMENT || s_type == CONST || s_type == FUNCTION || s_type == LABEL)
#define _HAS_VALUE (s_type == CONST || s_type == LITERAL)
#define _IS_FUNCTION (s_type == FUNCTION)
#define _IS_ALLOCATED (address != 0)
#define _NOT_NULL(arg) (arg != NULL)

const std::string VALUE_TYPE_MAP[] = 
{
	"VOID",
	"CHAR",
	"INT",
	"FLOAT",
	"STRING",
};

symbol::symbol():enable(true)
{
}

symbol* symbol::literal_symbol(value_type _type, std::string _value)
{
	symbol* s = new symbol();
	s->v_type = _type;
	s->value = _value;
	s->s_type = LITERAL;
	return s;
}
symbol* symbol::label_symbol(std::string _id)
{
	symbol* s = new symbol();
	s->id = _id;
	s->s_type = LABEL;
	return s;
}

symbol* symbol::variable_symbol(value_type _type, std::string _id)
{
	symbol* s = new symbol();
	s->v_type = _type;
	s->id = _id;
	s->s_type = VARIABLE;
	s->size = 4;
	return s;
}

symbol* symbol::argument_symbol(symbol::value_type _type, std::string _id)
{
	symbol* s = new symbol();
	s->v_type = _type;
	s->id = _id;
	s->s_type = ARGUMENT;
	s->size = 4;
	return s;
}

symbol* symbol::const_symbol(value_type _type, std::string _id, std::string _value)
{
	symbol* s = new symbol();
	s->v_type = _type;
	s->id = _id; 
	s->value = _value;
	s->s_type = CONST;
	s->size = 4;
	return s;
}

symbol* symbol::function_symbol(value_type _type, std::string _id, arguments* _args)
{
	symbol* s = new symbol();
	s->v_type = _type;
	s->id = _id;
	s->args = _args;
	s->s_type = FUNCTION;
	s->size = 0;
	return s;
}

std::string symbol::get_id() const 
{
	assert(_HAS_ID);
	return id;
}

std::string symbol::get_value() const
{
	assert(_HAS_VALUE);
	 return value;
}

symbol::symbol_type symbol::get_symbol_type() const
{
	return s_type;
}

symbol::value_type symbol::get_value_type() const 
{
	return v_type;
}

symbol::arguments* symbol::get_arguments() const
{
	assert(_IS_FUNCTION);
	return args;
}

symbol_table* symbol::get_scope() const 
{
	return scope;
}

unsigned int symbol::get_size()
{
	if(size == 0)
		if(s_type == symbol::FUNCTION)
			if(args != NULL)
				for(unsigned int i = 0; i < args->size(); i++)
					size += (*args)[i]->get_size();
	return size;
}

bool symbol::is_enable() const 
{
	return enable;
}

void symbol::set_scope(symbol_table* _scope)
{
	assert(_NOT_NULL(_scope));
	scope = _scope;
}

void symbol::set_enable(bool e)
{
	enable = e;
}

void operator<<(std::ostream& out, const symbol& _symbol)
{
	if(_symbol.s_type == symbol::CONST)
	{
		out << "[COSNT " << VALUE_TYPE_MAP[_symbol.v_type] << " " << _symbol.id << " = " << _symbol.value;
	}
	else if(_symbol.s_type == symbol::VARIABLE)
	{
		out << "[" << VALUE_TYPE_MAP[_symbol.v_type] << " " << _symbol.id;
	}
	else if(_symbol.s_type == symbol::FUNCTION)
	{
		out << "[" << VALUE_TYPE_MAP[_symbol.v_type] << " " << _symbol.id << "(";
		symbol::arguments::iterator iter = _symbol.args->begin();
		out << *iter;
		iter++;
		while(iter != _symbol.args->end())
		{
			out << ", " << *iter;
			iter++;
		}
		out << ")";
	}
	else if(_symbol.s_type == symbol::LABEL)
	{
		out << "[LABEL " << _symbol.id;
	}
	else if(_symbol.s_type == symbol::LITERAL)
	{
		out << "[" << VALUE_TYPE_MAP[_symbol.v_type] << " " << _symbol.value;
	}
	out << "]" << std::endl;
}

#undef _HAS_ID
#undef _HAS_VALUE
#undef _IS_FUNCTION
#undef _IS_ALLOCATED
#undef _NOT_NULL