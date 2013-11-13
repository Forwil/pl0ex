#include "data_stream.h"
#include "reach_definition.h"
#include "nets.h"
#include "live_variable.h"
#include "utility.h"
#include <cassert>

data_stream::data_stream(instructions* _inst):inst(_inst)
{
	analyze();
}

live_variable* data_stream::get_live_variable(function_block* block) const
{
	if(live.find(block) != live.end())
		return live.find(block)->second;
	else
	{
		assert(0);
		return NULL;
	}
}

conflict_graph* data_stream::get_conflict_graph(function_block* block) const
{
	if(graph.find(block) != graph.end())
		return graph.find(block)->second;
	else
	{
		assert(0);
		return NULL;
	}
}

void data_stream::analyze()
{
	std::vector<quaternion*> ins = inst->get_sequential();
	for(unsigned int i = 0; i < ins.size(); i++)
	{
		quaternion* q = ins[i];
		symbol* object_args = q->get_object_args();
		
		if(object_args != NULL)
			if((utility::is_variable(object_args) || utility::is_argument(object_args)) && !utility::is_global(object_args))
				definition[object_args] += i;
		
		std::vector<symbol*>& source_args = q->get_source_args();
		for(unsigned int j = 0; j < source_args.size(); j++)
		{
			if((utility::is_variable(source_args[j]) || utility::is_argument(source_args[j])) && !utility::is_global(source_args[j]))
				use[source_args[j]] += i;
		}
	}

	std::vector<function_block*> blocks = inst->get_function_blocks();
	for(unsigned int i = 0; i < blocks.size(); i++)
	{
		reach_definition* reach = new reach_definition(blocks[i], use, definition);
		nets* net = new nets(blocks[i], use, definition, reach);
		live_variable* live = new live_variable(blocks[i], use, definition, net);
		conflict_graph* graph = new conflict_graph(blocks[i], use, definition, reach, net, live);
		this->live[blocks[i]] = live;
		this->graph[blocks[i]] = graph;
	}
}
