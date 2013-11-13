#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <cassert>
#include "utility.h"
#include "translator.h"
#include "function_block.h"

using namespace std;

namespace
{
	const std::string printf_char = "$$printf_char";
	const std::string printf_int = "$$printf_int";
	const std::string printf_float = "$$printf_float";
	const std::string new_line = "$$new_line";
	const std::string scanf_char = "$$scanf_char";
	const std::string scanf_int = "$$scanf_int";
	const std::string scanf_float = "$$scanf_float";
}

translator::translator(instructions* _ins, global_symbol_table* _table, data_stream* _ds):ins(_ins), ds(_ds), ra(_table, _ins, _ds)
{
	table = _table;
}

void translator::translate(ostream& out)
{
	header(out);
	body(out);
}

void translator::header(ostream& out)
{
	ifstream fs("header.txt");
	char ch;
	while(fs.get(ch))
		out << ch;
}

void translator::const_pool(ostream& out)
{
	out << ".data" << endl << endl;
	out << printf_char << " db \"%c\", 0" << endl;
	out << printf_int << " db \"%d\", 0" << endl;
	out << printf_float << " db \"%f\", 0" << endl;
	out << new_line << " db 10, 13, 0" << endl;
	out << scanf_char << " db \"%c\", 0" << endl;
	out << scanf_int << " db \"%d\", 0" << endl;
	out << scanf_float << " db \"%f\", 0" << endl;

	std::set<symbol*> consts = ra.get_const_pool();
	for(std::set<symbol*>::iterator iter = consts.begin(); iter != consts.end(); iter++)
	{
		out << ra.get_operand_name(*iter);
		if(utility::is_string(*iter))
		{
			out << " db " << ra.get_literal_value(*iter) << endl;
		}
		else
		{
			out << " dd " << ra.get_literal_value(*iter) << endl;
		}
	}
	out << endl;
	out << ".data?" << endl << endl;
	std::set<symbol*> globals = ra.get_global_variable();
	for(std::set<symbol*>::iterator iter = globals.begin(); iter != globals.end(); iter++)
	{
		out << ra.get_operand_name(*iter) << " dd ?" << endl;
	}
	out << endl;
}

void translator::body(ostream& out)
{
		
	vector<function_block*> blocks = ins->get_function_blocks();
	for(unsigned int i = 0; i < blocks.size(); i++)
	{
		translate(blocks[i]);
	}
	obj_code.peephole();
	const_pool(out);
	out << obj_code;
}

void translator::translate(function_block* function)
{
	vector<quaternion*> quats = function->get_sequential();
	for(unsigned int i = 0; i < quats.size(); i++)
	{
		translate(function, quats[i]);
	}
}

void translator::translate(function_block* scope, quaternion* quat)
{
	switch(quat->get_operation())
	{
	case quaternion::ASSIGN:
		translate_assign(scope, quat);
		break;
	case quaternion::INV:
		translate_inv(scope, quat);
		break;
	case quaternion::ADD:
		translate_add(scope, quat);
		break;
	case quaternion::SUB:
		translate_sub(scope, quat);
		break;
	case quaternion::MUL:
		translate_mul(scope, quat);
		break;
	case quaternion::DIV:
		translate_div(scope, quat);
		break;
	case quaternion::FINV:
		translate_finv(scope, quat);
		break;
	case quaternion::FADD:
		translate_fadd(scope, quat);
		break;
	case quaternion::FSUB:
		translate_fsub(scope, quat);
		break;
	case quaternion::FMUL:
		translate_fmul(scope, quat);
		break;
	case quaternion::FDIV:
		translate_fdiv(scope, quat);
		break;
	case quaternion::FTOI:
		translate_ftoi(scope, quat);
		break;
	case quaternion::ITOF:
		translate_itof(scope, quat);
		break;
	case quaternion::JGT:
		translate_jgt(scope, quat);
		break;
	case quaternion::JGE:
		translate_jge(scope, quat);
		break;
	case quaternion::JLT:
		translate_jlt(scope, quat);
		break;
	case quaternion::JLE:
		translate_jle(scope, quat);
		break;
	case quaternion::JEQ:
		translate_jeq(scope, quat);
		break;
	case quaternion::JNE:
		translate_jne(scope, quat);
		break;
	case quaternion::FJGT:
		translate_fjgt(scope, quat);
		break;
	case quaternion::FJGE:
		translate_fjge(scope, quat);
		break;
	case quaternion::FJLT:
		translate_fjlt(scope, quat);
		break;
	case quaternion::FJLE:
		translate_fjle(scope, quat);
		break;
	case quaternion::FJEQ:
		translate_fjeq(scope, quat);
		break;
	case quaternion::FJNE:
		translate_fjne(scope, quat);
		break;
	case quaternion::JMP:
		translate_jmp(scope, quat);
		break;
	case quaternion::FUN:
		translate_fun(scope, quat);
		break;
	case quaternion::CALL:
		translate_call(scope, quat);
		break;
	case quaternion::PUSH:
		translate_push(scope, quat);
		break;
	case quaternion::PARAM:
		translate_param(scope, quat);
		break;
	case quaternion::RET:
		translate_ret(scope, quat);
		break;
	case quaternion::ENDF:
		translate_endf(scope, quat);
		break;
	case quaternion::LABEL:
		translate_label(scope, quat);
		break;
	case quaternion::READ:
		translate_read(scope, quat);
		break;
	case quaternion::WRITE:
		translate_write(scope, quat);
		break;
	case quaternion::NEW_LINE:
		translate_new_line(scope, quat);
		break;
	}
}

void translator::translate_assign(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	symbol* obj = quat->get_object_args();
	if(obj->get_value_type() == symbol::FLOAT)
	{
		obj_code.add(FLD, ra.get_operand(src, quat->get_number(), false));
		obj_code.add(FSTP, ra.get_operand(obj, quat->get_number(), true));
	}
	else
	{
		if(ra.in_register(obj, quat->get_number(), false) == NA)
		{
			occupy_local_register(obj, quat->get_number(), false);
		}
		obj_code.add(MOV, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(src, quat->get_number(), true));
	}
}

void translator::translate_inv(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	symbol* obj = quat->get_object_args();
	if(ra.in_register(obj, quat->get_number(), false) == NA)
	{
		occupy_local_register(obj, quat->get_number(), false);
	}
	obj_code.add(MOV, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(src, quat->get_number(), true));
	obj_code.add(NEG, ra.get_operand(obj, quat->get_number(), false));
}

void translator::translate_add(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_arithmetic(scope, quat, ADD);
}

void translator::translate_sub(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_arithmetic(scope, quat, SUB);
}

void translator::translate_mul(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_arithmetic(scope, quat, IMUL);
}

void translator::translate_div(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src1 = quat->get_source_args()[0];
	symbol* src2 = quat->get_source_args()[1];
	symbol* obj = quat->get_object_args();
	
	occupy_local_register(obj, EAX, quat->get_number(), false);
	obj_code.add(MOV, ra.get_operand(EAX), ra.get_operand(src1, quat->get_number(), true));
	release_local_register(EDX, quat->get_number());
	obj_code.add(CDQ);
	obj_code.add(IDIV, ra.get_operand(src2, quat->get_number(), true));
}

void translator::translate_finv(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	symbol* obj = quat->get_object_args();
	obj_code.add(FLD, ra.get_operand(src, quat->get_number(), true));
	obj_code.add(FCHS);
	obj_code.add(FSTP, ra.get_operand(obj, quat->get_number(), false));
}

void translator::translate_fadd(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_farithmetic(scope, quat, FADD);
}

void translator::translate_fsub(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_farithmetic(scope, quat, FSUB);
}

void translator::translate_fmul(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_farithmetic(scope, quat, FMUL);
}

void translator::translate_fdiv(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_farithmetic(scope, quat, FDIV);
}

void translator::translate_ftoi(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	symbol* obj = quat->get_object_args();
	obj_code.add(FLD, ra.get_operand(src, quat->get_number(), true));
	if(ra.in_register(obj, quat->get_number(), false))
	{
		obj_code.add(SUB, ra.get_operand(ESP), utility::to_string(4));
		ra.increase_offset(4);
		obj_code.add(FISTP, "dword ptr [esp]");
		obj_code.add(POP, ra.get_operand(obj, quat->get_number(), false));
		ra.increase_offset(-4);
	}
	else
	{
		obj_code.add(FISTP, ra.get_operand(obj, quat->get_number(), false));
	}
}

void translator::translate_itof(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	symbol* obj = quat->get_object_args();
	if(ra.in_register(src, quat->get_number(), true))
	{
		obj_code.add(PUSH, ra.get_operand(src, quat->get_number(), true));
		obj_code.add(FILD, "dword ptr [esp]");
		obj_code.add(ADD, ra.get_operand(ESP), utility::to_string(4));
		obj_code.add(FSTP, ra.get_operand(obj, quat->get_number(), false));
	}
	else
	{
		obj_code.add(FILD, ra.get_operand(src, quat->get_number(), true));
		obj_code.add(FSTP, ra.get_operand(obj, quat->get_number(), false));
	}
}

void translator::translate_jgt(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_branch(scope, quat, JG);
}

void translator::translate_jge(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_branch(scope, quat, JGE);
}

void translator::translate_jlt(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_branch(scope, quat, JL);
}

void translator::translate_jle(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_branch(scope, quat, JLE);
}

void translator::translate_jeq(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_branch(scope, quat, JE);
}

void translator::translate_jne(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	translate_branch(scope, quat, JNE);
}

void translator::translate_fjgt(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[2];
	translate_fbranch(scope, quat);
	obj_code.add(TEST, ra.get_operand(AH), utility::to_string(1));
	obj_code.add(JNE, ra.get_operand_name(src));
}

void translator::translate_fjge(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[2];
	translate_fbranch(scope, quat);
	obj_code.add(TEST, ra.get_operand(AH), utility::to_string(65));
	obj_code.add(JNE, ra.get_operand_name(src));
}

void translator::translate_fjlt(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[2];
	translate_fbranch(scope, quat);
	obj_code.add(TEST, ra.get_operand(AH), utility::to_string(65));
	obj_code.add(JP, ra.get_operand_name(src));
}

void translator::translate_fjle(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[2];
	translate_fbranch(scope, quat);
	obj_code.add(TEST, ra.get_operand(AH), utility::to_string(5));
	obj_code.add(JP, ra.get_operand_name(src));
}

void translator::translate_fjeq(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[2];
	translate_fbranch(scope, quat);
	obj_code.add(TEST, ra.get_operand(AH), utility::to_string(68));
	obj_code.add(JNP, ra.get_operand_name(src));
}

void translator::translate_fjne(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[2];
	translate_fbranch(scope, quat);
	obj_code.add(TEST, ra.get_operand(AH), utility::to_string(68));
	obj_code.add(JP, ra.get_operand_name(src));
}

void translator::translate_jmp(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	release_local_register(quat->get_number());
	obj_code.add(JMP, ra.get_operand_name(src));
}

void translator::translate_fun(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	obj_code.add(PROC, ra.get_operand_name(src));
	if(src->get_id() != "main")
	{
		obj_code.add(PUSH, ra.get_operand(EBX));
		obj_code.add(PUSH, ra.get_operand(EBP));
		obj_code.add(PUSH, ra.get_operand(ESI));
		obj_code.add(PUSH, ra.get_operand(EDI));
	}
	obj_code.add(SUB, ra.get_operand(ESP), utility::to_string(ra.get_local_space(src)));
}

void translator::translate_call(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	symbol* obj = quat->get_object_args();
	release_local_register(quat->get_number());
	obj_code.add(CALL, ra.get_operand_name(src));
	ra.increase_offset(-src->get_size());
	if(obj != NULL)
	{
		if(obj->get_value_type() == symbol::FLOAT)
		{
			obj_code.add(MOV, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(EAX));
		}
		else
		{
			ra.occupy_register(EAX, obj, quat->get_number(), false);
		}
	}
}

void translator::translate_push(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	obj_code.add(PUSH, ra.get_operand(src, quat->get_number(), true));
	ra.increase_offset(4);
}

void translator::translate_param(function_block* scope, quaternion* quat)
{
	symbol* obj = quat->get_object_args();
	if(ra.in_register(obj, quat->get_number(), false))
	{
		obj_code.add(MOV, ra.get_operand(obj, quat->get_number(), false), ra.get_address(obj));
	}
}

void translator::translate_ret(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src1 = quat->get_arg(0);
	symbol* src2 = quat->get_arg(1);
	if(src2 != NULL)
	{
		release_local_register(quat->get_number());
		occupy_local_register(src2, EAX, quat->get_number(), true);
		ra.release_register(EAX);
	}
	obj_code.add(JMP, "@@endof" + ra.get_operand_name(src1));
}

void translator::translate_endf(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	release_local_register(quat->get_number());
	obj_code.add(LABEL, "@@endof" + ra.get_operand_name(src));
	obj_code.add(ADD, ra.get_operand(ESP), utility::to_string(ra.get_local_space(src)));
	if(src->get_id() != "main")
	{
		obj_code.add(POP, ra.get_operand(EDI));
		obj_code.add(POP, ra.get_operand(ESI));
		obj_code.add(POP, ra.get_operand(EBP));
		obj_code.add(POP, ra.get_operand(EBX));
	}

	obj_code.add(RET, utility::to_string(ra.get_args_space(src)));
	obj_code.add(ENDP, ra.get_operand_name(src));

	if(src->get_id() == "main")
	{
		obj_code.add(END, ra.get_operand_name(src));
	}
}

void translator::translate_label(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	release_local_register(quat->get_number());
	obj_code.add(LABEL, ra.get_operand_name(src));
}

void translator::translate_read(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* obj = quat->get_object_args();
	release_local_register(quat->get_number());
	obj_code.add(SUB, ra.get_operand(ESP), utility::to_string(4));
	obj_code.add(PUSH, ra.get_operand(ESP));
	switch(obj->get_value_type())
	{
	case symbol::CHAR:
		obj_code.add(PUSH, "offset " + scanf_char);
		break;
	case symbol::INT:
		obj_code.add(PUSH, "offset " + scanf_int);
		break;
	case symbol::FLOAT:
		obj_code.add(PUSH, "offset " + scanf_float);
		break;
	}
	obj_code.add(CALL, "scanf");
	obj_code.add(ADD, ra.get_operand(ESP), utility::to_string(8));
	obj_code.add(POP, ra.get_operand(obj, quat->get_number(), false));
}

void translator::translate_write(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	symbol* src = quat->get_source_args()[0];
	release_local_register(quat->get_number());
	switch(src->get_value_type())
	{
	case symbol::CHAR:
		obj_code.add(PUSH, ra.get_operand(src, quat->get_number(), true));
		obj_code.add(PUSH, "offset " + printf_char);
		obj_code.add(CALL, "printf");
		obj_code.add(ADD, ra.get_operand(ESP), utility::to_string(8));
		break;
	case symbol::INT:
		obj_code.add(PUSH, ra.get_operand(src, quat->get_number(), true));
		obj_code.add(PUSH, "offset " + printf_int);
		obj_code.add(CALL, "printf");
		obj_code.add(ADD, ra.get_operand(ESP), utility::to_string(8));
		break;
	case symbol::FLOAT:
		obj_code.add(SUB, ra.get_operand(ESP), utility::to_string(8));
		ra.increase_offset(8);
		obj_code.add(FLD, ra.get_operand(src, quat->get_number(), true));
		obj_code.add(FSTP, "qword ptr [esp]");
		obj_code.add(PUSH, "offset " + printf_float);
		obj_code.add(CALL, "printf");
		obj_code.add(ADD, ra.get_operand(ESP), utility::to_string(12));
		ra.increase_offset(-8);
		break;
	case symbol::STRING:
		obj_code.add(PUSH, "offset " + ra.get_operand(src, quat->get_number(), true));
		obj_code.add(CALL, "printf");
		obj_code.add(ADD, ra.get_operand(ESP), utility::to_string(4));
		break;
	}
}

void translator::translate_new_line(function_block* scope, quaternion* quat)
{
	if(!quat->is_enable()) return;
	obj_code.add(PUSH, "offset " + new_line);
	obj_code.add(CALL, "printf");
	obj_code.add(ADD, ra.get_operand(ESP), utility::to_string(4));
}

void translator::translate_arithmetic(function_block* scope, quaternion* quat, asm_ins ins)
{
	symbol* src1 = quat->get_source_args()[0];
	symbol* src2 = quat->get_source_args()[1];
	symbol* obj = quat->get_object_args();
	
	if(src1 == obj)
	{
		// obj == src1 ?= src2
		if(ra.in_register(obj, quat->get_number(), false) == NA)
		{
			occupy_local_register(obj, quat->get_number(), false);
		}
		//obj == src1 == src2К±ТаїЙ
		obj_code.add(MOV, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(obj, quat->get_number(), true));
		obj_code.add(ins, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(src2, quat->get_number(), true));
	}
	else if(src2 == obj)
	{
		// obj == src2 != src1
		string oper2 = ra.get_operand(src2, quat->get_number(), true);
		occupy_local_register(obj, quat->get_number(), false);
		obj_code.add(MOV, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(src1, quat->get_number(), true));
		obj_code.add(ins, ra.get_operand(obj, quat->get_number(), false), oper2);
	}
	else if(src1 == src2)
	{
		// obj != src1 == src2
		if(ra.in_register(obj, quat->get_number(), false) == NA)
		{
			occupy_local_register(obj, quat->get_number(), false);
		}
		obj_code.add(MOV, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(src1, quat->get_number(), true));
		obj_code.add(ins, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(obj, quat->get_number(), false));
	}
	else
	{
		// obj != src1 != src2
		if(ra.in_register(obj, quat->get_number(), false) == NA)
		{
			occupy_local_register(obj, quat->get_number(), false);
		}
		obj_code.add(MOV, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(src1, quat->get_number(), true));
		obj_code.add(ins, ra.get_operand(obj, quat->get_number(), false), ra.get_operand(src2, quat->get_number(), true));
	}
}

void translator::translate_branch(function_block* scope, quaternion* quat, asm_ins ins)
{
	symbol* src1 = quat->get_source_args()[0];
	symbol* src2 = quat->get_source_args()[1];
	symbol* src3 = quat->get_source_args()[2];
	
	if(ra.in_register(src1, quat->get_number(), true) == NA && ra.in_register(src2, quat->get_number(), true) == NA)
	{
		occupy_local_register(src1, quat->get_number(), true);
	}
	obj_code.add(CMP, ra.get_operand(src1, quat->get_number(), true), ra.get_operand(src2, quat->get_number(), true));
	release_local_register(quat->get_number());
	obj_code.add(ins, ra.get_operand_name(src3));
}

void translator::translate_farithmetic(function_block* scope, quaternion* quat, asm_ins ins)
{
	symbol* src1 = quat->get_source_args()[0];
	symbol* src2 = quat->get_source_args()[1];
	symbol* obj = quat->get_object_args();
	obj_code.add(FLD, ra.get_operand(src1, quat->get_number(), true));
	obj_code.add(ins, ra.get_operand(src2, quat->get_number(), true));
	obj_code.add(FSTP, ra.get_operand(obj, quat->get_number(), false));
}

void translator::translate_fbranch(function_block* scope, quaternion* quat)
{
	symbol* src1 = quat->get_source_args()[0];
	symbol* src2 = quat->get_source_args()[1];
	obj_code.add(FLD, ra.get_operand(src1, quat->get_number(), true));
	obj_code.add(FLD, ra.get_operand(src2, quat->get_number(), true));
	obj_code.add(FCOMPP);
	release_local_register(EAX);
	obj_code.add(FNSTSW, ra.get_operand(AX));
}

void translator::release_local_register(unsigned int in_line)
{
	if(ds != NULL)
	{
		release_local_register(EDX, in_line);
		release_local_register(ECX, in_line);
		release_local_register(EAX, in_line);
	}
	else
	{
		release_local_register(EDI, in_line);
		release_local_register(ESI, in_line);
		release_local_register(EBP, in_line);
		release_local_register(EBX, in_line);
		release_local_register(EDX, in_line);
		release_local_register(ECX, in_line);
		release_local_register(EAX, in_line);
	}
}

void translator::release_local_register(reg_name reg, unsigned int in_line)
{
	pair<symbol*, pair<unsigned int, bool>> prev = ra.get_symbol_in_reg(reg);
	write_reg_back(prev.first, reg, prev.second.first, prev.second.second, in_line);
	ra.release_register(reg);
}

void translator::occupy_local_register(symbol* sym, unsigned int line, bool is_use)
{
	reg_name reg = ra.allocate(sym);
	occupy_local_register(sym, reg, line, is_use);
}

void translator::occupy_local_register(symbol* sym, reg_name reg, unsigned int line, bool is_use)
{
	release_local_register(reg, line);
	obj_code.add(MOV, ra.get_operand(reg), ra.get_operand(sym, line, is_use));
	ra.occupy_register(reg, sym, line, is_use);
}

void translator::write_reg_back(symbol* sym, reg_name reg, unsigned int line, bool is_use, unsigned int in_line)
{
	if(sym != NULL)
	{
		ra.release_register(reg);
		if(is_alive(sym, line, is_use, in_line))
		{
			if(!utility::is_literal(sym))
			{
				obj_code.add(MOV, ra.get_operand(sym, line, is_use), ra.get_operand(reg));
			}
		}
	}
}

bool translator::is_alive(symbol* sym, unsigned int line, bool is_use, unsigned int in_line)
{
	if(ds == NULL)
	{
		return true;
	}
	else
	{
		if(utility::is_literal(sym))
		{
			return false;
		}
		else if(utility::is_global(sym))
		{
			return true;
		}
		else
		{
			return ra.is_alive(sym, line, is_use, in_line);
		}
	}
}
