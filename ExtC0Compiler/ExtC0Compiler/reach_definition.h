#ifndef _COMPILER_REACH_DEFINITION
#define _COMPILER_REACH_DEFINITION

#include "basic_block.h"
#include "function_block.h"
#include "symbol.h"
#include "instructions.h"
#include "bit_set.h"
#include <map>

/*
	给出一个函数块，构造出
*/
class reach_definition
{
public:
	reach_definition(function_block*, std::map<symbol*, bit_set>&, std::map<symbol*, bit_set>&);
	bit_set get_in(quaternion*) const;
	bit_set get_out(quaternion*) const;
private:
	void analyze();

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