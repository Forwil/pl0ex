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
		合并各条链
		不同的链如果有相同的使用点，则可以合并到同一个网中
	*/
	void merge_chains(chains&);

	/*
		建立变量到网的映射
	*/
	void symbol_net_mapping();

	/*
		将第index条四元式中的arg映射到相应的网上
		假定同一条四元式中的相同符号一定属于同一个网
	*/
	void map_a_symbol(symbol*, unsigned int, bool);

	function_block* block;

	reach_definition* reach;

	/*
		存储一个符号在哪些四元式中被定义了
	*/
	std::map<symbol*, bit_set>& def;

	/*
		存储一个符号在哪些四元式中被使用了
	*/
	std::map<symbol*, bit_set>& use;

	/*
		网
	*/
	std::vector<std::pair<symbol*, net*>> net_nodes;

	/*
		网在net_nodes中的下标
	*/
	std::map<net*, unsigned int> net_index;
	
	/*
		符号和网的多重映射，一个符号可以包含于多个网
	*/
	std::multimap<symbol*, net*> symbol_net_map;

	/*
		存储某一行出现的变量对应哪个网
	*/
	std::map<std::pair<symbol*, unsigned int>, unsigned int> symbol_line_def_net_map;
	std::map<std::pair<symbol*, unsigned int>, unsigned int> symbol_line_use_net_map;
};

#endif