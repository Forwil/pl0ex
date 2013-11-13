#ifndef _COMPILER_SYMBOL_TABLE
#define _COMPILER_SYMBOL_TABLE

#include "symbol.h"
#include "temp_variable_manager.h"
#include "label_manager.h"
#include <string>
#include <vector>
#include <map>

class symbol_table
{
public:
	/*
		构造一个符号表
		如果该符号表是全局符号表，则scope为一个特殊的变量，名称为"@global"
		如果该符号表是某一个函数的符号表，则scope是该函数对应的symbol
	*/
	symbol_table(symbol* scope);
	
	/*
		向符号表中添加一个符号
		该符号可以是变量、参数、常量、函数，但不可以是字面值
		如果不存在同名变量，则添加成功返回true，否则返回false
	*/
	bool add_symbol(symbol*);

	/*
		在符号表中查找指定标识符的符号
		如果找到返回true，并将结果保存在symbol*&中，否则返回false
	*/
	bool find(std::string, symbol*&) const;

	/*
		返回指定类型的下一个临时变量
		临时变量的名称以$temp打头，后面加上不同的数字以示区分
	*/
	symbol* next_temp(symbol::value_type);
	/*
		返回下一个label
		label的名称为label，并且不添加进符号表，其句柄保存在labels中，在析构时统一释放
	*/
	symbol* next_label();

	symbol* get_scope() const {return scope;}
	std::string get_name() const {return scope->get_id();}
	
	void update_reference(symbol*, int);

	std::vector<symbol*> get_sequential() const;
private:
	symbol* scope;
	std::map<std::string, symbol*> symbols;
	/*
		符号的线性序列，分配地址时需要使用
	*/
	std::vector<symbol*> sequential;
	/*
		保存符号引用次数信息
	*/
	std::map<symbol*, unsigned int> references;
	
	//std::vector<symbol*> labels;
	
	temp_variable_manager* _temp_manager;
	label_manager* _label_manager;
	bool defined(std::string) const;
};

#endif