#ifndef _COMPILER_REACH_DEFINITION
#define _COMPILER_REACH_DEFINITION

#include "basic_block.h"
#include "function_block.h"
#include "symbol.h"
#include "instructions.h"
#include "bit_set.h"
#include <map>

/*
	����һ�������飬�����
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
		�洢һ����������Щ��Ԫʽ�б�������
		��symbol����Ԫʽ�ı�ŵ�ӳ��
	*/
	std::map<symbol*, bit_set>& def;

	/*
		�洢һ����������Щ��Ԫʽ�б�ʹ����
		��symbol����Ԫʽ�ı�ŵ�ӳ��
	*/
	std::map<symbol*, bit_set>& use;

	/*
		�������in����
	*/
	std::map<quaternion*, bit_set> q_in_map;
	/*
		�������out����
	*/
	std::map<quaternion*, bit_set> q_out_map;

};

#endif