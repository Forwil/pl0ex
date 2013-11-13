#ifndef _COMPILER_DATA_STREAM
#define _COMPILER_DATA_STREAM

#include <map>
#include <vector>
#include <set>
#include "bit_set.h"
#include "instructions.h"
#include "basic_block.h"
#include "quaternion.h"
#include "conflict_graph.h"

class data_stream
{
public:
	data_stream(instructions*);
    live_variable* get_live_variable(function_block*) const;
	conflict_graph* get_conflict_graph(function_block*) const;
private:
	instructions* inst;
	std::map<function_block*, live_variable*> live; 
	std::map<function_block*, conflict_graph*> graph;

	/*
		存储一个符号在哪些四元式中被定义了
	*/
	std::map<symbol*, bit_set> definition;

	/*
		存储一个符号在哪些四元式中被使用了
	*/
	std::map<symbol*, bit_set> use;

	void analyze();
};

#endif