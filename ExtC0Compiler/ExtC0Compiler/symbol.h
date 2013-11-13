#ifndef _COMPILER_SYMBOL
#define _COMPILER_SYMBOL

#include "token.h"
#include <string>
#include <vector>

class symbol_table;

/*
	���ţ��������ŵı�ʶ�����������͡�����ֵ���͵���Ϣ
	��Щ��Ϣֻ�ڷ���ʽ�ض����͵�ʱ�����Ч
	����argsֻ���ڷ���Ϊ����ʱ����Ч
	һ�����������ҽ�����һ�����ű�
*/
class symbol
{
public:
	const static enum symbol_type
	{
		LITERAL,
		CONST,
		VARIABLE,
		ARGUMENT,
		FUNCTION,
		LABEL
	};

	const static enum value_type
	{
		VOID,
		CHAR,
		INT,
		FLOAT,
		STRING,
	};

	typedef std::vector<symbol*> arguments;

	/*
		����һ������ֵ����
		��Ҫ�������ͺ�����ֵ���ַ�����ʽ
	*/
	static symbol* literal_symbol(value_type, std::string);
	/*
		����һ����ŷ���
		��Ҫ������ʶ��
	*/
	static symbol* label_symbol(std::string);
	/*
		����һ����������
		��Ҫ�������ͺͱ�ʶ��
	*/
	static symbol* variable_symbol(value_type, std::string);
	/*
		����һ����������
		��Ҫ���������ͺͱ�ʶ��
	*/
	static symbol* argument_symbol(value_type, std::string);
	/*
		����һ����������
		��Ҫ�������ͣ���ʶ����ֵ����ĸ����ʽ
	*/
	static symbol* const_symbol(value_type, std::string, std::string);
	/*
		����һ����������
		��Ҫ���������ķ���ֵ���ͣ���ʶ���Ͳ���
	*/
	static symbol* function_symbol(value_type, std::string, arguments*);
	
	/*
		��ȡһ�����ŵı�ʶ��
		������������ֵʱ��Ч
	*/
	std::string get_id() const;

	/*
		��ȡһ�����ŵ�ֵ
		�������Ǳ��������������������ʱ��Ч
	*/
	std::string get_value() const;

	/*
		��ȡ���ŵ�����
	*/
	symbol_type get_symbol_type() const;
	/*
		��ȡ���ŵ�ֵ����
		�������Ǳ��ʱ��Ч
	*/
	value_type get_value_type() const;
	/*
		��ȡ���ŵĲ�����Ϣ
		�����Ų��Ǻ���ʱ��Ч
	*/
	arguments* get_arguments() const;
	
	/*
		��ȡ���ŵ������ķ��ű�
		һ�����������ҽ�����һ�����ű�
	*/
	symbol_table* get_scope() const;
	///*
	//	��÷��ŵĵ�ַ
	//	�������Ǳ���������ʱ��Ч
	//	������Ҫ��֮ǰ���ù��������ڷ��ű��allocate������Ϊ���ŷ����ַ
	//*/
	//int get_address() const;

	///*
	//	��÷�����Ŀ��������Ĳ�������ʽ
	//*/
	//std::string get_operand() const;

	/*
		��ȡ����ռ�õ��ڴ��С
		��ǰ��ʵ�����κα�����ռ��4���ֽڵĴ�С
		����ռ�õĴ�С����Ϊ�ú��������в���ռ�õĴ�С֮��
		�������Ǳ�š�����ֵʱ��Ч
	*/
	unsigned int get_size();

	/*
		���һ�������Ƿ���Ч
		������Ż�����Щ���ſ��ܻ��Ϊ��Ч����
	*/
	bool is_enable() const;
	/*
		����һ�����������ķ��ű��ڱ����﷨��ʱ���ɱ���ʱ���ø���Ϣ
	*/
	void set_scope(symbol_table*);
	/*
		����һ�������Ƿ���Ч��ͨ�����Ż�������ɸ�����
	*/
	void set_enable(bool);
	///*
	//	����һ�������ĵ�ַ���ɷ��ű�allocateʱ���ø���Ϣ
	//*/
	//void set_address(int addr);
	
	friend void operator<<(std::ostream&, const symbol&);
private:
	symbol();
	symbol_type s_type;
	std::string id;
	value_type v_type;
	std::string value;
	arguments* args;
	symbol_table* scope;
	bool enable;
	///*
	//	��ʼΪ0�������Ϊ0�����ʾ�Ѿ�������˵�ַ
	//*/
	//int address;
	unsigned int size;
};

#endif