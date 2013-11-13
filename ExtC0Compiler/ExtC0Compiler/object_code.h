#ifndef _COMPILER_OBJECT_CODE
#define _COMPILER_OBJECT_CODE

#include <vector>
#include <string>
#include <iostream>

const static enum asm_ins
{
	LABEL,

	MOV,

	CDQ,
	NEG,
	ADD,
	SUB,
	IMUL,
	IDIV,

	CMP,
	TEST,
	JG,
	JGE,
	JL,
	JLE,
	JE,
	JNE,
	JP,
	JNP,

	JMP,

	PUSH,
	POP,
	CALL,
	PROC,
	ENDP,
	RET,
	END,

	FCHS,
	FLD,
	FILD,
	FSTP,
	FISTP,

	FADD,
	FSUB,
	FMUL,
	FDIV,

	FCOMPP,
	FNSTSW,

	FTOI,
	ITOF,
};

struct tuple
{
public:
	friend class object_code;
	tuple(asm_ins);
	tuple(asm_ins, std::string);
	tuple(asm_ins, std::string, std::string);
	friend std::ostream& operator<<(std::ostream&, const tuple&);
private:
	asm_ins oper;
	std::string oper1;
	std::string oper2;
	bool enable;
	int args;
};

class object_code
{
public:
	void add(asm_ins);
	void add(asm_ins, std::string);
	void add(asm_ins, std::string, std::string);
	void peephole();
	friend std::ostream& operator<<(std::ostream&, const object_code&);
private:
	std::vector<tuple*> obj_code;
};

#endif