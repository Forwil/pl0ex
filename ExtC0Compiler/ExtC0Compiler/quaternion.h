#ifndef _COMPILER_QUATERNION
#define _COMPILER_QUATERNION

#include "symbol.h"
#include "symbol_table.h"
#include <vector>

class basic_block;

/*
	四元式类型
	由translate方法，实现向目标语言的翻译
	共有34中不同的操作，其中算数指令和分支指令各有一套整数版本和实数版本，另外还有整数和实数类型的相互转换2句
*/
class quaternion
{
public:
	const static enum operation
	{
		ASSIGN,
		INV,
		ADD,
		SUB,
		MUL,
		DIV,
		FINV,
		FADD,
		FSUB,
		FMUL,
		FDIV,
		FTOI,
		ITOF,
		JGT,
		JGE,
		JLT,
		JLE,
		JEQ,
		JNE,
		FJGT,
		FJGE,
		FJLT,
		FJLE,
		FJEQ,
		FJNE,
		JMP,
		FUN,
		CALL,
		PUSH,
		PARAM,
		RET,
		ENDF,
		LABEL,
		READ,
		WRITE,
		NEW_LINE,
	};
	static quaternion* new_quaternion(operation, symbol*, symbol*, symbol*);
	static quaternion* new_quaternion(operation, symbol*, symbol*);
	static quaternion* new_quaternion(operation, symbol*);
	static quaternion* new_quaternion(operation);
    
	/*
		获取四元式的操作的枚举值
	*/
	operation get_operation() const;
	/*
		获取四元式的指定参数，如果超越了参数个数，则返回NULL
	*/
	symbol* get_arg(unsigned int) const;
	/*
		获取源操作数
	*/
	std::vector<symbol*> get_source_args() const;
	/*
		获取目标操作数
	*/
	symbol* get_object_args() const;

	/*
		获取四元式是否有效
		在优化过程中，可能使一条指令失效
	*/
	bool is_enable() const;

	unsigned int get_number() const;

	void disable();
	/*
		初始化四元式类，编号归零等
	*/
	static void flush();

	friend void operator<<(std::ostream&, const quaternion&);
protected:
	quaternion();
	operation oper;
	std::vector<symbol*> args;
	std::vector<symbol*> source_args;
	symbol* object_args;

	basic_block* scope;

	bool enabled;
	unsigned int number;
	static unsigned int instance_number;
};

#endif