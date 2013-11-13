#ifndef _COMPILER_GLOBAL_SYMBOL_TABLE
#define _COMPILER_GLOBAL_SYMBOL_TABLE

#include "symbol_table.h"
#include <map>
#include <vector>

class global_symbol_table
{
public:
	/*
		向栈顶符号表添加一个常量
		只可在语义分析时调用
	*/
	bool add_symbol(symbol::value_type, std::string, std::string);
	/*
		向栈顶符号表添加一个变量
		只可在语义分析时调用
	*/
	bool add_symbol(symbol::value_type, std::string);
	/*
		向栈顶符号表添加一个函数，并且返回该符号的symbol
		只可在语义分析时调用
	*/
	bool add_symbol(symbol::value_type, std::string, symbol::arguments*, symbol*&);

	/*
		向栈顶符号表添加一个参数
		只可在语义分析时调用
	*/
	bool add_argument(symbol::value_type, std::string, symbol::arguments*);
	
	/*
		向符号表中添加一个字面值
		只可在语义分析时调用
	*/
	void add_literal(symbol*&);

	/*
		在当前栈顶符号表中分配一个指定类型的临时变量
		只可在语义分析时调用
	*/
	void new_temp(symbol*&, symbol::value_type);
	/*
		在当前栈顶符号表中分配一个标号
		只可在语义分析时调用
	*/
	void new_label(symbol*&);

	/*
		在当前栈内符号表中查找指定标识符的符号
		只可在语义分析时调用
	*/
	symbol* look_up(std::string) const;

	/*
		获取一个符号表所属的函数或全局@global
	*/
	symbol* get_scope() const;

	/*
		获取指定函数的符号表
	*/
	symbol_table* get_table(symbol* sym) const;

	std::map<symbol*, symbol_table*> get_symbol_tables() const;

	/*
		在栈顶新建一个符号表
		只可在语义分析时调用
	*/
	void new_symbol_table(symbol*);
	/*
		弹出栈顶符号表
		只可在语义分析时调用
	*/
	void pop_symbol_table();

	///*
	//	为所有符号表分配地址
	//	应该在向目标代码翻译前调用
	//*/
	//void allocate();
private:
	bool add(symbol*);
	std::map<symbol*, symbol_table*> symbol_tables;
	std::vector<symbol_table*> stack_symbol_table;
	std::map<int, symbol*> int_literals;
	std::map<std::string, symbol*> char_literals;
	std::map<float, symbol*> float_literals;
	std::map<std::string, symbol*> string_literals;
};

#endif