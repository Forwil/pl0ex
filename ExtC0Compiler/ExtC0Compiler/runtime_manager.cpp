#include "runtime_manager.h"
#include "utility.h"
#include <sstream>
#include <cassert>

using namespace std;

#define _SHOULD_NOT_HAPPEN 0

using namespace std;

runtime_manager::runtime_manager(global_symbol_table* _table, instructions* _ins, data_stream* _ds):ins(_ins)
{
	table = _table;
	ds = _ds;
	offset = 0;
	index = 0;
	if(_ds != NULL)
	{
		reg_heap.push_back(EAX);
		reg_heap.push_back(ECX);
		reg_heap.push_back(EDX);
	}
	else
	{
		reg_heap.push_back(EAX);
		reg_heap.push_back(ECX);
		reg_heap.push_back(EDX);
		reg_heap.push_back(EBX);
		reg_heap.push_back(EBP);
		reg_heap.push_back(ESI);
		reg_heap.push_back(EDI);

	}

	std::vector<function_block*> blocks = ins->get_function_blocks();
	for(std::vector<function_block*>::iterator iter = blocks.begin(); iter != blocks.end(); iter++)
	{
		quaternion* q = (*iter)->get_sequential()[0];
		symbol* sym = q->get_arg(0);
		symbol_table* s_table = table->get_table(sym);
		if(ds != NULL)
			runtime_map[s_table] = new runtime_environment(*iter, s_table, ds->get_conflict_graph(*iter), ds->get_live_variable(*iter));
		else
			runtime_map[s_table] = new runtime_environment(*iter, s_table, NULL, NULL);
	}
}

string runtime_manager::get_operand_name(symbol* sym)
{
	stringstream ss(stringstream::in | stringstream::out);

	if(utility::is_label(sym))
	{
		ss <<  "@@" << sym->get_id() << "@" << sym->get_scope()->get_name();
	}
	else if(utility::is_function(sym))
	{
		ss << "$" << sym->get_id();
	}
	else if(utility::is_literal(sym))
	{
		ss << "$$" << get_literal(sym);
	}
	else if(utility::is_global(sym))
	{
		ss << "$" << sym->get_id() << "@global";
	}
	else if(utility::is_const(sym))
	{
		ss << "$" << sym->get_id() << "@" << sym->get_scope()->get_name();
	}
	else
	{
		assert(0);
	}
	return ss.str();
}

string runtime_manager::get_operand(symbol* sym, unsigned int line, bool is_use)
{
	reg_name reg = in_register(sym, line, is_use);
	if(reg != NA)
	{
		return get_operand(reg);
	}
	else
	{
		return get_address(sym);
	}
}

string runtime_manager::get_operand(reg_name reg)
{
	switch(reg)
	{
	case AH:
		return "AH";
	case AX:
		return "AX";
	case EAX:
		recently_used(reg);
		return "EAX";
	case ECX:
		recently_used(reg);
		return "ECX";
	case EDX:
		recently_used(reg);
		return "EDX";
	case EBX:
		return "EBX";
	case EBP:
		return "EBP";
	case ESI:
		return "ESI";
	case EDI:
		return "EDI";
	case ESP:
		return "ESP";
	default:
		assert(_SHOULD_NOT_HAPPEN);
		return "";
	}
}

string runtime_manager::get_address(symbol* sym)
{
	stringstream ss(stringstream::in | stringstream::out);

	if(utility::is_literal(sym))
	{
		if(const_pool.find(sym) == const_pool.end())
		{
			const_pool.insert(sym);
			literal_map[sym] = index++;
			literal_value_map[sym] = utility::to_value_string(sym);
		}
		ss << "$$" << get_literal(sym);
	}
	else if(utility::is_global(sym))
	{
		if(utility::is_const(sym) && const_pool.find(sym) == const_pool.end())
		{
			const_pool.insert(sym);
			literal_value_map[sym] = utility::to_value_string(sym);
		}
		else if(utility::is_variable(sym) && global_variable.find(sym) == global_variable.end())
		{
			global_variable.insert(sym);
		}
		ss << "dword ptr $" << sym->get_id() << "@global";
	}
	else if(utility::is_const(sym))
	{
		if(const_pool.find(sym) == const_pool.end())
		{
			const_pool.insert(sym);
			literal_value_map[sym] = utility::to_value_string(sym);
		}
		ss << "dword ptr $" << sym->get_id() << "@" << sym->get_scope()->get_name();
	}
	else
	{
		symbol_table* scope = sym->get_scope();
		int address = runtime_map[scope]->get_offset(sym) + offset;
		if(address > 0)
			ss << "dword ptr [esp] - " << address;
		else if(address < 0)
			ss << "dword ptr [esp] + " << -address;
		else
			ss << "dword ptr [esp]";
	}
	return ss.str();
}

std::set<symbol*> runtime_manager::get_global_variable() const
{
	return global_variable;
}

std::set<symbol*> runtime_manager::get_const_pool() const
{
	return const_pool;
}

int runtime_manager::get_local_space(symbol* sym)
{
	assert(utility::is_function(sym));
	symbol_table* scope = table->get_table(sym);
	return runtime_map[scope]->get_local_size();
}

int runtime_manager::get_args_space(symbol* sym)
{
	assert(utility::is_function(sym));
	symbol_table* scope = table->get_table(sym);
	return runtime_map[scope]->get_arg_size();
}

bool runtime_manager::is_alive(symbol* sym, unsigned int line, bool is_use, unsigned int in_line)
{
	assert(!utility::is_global(sym));
	return runtime_map[sym->get_scope()]->is_alive(sym, line, is_use, in_line);
}

string runtime_manager::get_literal_value(symbol* sym)
{
	return literal_value_map[sym];
}

void runtime_manager::increase_offset(int value)
{
	offset -= value;
}

symbol_net runtime_manager::get_symbol_in_reg(reg_name reg)
{
	return reg_variable_map[reg];
}

void runtime_manager::occupy_register(reg_name reg)
{
	if(reg_variable_map.find(reg) != reg_variable_map.end())
	{
		symbol_net prev = reg_variable_map[reg];
		if(prev.first != NULL)
			local_reg_map[prev.first] = NA;
	}
	reg_variable_map[reg].first = NULL;
	reg_status[reg] = OCCUPIED;
}

void runtime_manager::occupy_register(reg_name reg, symbol* sym, unsigned int line, bool is_use)
{
	if(reg_variable_map.find(reg) != reg_variable_map.end())
	{
		symbol_net prev = reg_variable_map[reg];
		if(prev.first != NULL)
			local_reg_map[prev.first] = NA;
	}
	if(local_reg_map[sym] == NA)
	{
		local_reg_map[sym] = reg;
		reg_variable_map[reg] = symbol_net(sym, std::pair<unsigned int, bool>(line, is_use));
		reg_status[reg] = OCCUPIED;
	}
	else
	{
		release_register(local_reg_map[sym]);
	}
}

void runtime_manager::release_register(reg_name reg)
{
	recently_released(reg);
	if(reg_variable_map.find(reg) != reg_variable_map.end())
	{
		symbol_net prev = reg_variable_map[reg];
		if(prev.first != NULL)
			local_reg_map[prev.first] = NA;
	}
	reg_variable_map[reg].first = NULL;
	reg_status[reg] = IDLE;
}

reg_name runtime_manager::allocate(symbol* sym)
{
	reg_name reg = in_local_register(sym);
	if(reg == NA)
	{
		reg = reg_heap.front();
	}
	return reg;
}

reg_name runtime_manager::in_register(symbol* sym, unsigned int line, bool is_use)
{
	reg_name reg = in_local_register(sym);
	if(reg != NA)
	{
		return reg;
	}
	else
	{
		if(utility::is_literal(sym) || utility::is_global(sym))
		{
			return NA;
		}
		else
		{
			symbol_table* scope = sym->get_scope();
			return runtime_map[scope]->in_register(sym, line, is_use);
		}
	}
}

reg_name runtime_manager::in_local_register(symbol* sym)
{
	if((local_reg_map.find(sym) != local_reg_map.end()))
		return local_reg_map[sym];
	else
		return NA;
}


string runtime_manager::get_literal(symbol* sym)
{
	stringstream ss(stringstream::in | stringstream::out);
	switch(sym->get_value_type())
	{
	case symbol::CHAR:
		ss << "char$";
		break;
	case symbol::INT:
		ss << "int$";
		break;
	case symbol::FLOAT:
		ss << "float$";
		break;
	case symbol::STRING:
		ss << "string$";
		break;
	}
	ss << literal_map[sym];
	return ss.str();
}

void runtime_manager::recently_used(reg_name reg)
{
	for(deque<reg_name>::iterator iter = reg_heap.begin(); iter != reg_heap.end(); iter++)
	{
		if(*iter == reg)
		{
			reg_heap.erase(iter);
			reg_heap.push_back(reg);
			break;
		}
	}
}

void runtime_manager::recently_released(reg_name reg)
{
	for(deque<reg_name>::iterator iter = reg_heap.begin(); iter != reg_heap.end(); iter++)
	{
		if(*iter == reg)
		{
			reg_heap.erase(iter);
			reg_heap.push_front(reg);
			break;
		}
	}
}
