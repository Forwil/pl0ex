#ifndef _COMPILER_runtime_manager
#define _COMPILER_RESISTER_ALLOCATOR

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include "symbol.h"
#include "symbol_table.h"
#include "global_symbol_table.h"
#include "instructions.h"
#include "function_block.h"
#include "data_stream.h"
#include "runtime_environment.h"

const static enum reg_name
{
	NA = 0,
	//GLOBAL
	EBX,
	EBP,
	ESI,
	EDI,
	//LOCAL
	AH,
	AX,
	EAX,
	ECX,
	EDX,
	//SPECIAL
	ESP,
	//
};

typedef std::pair<symbol*, std::pair<unsigned int, bool>> symbol_net;

class runtime_manager
{
public:
	runtime_manager(global_symbol_table*, instructions*, data_stream*);

	std::string get_operand_name(symbol*);
	std::string get_operand(symbol*, unsigned int, bool);
	std::string get_operand(reg_name);
	std::string get_address(symbol*);

	std::set<symbol*> get_const_pool() const;
	std::set<symbol*> get_global_variable() const;
	int get_local_space(symbol*);
	int get_args_space(symbol*);
	bool is_alive(symbol*, unsigned int, bool, unsigned int);
	std::string get_literal_value(symbol*);
	void increase_offset(int);

	symbol_net get_symbol_in_reg(reg_name);
	void occupy_register(reg_name);
	void occupy_register(reg_name, symbol*, unsigned int, bool);
	void release_register(reg_name);
	reg_name allocate(symbol*);
	reg_name in_register(symbol*, unsigned int, bool);
	reg_name in_local_register(symbol*);
private:
	enum status
	{
		OCCUPIED,
		IDLE,
	};
	global_symbol_table* table;
	instructions* ins;
	data_stream* ds;

	unsigned int index;

	std::map<symbol_table*, runtime_environment*> runtime_map;
	std::map<symbol*, reg_name> local_reg_map;
	std::map<reg_name, symbol_net> reg_variable_map;
	std::map<reg_name, status> reg_status;
	std::map<symbol*, unsigned int> literal_map;
	std::map<symbol*, std::string> literal_value_map;
	std::set<symbol*> const_pool;
	std::set<symbol*> global_variable;
	std::deque<reg_name> reg_heap;
	int offset;

	void recently_released(reg_name);
	void recently_used(reg_name);
	std::string get_literal(symbol*);
	void literal_manager();
};

#endif