#ifndef _COMPILER_NETS
#define _COMPILER_NETS

#include "basic_block.h"
#include "function_block.h"
#include "instructions.h"
#include "bit_set.h"
#include "reach_definition.h"
#include "symbol.h"

typedef std::map<unsigned int, bit_set> chains;
typedef std::pair<unsigned int, bit_set> chain;
typedef std::vector<chain> net;

class nets
{
public:
	nets(function_block*, std::map<symbol*, bit_set>&, std::map<symbol*, bit_set>&, reach_definition*);
	std::vector<std::pair<symbol*, net*>> get_nodes() const;
	symbol* get_symbol(net*);
	net* get_net(symbol*, unsigned int, bool) const;
	unsigned int get_net_index(net*) const;
	bool get_net_index(symbol*, unsigned int, bool, unsigned int&) const;
private:
	void analyze();

	/*
		�ϲ�������
		��ͬ�����������ͬ��ʹ�õ㣬����Ժϲ���ͬһ������
	*/
	void merge_chains(chains&);

	/*
		��������������ӳ��
	*/
	void symbol_net_mapping();

	/*
		����index����Ԫʽ�е�argӳ�䵽��Ӧ������
		�ٶ�ͬһ����Ԫʽ�е���ͬ����һ������ͬһ����
	*/
	void map_a_symbol(symbol*, unsigned int, bool);

	function_block* block;

	reach_definition* reach;

	/*
		�洢һ����������Щ��Ԫʽ�б�������
	*/
	std::map<symbol*, bit_set>& def;

	/*
		�洢һ����������Щ��Ԫʽ�б�ʹ����
	*/
	std::map<symbol*, bit_set>& use;

	/*
		��
	*/
	std::vector<std::pair<symbol*, net*>> net_nodes;

	/*
		����net_nodes�е��±�
	*/
	std::map<net*, unsigned int> net_index;
	
	/*
		���ź����Ķ���ӳ�䣬һ�����ſ��԰����ڶ����
	*/
	std::multimap<symbol*, net*> symbol_net_map;

	/*
		�洢ĳһ�г��ֵı�����Ӧ�ĸ���
	*/
	std::map<std::pair<symbol*, unsigned int>, unsigned int> symbol_line_def_net_map;
	std::map<std::pair<symbol*, unsigned int>, unsigned int> symbol_line_use_net_map;
};

#endif