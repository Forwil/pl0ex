#include "instructions.h"
#include "utility.h"
#include <cassert>

#define _NOT_NULL(arg) (arg != NULL)

using namespace std;

namespace
{
	symbol::value_type type_upgrade(symbol* arg1, symbol* arg2)
	{
		symbol::value_type v1 = arg1->get_value_type();
		symbol::value_type v2 = arg2->get_value_type();
		return (v1 > v2 ? v1 : v2);
	}

	quaternion* generate(quaternion::operation oper, symbol* arg1, symbol* arg2, symbol* arg3)
	{
		return quaternion::new_quaternion(oper, arg1, arg2, arg3);
	}

	quaternion* generate(quaternion::operation oper, symbol* arg1, symbol* arg2)
	{
		return quaternion::new_quaternion(oper, arg1, arg2);
	}

	quaternion* generate(quaternion::operation oper, symbol* arg1)
	{
		return quaternion::new_quaternion(oper, arg1);
	}

	quaternion* generate(quaternion::operation oper)
	{
		return quaternion::new_quaternion(oper);
	}

	bool is_jump_operation(quaternion::operation oper)
	{
		return (oper == quaternion::JMP ||
				oper == quaternion::JGT ||
				oper == quaternion::JGE ||
				oper == quaternion::JLT ||
				oper == quaternion::JLE ||
				oper == quaternion::JEQ ||
				oper == quaternion::JNE ||
				oper == quaternion::FJGT ||
				oper == quaternion::FJGE ||
				oper == quaternion::FJLT ||
				oper == quaternion::FJLE ||
				oper == quaternion::FJEQ ||
				oper == quaternion::FJNE);
	}
};

instructions::instructions(global_symbol_table& _tables):tables(_tables)
{
	function_block::flush();
	basic_block::flush();
	quaternion::flush();
}

void instructions::arithmetic_operation(symbol* arg1, token oper, symbol* arg2, symbol*& result)
{
	symbol::value_type type = type_upgrade(arg1, arg2);
	if(type >= symbol::FLOAT)
	{
		if(arg1->get_value_type() < symbol::FLOAT)
			itof(arg1, arg1);
		if(arg2->get_value_type() < symbol::FLOAT)
			itof(arg2, arg2);
		tables.new_temp(result, type);
		if(oper.get_value() == "+")
			new_instruction(generate(quaternion::FADD, arg1, arg2, result));
		else if(oper.get_value() == "-")
			new_instruction(generate(quaternion::FSUB, arg1, arg2, result));
		else if(oper.get_value() == "*")
			new_instruction(generate(quaternion::FMUL, arg1, arg2, result));
		else if(oper.get_value() == "/")
			new_instruction(generate(quaternion::FDIV, arg1, arg2, result));
	}
	else
	{
		tables.new_temp(result, type);
		if(oper.get_value() == "+")
			new_instruction(generate(quaternion::ADD, arg1, arg2, result));
		else if(oper.get_value() == "-")
			new_instruction(generate(quaternion::SUB, arg1, arg2, result));
		else if(oper.get_value() == "*")
			new_instruction(generate(quaternion::MUL, arg1, arg2, result));
		else if(oper.get_value() == "/")
			new_instruction(generate(quaternion::DIV, arg1, arg2, result));
	}
}

void instructions::inverse(symbol* arg1, symbol*& result)
{
	tables.new_temp(result, arg1->get_value_type());
	if(arg1->get_value_type() == symbol::FLOAT)
		new_instruction(generate(quaternion::FINV, arg1, result));
	else
		new_instruction(generate(quaternion::INV, arg1, result));
}

void instructions::assign(symbol* arg1, symbol* arg2)
{
	if(arg1->get_value_type() == symbol::FLOAT && arg2->get_value_type() != symbol::FLOAT)
		itof(arg2, arg2);
	else if(arg2->get_value_type() == symbol::FLOAT && arg1->get_value_type() != symbol::FLOAT)
		ftoi(arg2, arg2);
	new_instruction(generate(quaternion::ASSIGN, arg2, arg1));
}

void instructions::assign(symbol* arg1, symbol* arg2, token oper, symbol* arg3)//增量语句
{
	symbol* result;
	arithmetic_operation(arg2, oper, arg3, result);
	assign(arg1, result);
}

void instructions::add_label(symbol* label)
{
	new_instruction(generate(quaternion::LABEL, label));
}

void instructions::jump(symbol* label)
{
	new_instruction(generate(quaternion::JMP, label));
}

/*
	条件不成立时跳转,翻译成条件成立时跳转,要取反
*/
void instructions::branch(symbol* arg1, token oper, symbol* arg2, symbol*& label)
{
	symbol::value_type type = type_upgrade(arg1, arg2);
	tables.new_label(label);
	if(type >= symbol::FLOAT)
	{
		if(arg1->get_value_type() < symbol::FLOAT)
			itof(arg1, arg1);
		if(arg2->get_value_type() < symbol::FLOAT)
			itof(arg2, arg2);
		if(oper.get_value() == "==")
			new_instruction(generate(quaternion::FJNE, arg1, arg2, label));
		else if(oper.get_value() == "!=")
			new_instruction(generate(quaternion::FJEQ, arg1, arg2, label));
		else if(oper.get_value() == ">")
			new_instruction(generate(quaternion::FJLE, arg1, arg2, label));
		else if(oper.get_value() == ">=")
			new_instruction(generate(quaternion::FJLT, arg1, arg2, label));
		else if(oper.get_value() == "<")
			new_instruction(generate(quaternion::FJGE, arg1, arg2, label));
		else if(oper.get_value() == "<=")
			new_instruction(generate(quaternion::FJGT, arg1, arg2, label));	
	}
	else
	{
		if(oper.get_value() == "==")
			new_instruction(generate(quaternion::JNE, arg1, arg2, label));
		else if(oper.get_value() == "!=")
			new_instruction(generate(quaternion::JEQ, arg1, arg2, label));
		else if(oper.get_value() == ">")
			new_instruction(generate(quaternion::JLE, arg1, arg2, label));
		else if(oper.get_value() == ">=")
			new_instruction(generate(quaternion::JLT, arg1, arg2, label));
		else if(oper.get_value() == "<")
			new_instruction(generate(quaternion::JGE, arg1, arg2, label));
		else if(oper.get_value() == "<=")
			new_instruction(generate(quaternion::JGT, arg1, arg2, label));	
	}
}

void instructions::ftoi(symbol* arg1, symbol*& result)
{
	tables.new_temp(result, symbol::INT);
	new_instruction(generate(quaternion::FTOI, arg1, result));
}

void instructions::itof(symbol* arg1, symbol*& result)
{
	tables.new_temp(result, symbol::FLOAT);
	new_instruction(generate(quaternion::ITOF, arg1, result));
}

void instructions::fun(symbol* fun)
{
	new_instruction(generate(quaternion::FUN, fun));
}

void instructions::param(symbol* param)
{
	new_instruction(generate(quaternion::PARAM, param));
}

void instructions::call(symbol* fun)
{
	new_instruction(generate(quaternion::CALL, fun));
}

void instructions::call(symbol* fun, symbol*& result)
{
	tables.new_temp(result, fun->get_value_type());
	new_instruction(generate(quaternion::CALL, fun, result));
}

void instructions::add_parameter(symbol* arg1)
{
	new_instruction(generate(quaternion::PUSH, arg1));
}

void instructions::ret(symbol* arg1, symbol* arg2)
{
	if(arg1->get_value_type() == symbol::FLOAT && arg2->get_value_type() != symbol::FLOAT)
		itof(arg2, arg2);
	else if(arg2->get_value_type() == symbol::FLOAT && arg1->get_value_type() != symbol::FLOAT)
		ftoi(arg2, arg2);
	new_instruction(generate(quaternion::RET, arg1, arg2));
}

void instructions::ret(symbol* arg1)
{
	new_instruction(generate(quaternion::RET, arg1));
}

void instructions::end_fun(symbol* arg1)
{
	new_instruction(generate(quaternion::ENDF, arg1));
}

void instructions::read(symbol* arg1)
{
	new_instruction(generate(quaternion::READ, arg1));
}

void instructions::write(symbol* arg1)
{
	new_instruction(generate(quaternion::WRITE, arg1));
}

void instructions::new_line()
{
	new_instruction(generate(quaternion::NEW_LINE));
}

std::vector<quaternion*> instructions::get_sequential() const
{
	return this->sequential;
}

std::vector<function_block*> instructions::get_function_blocks() const
{
	return this->blocks;
}

ostream& operator<<(ostream& out, const instructions& ins)
{
	for(unsigned int i = 0; i < ins.blocks.size(); i++)
		out << *ins.blocks[i];
	return out;
}

void instructions::new_instruction(quaternion* q)
{	
	assert(_NOT_NULL(q));
	/*
		FUN是一个基本块的开始，同时也表示了ENDF是前一个基本块的结束
	*/
	if(q->get_operation() == quaternion::FUN)
	{
		this->blocks.push_back(new function_block());
	}
	update_reference(q);
	blocks.back()->add(q);
	sequential.push_back(q);
}

quaternion* instructions::operator[](unsigned int index)
{
	return sequential[index];
}

void instructions::update_reference(quaternion* q)
{
	std::vector<symbol*>& source_ref = q->get_source_args();
	symbol* object_ref = q->get_object_args();
	for(unsigned int i = 0; i < source_ref.size(); i++)
	{
		if(!utility::is_literal(source_ref[i]))
			source_ref[i]->get_scope()->update_reference(source_ref[i], 1);
	}
	if(object_ref != NULL && !utility::is_literal(object_ref))
		object_ref->get_scope()->update_reference(object_ref, 1);
}

#undef _NOT_NULL