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
		����һ�����ű�
		����÷��ű���ȫ�ַ��ű���scopeΪһ������ı���������Ϊ"@global"
		����÷��ű���ĳһ�������ķ��ű���scope�Ǹú�����Ӧ��symbol
	*/
	symbol_table(symbol* scope);
	
	/*
		����ű������һ������
		�÷��ſ����Ǳ�������������������������������������ֵ
		���������ͬ������������ӳɹ�����true�����򷵻�false
	*/
	bool add_symbol(symbol*);

	/*
		�ڷ��ű��в���ָ����ʶ���ķ���
		����ҵ�����true���������������symbol*&�У����򷵻�false
	*/
	bool find(std::string, symbol*&) const;

	/*
		����ָ�����͵���һ����ʱ����
		��ʱ������������$temp��ͷ��������ϲ�ͬ��������ʾ����
	*/
	symbol* next_temp(symbol::value_type);
	/*
		������һ��label
		label������Ϊlabel�����Ҳ���ӽ����ű�����������labels�У�������ʱͳһ�ͷ�
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
		���ŵ��������У������ַʱ��Ҫʹ��
	*/
	std::vector<symbol*> sequential;
	/*
		����������ô�����Ϣ
	*/
	std::map<symbol*, unsigned int> references;
	
	//std::vector<symbol*> labels;
	
	temp_variable_manager* _temp_manager;
	label_manager* _label_manager;
	bool defined(std::string) const;
};

#endif