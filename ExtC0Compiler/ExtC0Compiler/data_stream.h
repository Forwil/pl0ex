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
		�洢һ����������Щ��Ԫʽ�б�������
	*/
	std::map<symbol*, bit_set> definition;

	/*
		�洢һ����������Щ��Ԫʽ�б�ʹ����
	*/
	std::map<symbol*, bit_set> use;

	void analyze();
};

#endif