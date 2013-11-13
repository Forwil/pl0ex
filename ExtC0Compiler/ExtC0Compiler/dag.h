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
	//�����ڱ��Ϊunsigned int�ڵ��ϵ�symbol��
	std::map<unsigned int, std::vector<symbol*>> symbols_sharing_operand;

	//����һ��������ֵ���ĸ��ڵ��б������Ϣ
	std::map<symbol*, unsigned int> symbols;

	//����һ�����������ĸ��ڵ��б���
	std::map<quaternion::operation, std::set<unsigned int>> operators;
	
	//�ڵ�ı������м�ڵ��Ҷ�ڵ�
	std::vector<dag_node*> nodes;

	//�ڵ�ĸ��ڵ�ı�
	std::map<unsigned int, std::vector<unsigned int>> parents;

	/*
		�ڵ�i��operǰִ�еĸ�ֵ����
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