#include "runtime_environment.h"
#include "runtime_manager.h"
#include "utility.h"
#include <cassert>
#include <iostream>

runtime_environment::runtime_environment(function_block* _block, symbol_table* table, conflict_graph* graph, live_variable* _live):block(_block), live(_live)
{
	if(graph != NULL)
	{
		_nets = graph->get_nets();
		allocate_global_register(graph);
	}
	allocate_memory(table);
}

reg_name runtime_environment::in_register(symbol* sym, unsigned int line, bool is_use) const
{
	if(global_reg_map.size() == 0)
		return NA;
	else
	{
		net* _net = _nets->get_net(sym, line, is_use);
		if(global_reg_map.find(_net) != global_reg_map.end())
		{
			return global_reg_map.find(_net)->second;
		}
		else 
		{
			return NA;
		}
	}
}

int runtime_environment::get_offset(symbol* sym)
{
	if(utility::is_argument(sym))
		return -offset_map[sym] - local_size - call_offset;
	else
		return offset_map[sym] - local_size;
}

int runtime_environment::get_local_size() const
{
	return local_size;
}

int runtime_environment::get_arg_size() const
{
	return arg_size;
}

bool runtime_environment::is_alive(symbol* sym, unsigned int line, bool is_use, unsigned int in_line)
{
	unsigned int index;
	if(_nets->get_net_index(sym, line, is_use, index))
	{
		return live->get_in((*block)[in_line]).find(index);
	}
	else
	{
		assert(0);
		return false;
	}
}

bool runtime_environment::in_register(symbol* sym) const
{
	if(global_reg_map.size() == 0)
		return false;
	else
		return symbol_reg_set.find(sym) == symbol_reg_set.end();
}

void runtime_environment::allocate_global_register(conflict_graph* graph)
{
	std::map<net*, unsigned int> coloring = graph->get_coloring(4);
	for(std::map<net*, unsigned int>::iterator iter = coloring.begin(); iter != coloring.end(); iter++)
	{
		switch(iter->second)
		{
		case 0:
			global_reg_map[iter->first] = NA;
			symbol_reg_set.insert(_nets->get_symbol(iter->first));
			break;
		case 1:
			global_reg_map[iter->first] = EBX;
			break;
		case 2:
			global_reg_map[iter->first] = EBP;
			break;
		case 3:
			global_reg_map[iter->first] = ESI;
			break;
		case 4:
			global_reg_map[iter->first] = EDI;
			break;
		default:
			assert(0);
		}
	}
}

void runtime_environment::allocate_memory(symbol_table* table)
{
	local_size = 0;
	arg_size = 0;
	std::vector<symbol*> sequential = table->get_sequential();
	for(unsigned int i = 0; i < sequential.size(); i++)
	{
		if(utility::is_argument(sequential[i]))
		{
			arg_size += sequential[i]->get_size();
			offset_map[sequential[i]] = arg_size;
		}
		else if(utility::is_variable(sequential[i]) && !in_register(sequential[i]) && sequential[i]->is_enable())
		{
			local_size += sequential[i]->get_size();
			offset_map[sequential[i]] = local_size;
		}
	}
}
