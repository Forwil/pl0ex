#ifndef _COMPILER_GLOBAL_SYMBOL_TABLE
#define _COMPILER_GLOBAL_SYMBOL_TABLE

#include "symbol_table.h"
#include <map>
#include <vector>

class global_symbol_table
{
public:
	/*
		��ջ�����ű����һ������
		ֻ�����������ʱ����
	*/
	bool add_symbol(symbol::value_type, std::string, std::string);
	/*
		��ջ�����ű����һ������
		ֻ�����������ʱ����
	*/
	bool add_symbol(symbol::value_type, std::string);
	/*
		��ջ�����ű����һ�����������ҷ��ظ÷��ŵ�symbol
		ֻ�����������ʱ����
	*/
	bool add_symbol(symbol::value_type, std::string, symbol::arguments*, symbol*&);

	/*
		��ջ�����ű����һ������
		ֻ�����������ʱ����
	*/
	bool add_argument(symbol::value_type, std::string, symbol::arguments*);
	
	/*
		����ű������һ������ֵ
		ֻ�����������ʱ����
	*/
	void add_literal(symbol*&);

	/*
		�ڵ�ǰջ�����ű��з���һ��ָ�����͵���ʱ����
		ֻ�����������ʱ����
	*/
	void new_temp(symbol*&, symbol::value_type);
	/*
		�ڵ�ǰջ�����ű��з���һ�����
		ֻ�����������ʱ����
	*/
	void new_label(symbol*&);

	/*
		�ڵ�ǰջ�ڷ��ű��в���ָ����ʶ���ķ���
		ֻ�����������ʱ����
	*/
	symbol* look_up(std::string) const;

	/*
		��ȡһ�����ű������ĺ�����ȫ��@global
	*/
	symbol* get_scope() const;

	/*
		��ȡָ�������ķ��ű�
	*/
	symbol_table* get_table(symbol* sym) const;

	std::map<symbol*, symbol_table*> get_symbol_tables() const;

	/*
		��ջ���½�һ�����ű�
		ֻ�����������ʱ����
	*/
	void new_symbol_table(symbol*);
	/*
		����ջ�����ű�
		ֻ�����������ʱ����
	*/
	void pop_symbol_table();

	///*
	//	Ϊ���з��ű�����ַ
	//	Ӧ������Ŀ����뷭��ǰ����
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