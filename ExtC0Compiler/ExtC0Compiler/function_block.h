#ifndef _COMPILER_FUNCTION_BLOCK
#define _COMPILER_FUNCTION_BLOCK

#include "quaternion.h"
#include "basic_block.h"
#include <vector>
#include <iostream>
#include <map>

class function_block
{
public:
	friend class dag;
	friend class data_stream;
	friend class nets;
	friend class live_variable;
	friend class reach_definition;
	friend class conflict_graph;

	function_block();

	/*
		向函数块中添加一条四元式
	*/
	void add(quaternion* q);

	std::vector<basic_block*> get_basic_blocks() const;
	std::vector<quaternion*> get_sequential() const;

	/*
		初始化函数块，编号清零等等
	*/
	static void flush();
	
	quaternion* operator[](unsigned int);
	friend void operator<<(std::ostream&, const function_block&);
private:
	std::vector<basic_block*> blocks;
	std::map<symbol*, basic_block*> block_entry;
	std::map<basic_block*, symbol*> block_exit;
	std::vector<basic_block*> return_blocks;
	std::vector<quaternion*> sequential;

	std::map<unsigned int, unsigned int> index_map;

	/*
		与状态有关的一些变量
	*/
	bool new_block;
	bool following_label;
	bool is_end;
	basic_block* former_block;

	unsigned int number;
	static unsigned int instance_number;

	void flow(basic_block*, basic_block*);
	void flow();
	void new_basic_block();
	void map_label_to_block(symbol*, basic_block*);
};

#endif