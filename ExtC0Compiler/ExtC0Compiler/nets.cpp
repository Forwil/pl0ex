#include "nets.h"
#include <cassert>

#define _CONSISTENCE_DEF (symbol_line_def_net_map.find(std::pair<symbol*, unsigned int>(arg, index)) == symbol_line_def_net_map.end() || symbol_line_def_net_map[std::pair<symbol*, unsigned int>(arg, index)] == net_index[chains])
#define _CONSISTENCE_USE (symbol_line_use_net_map.find(std::pair<symbol*, unsigned int>(arg, index)) == symbol_line_use_net_map.end() || symbol_line_use_net_map[std::pair<symbol*, unsigned int>(arg, index)] == net_index[chains])

nets::nets(function_block* _block, std::map<symbol*, bit_set>& _use, std::map<symbol*, bit_set>& _def, reach_definition* _reach):block(_block), use(_use), def(_def), reach(_reach)
{
	analyze();
	symbol_net_mapping();
}

std::vector<std::pair<symbol*, net*>> nets::get_nodes() const
{
	return net_nodes;
}

symbol* nets::get_symbol(net* _net)
{
	unsigned int index = net_index[_net];
	return net_nodes[index].first;
}

net* nets::get_net(symbol* s, unsigned int line, bool is_use) const
{
	if(is_use)
	{
		std::map<std::pair<symbol*, unsigned int>, unsigned int>::const_iterator iter = symbol_line_use_net_map.find(std::pair<symbol*, unsigned int>(s, line));
		if(iter != symbol_line_use_net_map.end())
			return net_nodes[iter->second].second;
		else
			return NULL;
	}
	else
	{
		std::map<std::pair<symbol*, unsigned int>, unsigned int>::const_iterator iter = symbol_line_def_net_map.find(std::pair<symbol*, unsigned int>(s, line));
		if(iter != symbol_line_def_net_map.end())
			return net_nodes[iter->second].second;
		else
			return NULL;
	}
}

unsigned int nets::get_net_index(net* _net) const
{
	return net_index.find(_net)->second;
}

bool nets::get_net_index(symbol* s, unsigned int line, bool is_use, unsigned int& index) const
{
	if(is_use)
	{
		std::map<std::pair<symbol*, unsigned int>, unsigned int>::const_iterator iter = symbol_line_use_net_map.find(std::pair<symbol*, unsigned int>(s, line));
		if(iter != symbol_line_use_net_map.end())
			return index = iter->second, true;
		else
			return false;
	}
	else
	{
		std::map<std::pair<symbol*, unsigned int>, unsigned int>::const_iterator iter = symbol_line_def_net_map.find(std::pair<symbol*, unsigned int>(s, line));
		if(iter != symbol_line_def_net_map.end())
			return index = iter->second, true;
		else
			return false;
	}
}

void nets::analyze()
{
	chains chains_map;

	/*
		遍历使用点，该符号的定义点和符号所在的基本块的到达定义的交集是该使用点所属的定义使用链
		在symbol_def_chain_map中存储着不同符号的不同定义的使用
	*/
	for(std::map<symbol*, bit_set>::iterator use_of_symbol = use.begin(); use_of_symbol != use.end(); use_of_symbol++)
	{
		symbol* s = use_of_symbol->first;
		std::vector<unsigned int>& uses = use_of_symbol->second.get_numbers();
		for(std::vector<unsigned int>::iterator symbol_use = uses.begin(); symbol_use != uses.end(); symbol_use++)
		{
			quaternion* q = (*block)[*symbol_use];
			std::vector<unsigned int> symbol_def = (reach->get_in(q) & def[s]).get_numbers();
			for(unsigned int i = 0; i < symbol_def.size(); i++)
			{
				chains_map[symbol_def[i]] += *symbol_use;
			}
		}
	}
	merge_chains(chains_map);
}

void nets::merge_chains(chains& chains_map)
{
	/*
		保存一个网的所有链的使用点的并集
	*/
	std::vector<bit_set> net_use;
	for(chains::iterator chain = chains_map.begin(); chain != chains_map.end(); chain++)
	{
		unsigned int chain_def = chain->first;
		bit_set chain_uses = chain->second;
		symbol* object_args = (*block)[chain_def]->get_object_args();
		bool flag = true;
		for(unsigned int i = 0; i < net_use.size(); i++)
		{
			if(net_nodes[i].first != object_args)continue;
			if(!(net_use[i] & chain_uses).empty())
			{
				net_nodes[i].second->push_back(*chain);
				net_use[i] += chain_uses;
				flag = false;
				break;
			}
		}
		/*
		没有可合并的链，新建一个网
		*/
		if(flag)
		{
			net* new_net = new net();

			new_net->push_back(*chain);
			net_index[new_net] = net_nodes.size();
			net_use.push_back(chain_uses);
			
			std::pair<symbol*, net*> net_pair(object_args, new_net);
			net_nodes.push_back(net_pair);
			symbol_net_map.insert(net_pair);
		}
	}
}

void nets::symbol_net_mapping()
{
	std::vector<quaternion*> ins = block->get_sequential();
	for(unsigned int i = 0; i < ins.size(); i++)
	{
		quaternion* q = ins[i];
		/*
			网的使用点
		*/
		std::vector<symbol*>& source_args = q->get_source_args();
		for(unsigned int j = 0; j < source_args.size(); j++)
		{
			map_a_symbol(source_args[j], q->get_number(), false);
		}

		/*
			网的定义点
		*/
		symbol* object_args = q->get_object_args();
		map_a_symbol(object_args, q->get_number(), true);
	}
}

void nets::map_a_symbol(symbol* arg, unsigned int index, bool is_object)
{
	std::multimap<symbol*, net*>::iterator upper = symbol_net_map.upper_bound(arg);
	std::multimap<symbol*, net*>::iterator lower = symbol_net_map.lower_bound(arg);
	while(upper != lower)
	{
		net* chains = lower->second;
		bool flag = false;
		for(unsigned int i = 0; i < chains->size(); i++)
		{
			//是目标操作数，且是定义点，或者是源操作数，且是使用点
			if(!is_object && (*chains)[i].second.find(index))
			{
				assert(_CONSISTENCE_USE);
				symbol_line_use_net_map[std::pair<symbol*, unsigned int>(arg, index)] = net_index[chains];
				flag = true;
				break;
			}
			else if(is_object && (*chains)[i].first == index)
			{
				assert(_CONSISTENCE_DEF);
				symbol_line_def_net_map[std::pair<symbol*, unsigned int>(arg, index)] = net_index[chains];
				flag = true;
				break;
			}
		}
		if(flag)break;
		lower++;
	}
}