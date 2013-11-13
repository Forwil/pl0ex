#include "reach_definition.h"

namespace
{
	bool is_function(quaternion* q)
	{
		return q->get_operation() == quaternion::FUN;
	}
}

reach_definition::reach_definition(function_block* _block, std::map<symbol*, bit_set>& _use, std::map<symbol*, bit_set>& _def):block(_block), use(_use), def(_def)
{
	analyze();
}

bit_set reach_definition::get_in(quaternion* q) const
{
	if(q_in_map.find(q) != q_in_map.end())
		return q_in_map.find(q)->second;
	else
		return bit_set();
}

bit_set reach_definition::get_out(quaternion* q) const
{
	if(q_out_map.find(q) != q_out_map.end())
		return q_out_map.find(q)->second;
	else
		return bit_set();
}

void reach_definition::analyze()
{
	std::map<basic_block*, bit_set> in_map;
	std::map<basic_block*, bit_set> out_map;
	std::map<basic_block*, bit_set> gen_map;
	std::map<basic_block*, bit_set> kill_map;
	std::map<quaternion*, bit_set> q_gen_map;
	std::map<quaternion*, bit_set> q_kill_map;
	/*
		初始化gen和kill集合
	*/
	std::vector<basic_block*> blocks = block->get_basic_blocks();
	for(unsigned int i = 0; i < blocks.size(); i++)
	{
		basic_block* b_block = blocks[i];
		bit_set gen;
		bit_set kill;
		std::vector<quaternion*> ins = b_block->get_sequential();
		for(unsigned int j = 0; j < ins.size(); j++)
		{
			quaternion* q = ins[j];
			unsigned int index = q->get_number();
			symbol* object_args = q->get_object_args();
			if(object_args != NULL)
			{
				gen += index;
				kill += (def[object_args] - index);
				q_gen_map[q] += index;
				q_kill_map[q] = (def[object_args] - index);
			}
		}
		gen_map[b_block] = gen;
		kill_map[b_block] = kill;
		out_map[b_block] = gen - kill;
	}
	/*
		迭代直到任何一个基本块的in集合不再发生变化为止
	*/
	while(1)
	{
		bool flag = false;
		std::vector<basic_block*> blocks = block->get_basic_blocks();
		for(unsigned int i = 0; i < blocks.size(); i++)
		{
			basic_block* b_block = blocks[i];
			bit_set in, temp;

			for(unsigned int j = 0; j < b_block->entry.size(); j++)
			{
				in += out_map[b_block->entry[j]];
			}
			temp = in;
			std::vector<quaternion*> ins = b_block->get_sequential();
			for(unsigned int j = 0; j < ins.size(); j++)
			{
				q_in_map[ins[j]] = temp;
				temp = q_out_map[ins[j]] += q_gen_map[ins[j]] + (temp - q_kill_map[ins[j]]);
			}
			if(in_map[b_block] != in)
			{
				flag = true;
				in_map[b_block] = in;
				out_map[b_block] += gen_map[b_block] + (in - kill_map[b_block]);
			}
		}
		if(!flag)
		{
			//for(unsigned int i = 0; i < block->sequential.size(); i++)
			//{
			//	std::cout << q_in_map[block->sequential[i]];
			//	std::cout << q_out_map[block->sequential[i]];
			//	std::cout << *block->sequential[i];
			//}
			break;
		}
	}
}