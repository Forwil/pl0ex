#ifndef _COMPILER_INSTRUCTIONS
#define _COMPILER_INSTRUCTIONS

#include "token.h"
#include "symbol.h"
#include "quaternion.h"
#include "basic_block.h"
#include "function_block.h"
#include "global_symbol_table.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>

class instructions
{
public:
	friend class constance_propagation;
	friend class dag;
	friend class data_stream;
	friend class nets;
	friend class live_variable;
	friend class reach_definition;
	friend class conflict_graph;

	instructions(global_symbol_table&);
	/*
		表达式相关
	*/
	void arithmetic_operation(symbol*, token, symbol*, symbol*&);
	void inverse(symbol*, symbol*&);
	void assign(symbol*, symbol*);
	void assign(symbol*, symbol*, token, symbol*);//增量语句
	/*
		跳转相关
	*/
	void add_label(symbol*);
	void jump(symbol*);
	void branch(symbol*, token, symbol*, symbol*&);
	/*
		函数调用相关
	*/
	void fun(symbol*);
	void param(symbol*);
	void call(symbol*);
	void call(symbol*, symbol*&);
	void add_parameter(symbol*);
	void ret(symbol*, symbol*);
	void ret(symbol*);
	void end_fun(symbol*);
	/*
		类型转换
	*/
	void ftoi(symbol*, symbol*&);
	void itof(symbol*, symbol*&);
	/*
		IO
	*/
	void read(symbol*);
	void write(symbol*);
	void new_line();

	std::vector<quaternion*> get_sequential() const;
	std::vector<function_block*> get_function_blocks() const;
	friend std::ostream& operator<<(std::ostream&, const instructions&);
	quaternion* operator[](unsigned int);
private:
	void new_instruction(quaternion*);
	
	/*
		四元式的线性序列
	*/
	std::vector<quaternion*> sequential;

	/*
		函数块的序列
	*/
	std::vector<function_block*> blocks;

	/*
		全局符号表的引用
	*/
	global_symbol_table& tables;
	void update_reference(quaternion*);
};
#endif