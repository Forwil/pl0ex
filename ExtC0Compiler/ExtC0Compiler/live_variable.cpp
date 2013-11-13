#include "live_variable.h"

live_variable::live_variable(function_block* _block, std::map<symbol*,bit_set>& _use, std::map<symbol*,bit_set>& _def, nets* _nets):block(_block), use(_use), def(_def), _net(_nets)
{
	analyze();
	eliminate();
}

void live_variable::analyze()
{
	std::map<basic_block*, bit_set> in_map;
	std::map<basic_block*, bit_set> out_map;
	std::map<basic_block*, bit_set> def_map;
	std::map<basic_block*, bit_set> use_map;
	std::map<quaternion*, bit_set> q_def_map;
	std::map<quaternion*, bit_set> q_use_map;
	/*
		初始化def和use集合
	*/
	std::vector<basic_block*> blocks = block->get_basic_blocks();
	for(unsigned int i = 0; i < blocks.size(); i++)
	{
		basic_block* b_block = blocks[i];
		bit_set def;
		bit_set use;
		std::set<unsigned int> defined;
		std::vector<quaternion*> ins = b_block->get_sequential();
		for(unsigned int j = 0; j < ins.size(); j++)
		{
			quaternion* q = ins[j];
			std::vector<symbol*>& source_args = q->get_source_args();
			for(unsigned int k = 0; k < source_args.size(); k++)
			{
				define_or_use(source_args[k], q, defined, use, q_use_map, true);
			}
			symbol* object_args = q->get_object_args();
			if(object_args != NULL)
			{
				define_or_use(object_args, q, defined, def, q_def_map, false);
			}
		}
		def_map[b_block] = def;
		use_map[b_block] = use;
		in_map[b_block] = use - def;
	}
	/*
		迭代直到任何一个基本块的out集合不再发生变化为止
	*/
	while(1)
	{
		bool flag = false;
		std::vector<basic_block*> blocks = block->get_basic_blocks();
		for(unsigned int i = 0; i < blocks.size(); i++)
		{
			basic_block* b_block = blocks[i];
			bit_set out, temp;
			for(unsigned int j = 0; j < b_block->exit.size(); j++)
			{
				out += in_map[b_block->exit[j]];
			}
			temp = out;
			std::vector<quaternion*> ins = b_block->get_sequential();
			for(unsigned int j = ins.size(); j > 0; j--)
			{
				q_out_map[ins[j - 1]] = temp;
				temp = q_in_map[ins[j - 1]] = q_use_map[ins[j - 1]] + (temp - q_def_map[ins[j - 1]]);
			}
			if(out_map[b_block] != out)
			{
				flag = true;
				out_map[b_block] = out;
				in_map[b_block] += use_map[b_block] + (out - def_map[b_block]);
			}
		}
		if(!flag)
		{
			/*for(unsigned int i = 0; i < block->sequential.size(); i++)
			{
				std::cout << q_in_map[block->sequential[i]];
				std::cout << q_out_map[block->sequential[i]];
				std::cout << *block->sequential[i];
			}*/
			break;
		}
	}
}

void live_variable::eliminate()
{
	std::vector<quaternion*> ins = block->get_sequential();
	for(unsigned int i = 0; i < ins.size(); i++)
	{
		symbol* arg = ins[i]->get_object_args();
		if(arg != NULL)
		{
			if(arg->get_scope()->get_name() == "@global")continue;
			unsigned int index;
			if(_net->get_net_index(arg, ins[i]->get_number(), false, index))
			{
				if(!q_out_map[ins[i]].find(index))
					ins[i]->disable();
			}
			else
			{
				ins[i]->disable();
			}
		}
	}
}

bit_set live_variable::get_in(quaternion* q) const
{
	if(q_in_map.find(q) != q_in_map.end())
		return q_in_map.find(q)->second;
	else
		return bit_set();
}

bit_set live_variable::get_out(quaternion* q) const
{
	if(q_out_map.find(q) != q_out_map.end())
		return q_out_map.find(q)->second;
	else
		return bit_set();
}

void live_variable::define_or_use(symbol* arg, quaternion* q, std::set<unsigned int>& defined, bit_set& _set, std::map<quaternion*, bit_set>& _map, bool is_use)
{
	unsigned int index;
	unsigned int line = q->get_number();
	if(_net->get_net_index(arg, line, is_use, index))
	{
		if(defined.count(index) == 0)
		{
			defined.insert(index);
			_set += index;
		}
		_map[q] += index;
	}
}
