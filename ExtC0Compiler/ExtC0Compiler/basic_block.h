#ifndef _COMPILER_BASIC_BLOCK
#define _COMPILER_BASIC_BLOCK

#include "quaternion.h"
#include <vector>
#include <iostream>

/*
	基本块
	一个基本块有且仅有一个入口和一个出口，其中的指令顺序执行
	入口要么是LABEL，要么是FUN，要么是非以上两种指令的转移指令、RET指令之后的一条指令
	出口要么是转移指令、RET指令，要么是ENDF

	构造流图时，通过转移指令和LABEL之间的映射，建立起部分有向边
	其他情况，流图一定是在相邻的两个基本块儿之间顺序流通的

	如果之前的基本块以JMP或RET终止，则下一个相邻的基本块是否存在前驱不能确定
*/
class basic_block
{
public:
	friend class dag;
	friend class data_stream;
	friend class nets;
	friend class live_variable;
	friend class reach_definition;
	friend class conflict_graph;

	basic_block();
	/*
		向基本块中添加一条四元式
	*/
	void add(quaternion* q);

	std::vector<quaternion*> get_sequential() const;
	/*
		设置基本块的入口
	*/
	void set_entry(basic_block* block);
	/*
		设置基本块的出口
	*/
	void set_exit(basic_block* block);
	/*
		初始化基本块，编号清零等等
	*/
	static void flush();

	quaternion* operator[](unsigned int);
	friend void operator<<(std::ostream&, const basic_block&);
private:
	std::vector<quaternion*> ins;
	std::vector<basic_block*> entry;
	std::vector<basic_block*> exit;
	std::map<unsigned int, unsigned int> index_map;
	unsigned int number;

	static unsigned int instance_number;

};

#endif