#ifndef _COMPILER_CONFLICT_GRAPH
#define _COMPILER_CONFLICT_GRAPH

#include <vector>
#include "reach_definition.h"
#include "nets.h"
#include "instructions.h"
#include "live_variable.h"

class conflict_graph
{
public:
	conflict_graph(function_block*, std::map<symbol*, bit_set>&, std::map<symbol*, bit_set>&, reach_definition*, nets*, live_variable*);
	std::map<net*, unsigned int> get_coloring(unsigned int);
	nets* get_nets() const;
private:
	std::map<net*, unsigned int> color_map;
	std::map<net*, unsigned int> refs;

	function_block* block;
	std::map<symbol*, bit_set>& def;
	std::map<symbol*, bit_set>& use;
	reach_definition* reach;
	nets* _net;
	live_variable* live;
	
	bool flag;

	std::vector<std::vector<bool>> adjacency_matrix;

	void reference_counting();
	void build();
	void coloring(unsigned int);
	void reference_coloring(unsigned int);
	unsigned int find_degree_less_than(unsigned int, std::vector<bool>&);
	unsigned int choose_no_reg(std::vector<std::pair<symbol*, net*>>&, std::vector<bool>&);
	unsigned int allocate_color(unsigned int, unsigned int, std::vector<std::pair<symbol*, net*>>&, std::vector<bool>&);
	bool is_conflict(quaternion*, unsigned int, chain);
};

#endif