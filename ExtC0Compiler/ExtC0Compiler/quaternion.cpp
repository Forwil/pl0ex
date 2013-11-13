#include "quaternion.h"
#include "utility.h"
#include <iostream>
#include <iomanip>
#include <cassert>

#define _ARG_ERROR 0
#define _NOT_NULL(arg) (arg != NULL)

const std::string QUATERNION_TYPE_STRING_MAP[]=
{
	"ASSIGN",
	"INV",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"FINV",
	"FADD",
	"FSUB",
	"FMUL",
	"FDIV",
	"FTOI",
	"ITOF",
	"JGT",
	"JGE",
	"JLT",
	"JLE",
	"JEQ",
	"JNE",
	"FJGT",
	"FJGE",
	"FJLT",
	"FJLE",
	"FJEQ",
	"FJNE",
	"JMP",
	"FUN",
	"CALL",
	"PUSH",
	"PARAM",
	"RET",
	"ENDF",
	"LABEL",
	"READ",
	"WRITE",
	"NEWLINE",
};

unsigned int quaternion::instance_number = 0;

quaternion::quaternion()
{
	enabled = true;
	object_args = NULL;
	number = instance_number++;
}

quaternion* quaternion::new_quaternion(operation _oper, symbol* _arg1, symbol* _arg2, symbol* _arg3)
{
	assert(_NOT_NULL(_arg1));
	assert(_NOT_NULL(_arg2));
	assert(_NOT_NULL(_arg3));
	quaternion* q = new quaternion();
	switch(_oper)
	{
	case ADD:
	case SUB:
	case MUL:
	case DIV:
	case FADD:
	case FSUB:
	case FMUL:
	case FDIV:
		q->source_args.push_back(_arg1);
		q->source_args.push_back(_arg2);
		q->object_args = _arg3;
		break;
	case JGT:
	case JGE:
	case JLT:
	case JLE:
	case JEQ:
	case JNE:
	case FJGT:
	case FJGE:
	case FJLT:
	case FJLE:
	case FJEQ:
	case FJNE:
		q->source_args.push_back(_arg1);
		q->source_args.push_back(_arg2);
		q->source_args.push_back(_arg3);
		break;
	default:
		assert(_ARG_ERROR);
	}
	q->oper = _oper;
	q->args.push_back(_arg1);
	q->args.push_back(_arg2);
	q->args.push_back(_arg3);
	return q;
}

//三元
quaternion* quaternion::new_quaternion(operation _oper, symbol* _arg1, symbol* _arg2)
{
	assert(_NOT_NULL(_arg1));
	assert(_NOT_NULL(_arg2));
	quaternion* q = new quaternion();
	switch(_oper)
	{
	case ASSIGN:
	case INV:
	case FINV:
	case FTOI:
	case ITOF:
		q->source_args.push_back(_arg1);
		q->object_args = _arg2;
		break;
	case CALL:
		q->source_args.push_back(_arg1);
		q->object_args = _arg2;
		break;
	case RET:
		q->source_args.push_back(_arg1);
		q->source_args.push_back(_arg2);
		break;
	default:
		assert(_ARG_ERROR);
	}
	q->oper = _oper;
	q->args.push_back(_arg1);
	q->args.push_back(_arg2);
	return q;
}

//二元
quaternion* quaternion::new_quaternion(operation _oper, symbol* _arg1)
{
	assert(_NOT_NULL(_arg1));
	quaternion* q = new quaternion();
	switch(_oper)
	{
	case FUN:
	case JMP:
	case ENDF:
	case CALL:
	case RET:
	case LABEL:
		q->source_args.push_back(_arg1);
		break;
	case PUSH:
	case WRITE:
		q->source_args.push_back(_arg1);
		break;
	case PARAM:
		q->object_args = _arg1;
		break;
	case READ:
		q->object_args = _arg1;
		break;
	default:
		assert(_ARG_ERROR);
	}
	q->oper = _oper;
	q->args.push_back(_arg1);
	return q;
}

quaternion* quaternion::new_quaternion(operation _oper)
{
	quaternion* q = new quaternion();
	switch(_oper)
	{
	case NEW_LINE:
		break;
	default:
		assert(_ARG_ERROR);
	}
	q->oper = _oper;
	return q;
}

symbol* quaternion::get_arg(unsigned int index) const
{
	if(index < args.size())
		return args[index];
	else
		return NULL;
}

quaternion::operation quaternion::get_operation() const
{
	return oper;
}

std::vector<symbol*> quaternion::get_source_args() const
{
	return source_args;
}

symbol* quaternion::get_object_args() const 
{
	return object_args;
}

bool quaternion::is_enable() const 
{
	return enabled;
}

unsigned int quaternion::get_number() const
{
	return number;
}

void quaternion::disable()
{
	std::vector<symbol*>& source_ref = get_source_args();
	symbol* object_ref = get_object_args();
	for(unsigned int i = 0; i < source_ref.size(); i++)
	{
		if(!utility::is_literal(source_ref[i]))
			source_ref[i]->get_scope()->update_reference(source_ref[i], -1);
	}
	if(object_ref != NULL && !utility::is_literal(object_ref))
		object_ref->get_scope()->update_reference(object_ref, -1);
	enabled = false;
}

void quaternion::flush()
{
	instance_number = 0;
}

void operator<<(std::ostream& out, const quaternion& q)
{
	if(!q.enabled) out << "/* ";
	out << std::setiosflags(std::ios_base::left) << std::setw(4) << q.number << ": ";
	out << std::setiosflags(std::ios_base::left) << std::setw(16) << QUATERNION_TYPE_STRING_MAP[q.oper];
	for(unsigned int i = 0; i < q.args.size(); i++)
	{
		if(utility::is_literal(q.args[i]))
			out << std::setw(8) << q.args[i]->get_value();
		else
			out << std::setw(8) << q.args[i]->get_id();
	}
	if(!q.enabled) out << " */";
	out << std::endl;
}

#undef _ARG_ERROR
