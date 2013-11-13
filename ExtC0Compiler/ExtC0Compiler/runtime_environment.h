#ifndef _COMPILER_RUNTIME_ENVIRONMENT
#define _COMPILER_RUNTIME_ENVIRONMENT

#include <string>
#include <map>
#include "symbol.h"
#include "symbol_table.h"
#include "function_block.h"
#include "conflict_graph.h"
#include "live_variable.h"

class runtime_manager;
enum reg_name;

class runtime_environment
{
public:
	runtime_environment(function_block*, symbol_table*, conflict_graph*, live_variable*);
	reg_name in_register(symbol*, unsigned int, bool) const;
	int get_offset(symbol*);
	int get_local_size() const;
	int get_arg_size() const;
	bool is_alive(symbol*, unsigned int, bool, unsigned int);
private:
	function_block* block;
	live_variable* live;
	nets* _nets;
	
	std::map<net*, reg_name> global_reg_map;
	std::set<symbol*> symbol_reg_set;

	std::map<symbol*, int> offset_map;
	int local_size;
	int arg_size;
	const static int call_offset = 16;

	bool in_register(symbol*) const;
	void allocate_global_register(conflict_graph*);
	void allocate_memory(symbol_table*);
};

#endif