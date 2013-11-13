#include "constance_propagation.h"
#include "symbol.h"
#include <string>
#include <sstream>

namespace
{
	std::string get_string(int value)
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << value;
		return ss.str();
	}

	std::string get_string(float value)
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << value;
		return ss.str();
	}

	int get_int_value(std::string s)
	{
		int value;
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << s;
		ss >> value;
		return value;
	}

	float get_float_value(std::string s)
	{
		float value;
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << s;
		ss >> value;
		return value;
	}

	//可能含有常数的操作
	bool may_contain_constance(quaternion::operation oper)
	{
		return
		   (oper == quaternion::INV ||
			oper == quaternion::ADD ||
			oper == quaternion::SUB ||
			oper == quaternion::MUL ||
			oper == quaternion::DIV ||
			oper == quaternion::FDIV ||
			oper == quaternion::FADD ||
			oper == quaternion::FSUB ||
			oper == quaternion::FMUL ||
			oper == quaternion::FINV ||
			oper == quaternion::FTOI ||
			oper == quaternion::ITOF ||
			oper == quaternion::ASSIGN);
	}

	bool is_changed(quaternion* q, unsigned int arg)
	{
		return false;
	}
}

void constance_propagation::propagate(instructions& ins)
{
	for(unsigned int i = 0; i < ins.sequential.size(); i++)
	{
		quaternion* q = ins.sequential[i];
		symbol* arg1 = q->get_arg(0);
		symbol* arg2 = q->get_arg(1);
		symbol* arg3 = q->get_arg(2);
		//switch(q->get_operation())
		//{
		//case quaternion::INV:
		//	if(arg1->get_symbol_type() == symbol::LITERAL || arg1->get_symbol_type() == symbol::CONST)
		//	{
		//		//获得其数值
		//		int value = -get_int_value(arg1->get_value());
		//		std::vector<std::pair<unsigned int, unsigned int>>& ref = ins.reference.find(arg2)->second;
		//		for(unsigned int j = 0; j < ref.size(); j++)
		//		{
		//			//只能更改之后的引用，并且值改变之前的引用
		//			if(ref[j].first > i)
		//			{
		//				if(!is_changed(ins.sequential[ref[j].first], ref[j].second))
		//				{
		//					symbol* s = symbol::literal_symbol(symbol::INT, get_string(value));
		//					ins.sequential[ref[j].first]->set_arg(ref[j].second, s);
		//					ref[j].first = -1;
		//				}
		//				else
		//				{
		//					break;
		//				}
		//			}
		//		}

		//		q->set_enable(false);
		//	}
		//	break;
		//case quaternion::FINV:
		//	break;
		//case quaternion::FTOI:
		//	break;
		//case quaternion::ITOF:
		//	break;
		//case quaternion::ADD:
		//case quaternion::SUB:
		//case quaternion::MUL:
		//case quaternion::DIV:
		//	break;
		//case quaternion::FADD:
		//case quaternion::FSUB:
		//case quaternion::FMUL:
		//case quaternion::FDIV:
		//	break;
		//}
	}
}
