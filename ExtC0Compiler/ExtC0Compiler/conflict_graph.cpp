#include "conflict_graph.h"
#include "symbol.h"
#include <vector>
#include <cassert>

conflict_graph::conflict_graph(function_block* _block, std::map<symbol*, bit_set>& _use, std::map<symbol*, bit_set>& _def, reach_definition* _reach, nets* _nets, live_variable* _live):block(_block), use(_use), def(_def), reach(_reach), _net(_nets), live(_live)
{
	reference_counting();
	build();
}

void conflict_graph::reference_counting()
{
	std::vector<basic_block*> blocks = block->get_basic_blocks();
	for(unsigned int j = 0; j < blocks.size(); j++)
	{
		std::vector<quaternion*> ins = blocks[j]->get_sequential();
		for(unsigned int k = 0; k < ins.size(); k++)
		{
			std::vector<symbol*> source_args = ins[k]->get_source_args();
			symbol* object_args = ins[k]->get_object_args();
			for(unsigned int l = 0; l < source_args.size(); l++)
			{
				net* a_net = _net->get_net(source_args[l], ins[k]->get_number(), true);
				refs[a_net]++;
			}
			if(object_args != NULL)
			{
				net* a_net = _net->get_net(object_args, ins[k]->get_number(), false);
				refs[a_net]++;
			}
		}
	}
}

void conflict_graph::build()
{
	std::vector<std::pair<symbol*, net*>>& chains = _net->get_nodes();
	adjacency_matrix.resize(chains.size());
	for(unsigned int i = 0; i < chains.size(); i++)
	{
		adjacency_matrix[i].resize(chains.size());
	}

	for(unsigned int i = 0; i < chains.size(); i++)
	{
		net* net1 = chains[i].second;
		for(unsigned int j = i + 1; j < chains.size(); j++)
		{
			net* net2 = chains[j].second;
			bool conflict = false;
			/*
				遍历两条链的各个定义点
			*/
			for(std::vector<chain>::iterator chain1 = net1->begin(); chain1 != net1->end(); chain1++)
			{
				for(std::vector<chain>::iterator chain2 = net2->begin(); chain2 != net2->end(); chain2++)
				{
					if(is_conflict((*block)[chain2->first], i, *chain1) || is_conflict((*block)[chain1->first], j, *chain2))
					{
						conflict = true;
						break;
					}
				}
				if(conflict)break;
			}
			if(conflict)
			{
				adjacency_matrix[i][j] = adjacency_matrix[j][i] = true;
				//std::cout << i << " " << j << std::endl; 
				continue;
			}
		}
	}
}

std::map<net*, unsigned int> conflict_graph::get_coloring(unsigned int reg_num)
{
	if(color_map.size() != 0)
	{
		return color_map;
	}
	else 
	{
		coloring(reg_num);
		return color_map;
	}
}

nets* conflict_graph::get_nets() const
{
	return _net;
}

void conflict_graph::coloring(unsigned int reg_num)
{
	std::vector<std::pair<symbol*, net*>> nodes = _net->get_nodes();
	std::vector<bool> used(nodes.size());
	std::vector<bool> not_use_reg(nodes.size());
	std::vector<unsigned int> close_nodes;

	while(close_nodes.size() < nodes.size())
	{
		unsigned int index = find_degree_less_than(reg_num, used);
		if(index < adjacency_matrix.size())
		{
			
			used[index] = true;
			if(nodes[index].first->get_value_type() == symbol::FLOAT || nodes[index].first->get_scope()->get_name() == "@global")
				not_use_reg[index] = true;
			else
				not_use_reg[index] = false;
			close_nodes.push_back(index);
		}
		else
		{
			unsigned int no_reg = choose_no_reg(nodes, used);
			used[no_reg] = true;
			not_use_reg[no_reg] = true;
			close_nodes.push_back(no_reg);
		}
	}
	for(unsigned int i = close_nodes.size(); i > 0; i--)
	{
		if(not_use_reg[close_nodes[i - 1]])
		{
			color_map[nodes[close_nodes[i - 1]].second] = 0;
			used[close_nodes[i - 1]] = false;
		}
		else
		{
			color_map[nodes[close_nodes[i - 1]].second] = allocate_color(close_nodes[i - 1], reg_num, nodes, used);
		}
	}
}

void conflict_graph::reference_coloring(unsigned int reg_num)
{
	/*std::vector<std::pair<symbol*, net*>> nodes = _net->get_nodes();
	std::vector<bool> used(nodes.size());
	std::vector<bool> not_use_reg(nodes.size());

	while(color_map.size() < nodes.size())
	{
		unsigned int index = color_map.size() + reg_num;
		if(index < adjacency_matrix.size())
		{
			unsigned int no_reg = choose_no_reg(nodes, used);
			used[no_reg] = true;
			color_map[nodes[no_reg].second] = 0;
		}
		else
		{
			unsigned int r = 1;
			for(unsigned int i = 0; i < nodes.size(); i++)
			{
				if(used[i])continue;
				used[i] = true;
				if(nodes[i].first->get_value_type() == symbol::FLOAT)
					color_map[nodes[i].second] = 0;
				else
					color_map[nodes[i].second] = r++;
			}			
		}
	}*/
}

unsigned int conflict_graph::allocate_color(unsigned int index, unsigned int reg_num, std::vector<std::pair<symbol*, net*>>& nodes, std::vector<bool>& used)
{
	std::vector<bool> available_color(reg_num + 1, true);
	for(unsigned int i = 0; i < adjacency_matrix[index].size(); i++)
	{
		if(used[i] && adjacency_matrix[index][i])
			available_color[color_map[nodes[i].second]] = false;
	}
	for(unsigned int i = 1; i < available_color.size(); i++)
		if(available_color[i])return i;
	assert(0);
	return 0;
}

unsigned int conflict_graph::find_degree_less_than(unsigned int reg_num, std::vector<bool>& used)
{
	for(unsigned int i = 0; i < adjacency_matrix.size(); i++)
	{
		if(used[i])continue;
		unsigned int num = 0;
		for(unsigned int j = 0; j < adjacency_matrix[i].size(); j++)
			if(adjacency_matrix[i][j] && !used[j])num++;
		if(num < reg_num)
		{
			return i;
		}
	}
	return adjacency_matrix.size();
}

unsigned int conflict_graph::choose_no_reg(std::vector<std::pair<symbol*, net*>>& nodes, std::vector<bool>& used)
{
	unsigned int times, index;
	bool found = false;
	for(unsigned int i = 0; i < used.size(); i++)
	{
		if(used[i])continue;
		if(nodes[i].first->get_symbol_type() == symbol::FLOAT)
			return i;
		unsigned int ref_times = refs[nodes[i].second];
		if(!found || times > ref_times)
		{
			times = ref_times;
			index = i;
			found = true;
		}
	}
	return index;
}

bool conflict_graph::is_conflict(quaternion* q2, unsigned int net1, chain chain1)
{
	/*
		N1的一个定义点能够到达N2的一个定义点所在基本块的出口
	*/
	if(reach->get_in(q2).find(chain1.first) && live->get_in(q2).find(net1))
	{
		return true;
	}
	else
	{
		return false;
	}
}