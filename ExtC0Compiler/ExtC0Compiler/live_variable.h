#ifndef _COMPILER_LIVE_VARIABLE
#define _COMPILER_LIVE_VARIABLE

#include "instructions.h"
#include "symbol.h"
#include "bit_set.h"
#include "nets.h"
#include "basic_block.h"
#include "function_block.h"

class live_variable
{
public:
	live_variable(function_block*, std::map<symbol*, bit_set>&, std::map<symbol*, bit_set>&, nets*);
	bit_set get_in(quaternion*) const;
	bit_set get_out(quaternion*) const;
private:
	void analyze();
	void eliminate();
	void define_or_use(symbol*, quaternion*, std::set<unsigned int>&, bit_set&, std::map<quaternion*, bit_set>&, bool);

	nets* _net;
	function_block* block;

	/*
		存储一个符号在哪些四元式中被定义了
		是symbol到四元式的编号的映射
	*/
	std::map<symbol*, bit_set>& def;

	/*
		存储一个符号在哪些四元式中被使用了
		是symbol到四元式的编号的映射
	*/
	std::map<symbol*, bit_set>& use;

	/*
		基本块的in集合
	*/
	std::map<quaternion*, bit_set> q_in_map;
	/*
		基本块的out集合
	*/
	std::map<quaternion*, bit_set> q_out_map;
};

#endif