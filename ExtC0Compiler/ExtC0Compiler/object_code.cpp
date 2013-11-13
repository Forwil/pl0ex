#include "object_code.h"

namespace
{
	const static std::string ASM_INS_STRING_MAP[] = 
	{
		"LABEL",

		"MOV",

		"CDQ",
		"NEG",
		"ADD",
		"SUB",
		"IMUL",
		"IDIV",

		"CMP",
		"TEST",
		"JG",
		"JGE",
		"JL",
		"JLE",
		"JE",
		"JNE",
		"JP",
		"JNP",

		"JMP",

		"PUSH",
		"POP",
		"CALL",
		"PROC",
		"ENDP",
		"RET",
		"END",

		"FCHS",
		"FLD",
		"FILD",
		"FSTP",
		"FISTP",

		"FADD",
		"FSUB",
		"FMUL",
		"FDIV",

		"FCOMPP",
		"FNSTSW",

		"FTOI",
		"ITOF",
	};
}

tuple::tuple(asm_ins _ins):oper(_ins)
{
	enable = true;
	args = 0;
}
tuple::tuple(asm_ins _ins, std::string _oper1):oper(_ins), oper1(_oper1)
{
	enable = true;
	args = 1;
}
tuple::tuple(asm_ins _ins, std::string _oper1, std::string _oper2):oper(_ins), oper1(_oper1), oper2(_oper2)
{
	enable = true;
	args = 2;
}

std::ostream& operator<<(std::ostream& out, const tuple& ins)
{
	if(!ins.enable)return out;
	switch(ins.args)
	{
	case 0:
		out << "\t" << ASM_INS_STRING_MAP[ins.oper] << std::endl;
		break;
	case 1:
		switch(ins.oper)
		{
		case PROC:
			out << std::endl;
		case ENDP:
			out << ins.oper1 << " " << ASM_INS_STRING_MAP[ins.oper] << std::endl;
			break;
		case END:
			out << ASM_INS_STRING_MAP[ins.oper] << " " << ins.oper1 << std::endl;
			break;
		case LABEL:
			out << ins.oper1 << ":" << std::endl;
			break;
		default:
			out << "\t" << ASM_INS_STRING_MAP[ins.oper] << " " << ins.oper1 << std::endl;
		}
		break;
	case 2:
		out << "\t" << ASM_INS_STRING_MAP[ins.oper] << " " << ins.oper1 << ", " << ins.oper2 << std::endl;
		break;
	}
	return out;
}

void object_code::add(asm_ins oper)
{
	obj_code.push_back(new tuple(oper));
}

void object_code::add(asm_ins oper, std::string oper1)
{
	obj_code.push_back(new tuple(oper, oper1));
}

void object_code::add(asm_ins oper, std::string oper1, std::string oper2)
{
	obj_code.push_back(new tuple(oper, oper1, oper2));
}

void object_code::peephole()
{
	tuple* former = obj_code[0];
	for(unsigned int i = 1; i < obj_code.size(); i++)
	{
		switch(obj_code[i]->oper)
		{
		case MOV:
			if(obj_code[i]->oper1 == obj_code[i]->oper2)
			{
				obj_code[i]->enable = false;
			}
			else if(former->oper == MOV)
			{
				if(former->oper1 == obj_code[i]->oper1)
					former->enable = false;
				else if(former->oper2 == "EAX" || former->oper2 == "ECX" || former->oper2 == "EDX")
					/*
						MOV MEM, REG
						MOV REG, MEM，则第二条可以优化掉
					*/
					if(former->oper2 == obj_code[i]->oper1 && former->oper1 == obj_code[i]->oper2)
						obj_code[i]->enable = false;
			}
			break;
		case ADD:
		case SUB:
			break;
		case JMP:
			if(i + 1 < obj_code.size() && obj_code[i]->oper1 == obj_code[i + 1]->oper1)
			{
				obj_code[i]->enable = false;
			}
			break;
		}
		if(obj_code[i]->enable)former = obj_code[i];
	}
}

std::ostream& operator<<(std::ostream& out, const object_code& obj_code)
{
	out << ".code" << std::endl;
	for(unsigned int i = 0; i < obj_code.obj_code.size(); i++)
	{
		out << *(obj_code.obj_code[i]);
	}
	return out;
}
