#ifndef _COMPILER_DAG
#define _COMPILER_DAG

#include "dag_node.h"
#include "instructions.h"
#include "global_symbol_table.h"
#include <vector>
#include <set>
#include <map>

class dag
{
public:
	dag(instructions*&);
private:
	//附属在编号为unsigned int节点上的symbol们
	std::map<unsigned int, std::vector<symbol*>> symbols_sharing_operand;

	//保存一个变量的值在哪个节点中保存的信息
	std::map<symbol*, unsigned int> symbols;

	//保存一个操作符在哪个节点中保存
	std::map<quaternion::operation, std::set<unsigned int>> operators;
	
	//节点的表，包括中间节点和叶节点
	std::vector<dag_node*> nodes;

	//节点的父节点的表
	std::map<unsigned int, std::vector<unsigned int>> parents;

	/*
		在第i个oper前执行的赋值操作
	*/
	std::map<unsigned int, std::vector<std::pair<symbol*, unsigned int>>> assign_info;

	std::multimap<symbol*, symbol*> where_origin_is;
	unsigned int last_oper;
	void build(instructions*& ins);
	void export_quaternions(instructions& ins);
	void flush();
	void renew_sharing(symbol*, unsigned int);
	symbol* get_primative_operand(std::map<unsigned int, symbol*>&, unsigned int);
	void update_origin(symbol*, unsigned int);
	void sharing_assign(instructions&, std::map<unsigned int, symbol*>&, unsigned int);
};

#endif