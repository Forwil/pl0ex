#ifndef _COMPILER_SYMBOL
#define _COMPILER_SYMBOL

#include "token.h"
#include <string>
#include <vector>

class symbol_table;

/*
	符号，包含符号的标识符、符号类型、符号值类型等信息
	有些信息只在符号式特定类型的时候才有效
	例如args只有在符号为函数时才有效
	一个符号属于且仅属于一个符号表
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
		构造一个字面值符号
		需要给出类型和字面值的字符串形式
	*/
	static symbol* literal_symbol(value_type, std::string);
	/*
		构造一个标号符号
		需要给出标识符
	*/
	static symbol* label_symbol(std::string);
	/*
		构造一个变量符号
		需要给出类型和标识符
	*/
	static symbol* variable_symbol(value_type, std::string);
	/*
		构造一个参数符号
		需要给出参类型和标识符
	*/
	static symbol* argument_symbol(value_type, std::string);
	/*
		构造一个常量符号
		需要给出类型，标识符和值的字母串形式
	*/
	static symbol* const_symbol(value_type, std::string, std::string);
	/*
		构造一个函数符号
		需要给出函数的返回值类型，标识符和参数
	*/
	static symbol* function_symbol(value_type, std::string, arguments*);
	
	/*
		获取一个符号的标识符
		当符号是字面值时无效
	*/
	std::string get_id() const;

	/*
		获取一个符号的值
		当符号是变量、参数、函数、标号时无效
	*/
	std::string get_value() const;

	/*
		获取符号的类型
	*/
	symbol_type get_symbol_type() const;
	/*
		获取符号的值类型
		当符号是标号时无效
	*/
	value_type get_value_type() const;
	/*
		获取符号的参数信息
		当符号不是函数时无效
	*/
	arguments* get_arguments() const;
	
	/*
		获取符号的所属的符号表
		一个符号属于且仅属于一个符号表
	*/
	symbol_table* get_scope() const;
	///*
	//	获得符号的地址
	//	当符号是变量、参数时有效
	//	并且需要在之前调用过符号所在符号表的allocate方法来为符号分配地址
	//*/
	//int get_address() const;

	///*
	//	获得符号在目标语言里的操作数形式
	//*/
	//std::string get_operand() const;

	/*
		获取符号占用的内存大小
		当前的实现是任何变量均占用4个字节的大小
		函数占用的大小定义为该函数的所有参数占用的大小之和
		当符号是标号、字面值时无效
	*/
	unsigned int get_size();

	/*
		获得一个符号是否有效
		在完成优化后，有些符号可能会成为无效符号
	*/
	bool is_enable() const;
	/*
		设置一个符号所属的符号表，在遍历语法树时生成变量时设置该信息
	*/
	void set_scope(symbol_table*);
	/*
		设置一个变量是否有效，通常由优化器来完成该任务
	*/
	void set_enable(bool);
	///*
	//	设置一个变量的地址，由符号表allocate时设置该信息
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
	//	初始为0，如果不为0，则表示已经分配过了地址
	//*/
	//int address;
	unsigned int size;
};

#endif