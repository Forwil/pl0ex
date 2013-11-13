#ifndef _COMPILER_QUATERNION
#define _COMPILER_QUATERNION

#include "symbol.h"
#include "symbol_table.h"
#include <vector>

class basic_block;

/*
	��Ԫʽ����
	��translate������ʵ����Ŀ�����Եķ���
	����34�в�ͬ�Ĳ�������������ָ��ͷ�ָ֧�����һ�������汾��ʵ���汾�����⻹��������ʵ�����͵��໥ת��2��
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
		��ȡ��Ԫʽ�Ĳ�����ö��ֵ
	*/
	operation get_operation() const;
	/*
		��ȡ��Ԫʽ��ָ�������������Խ�˲����������򷵻�NULL
	*/
	symbol* get_arg(unsigned int) const;
	/*
		��ȡԴ������
	*/
	std::vector<symbol*> get_source_args() const;
	/*
		��ȡĿ�������
	*/
	symbol* get_object_args() const;

	/*
		��ȡ��Ԫʽ�Ƿ���Ч
		���Ż������У�����ʹһ��ָ��ʧЧ
	*/
	bool is_enable() const;

	unsigned int get_number() const;

	void disable();
	/*
		��ʼ����Ԫʽ�࣬��Ź����
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