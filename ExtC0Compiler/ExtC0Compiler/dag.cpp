#include "dag.h"
#include "utility.h"
#include <cassert>

#define _NOT_NULL(arg) (arg != NULL)
#define _SHOULD_NOT_HAPPEN 0
#define _IS_SYMBOL(arg) (arg->is_leaf())
#define _IS_OPERATION(arg) (!arg->is_leaf())

namespace
{
	bool find_symbol(symbol* arg, std::map<symbol*, unsigned int>& symbols, unsigned int& index)
	{
		std::map<symbol*, unsigned int>::iterator iter = symbols.find(arg);
		if(iter != symbols.end())
		{
			index = iter->second;
			return true;
		}
		else
			return false;
	}

	/*
		��operators��Ѱ�Ҳ���Ϊoper,���Ӻ��Һ��ӷֱ�Ϊleft��right�Ľڵ㣬���±걣����index��
	*/
	bool find_operator(quaternion::operation oper, std::map<quaternion::operation, std::set<unsigned int>>& operators, std::vector<dag_node*>& nodes, unsigned int left, unsigned int right, unsigned int& index)
	{
		std::map<quaternion::operation, std::set<unsigned int>>::iterator iter = operators.find(oper);
		if(iter != operators.end())
		{
			std::set<unsigned int>& list = iter->second;
			for(std::set<unsigned int>::iterator i = list.begin(); i != list.end(); i++)
			{
				if(nodes[*i]->get_left() == left && nodes[*i]->get_right() == right)
				{
					index = *i;
					return true;
				}
			}
			return false;
		}
		else
			return false;
	}

	bool find_operator(quaternion::operation oper, std::map<quaternion::operation, std::set<unsigned int>>& operators, std::vector<dag_node*>& nodes, unsigned int left, unsigned int& index)
	{
		std::map<quaternion::operation, std::set<unsigned int>>::iterator iter = operators.find(oper);
		if(iter != operators.end())
		{
			std::set<unsigned int>& list = iter->second;
			for(std::set<unsigned int>::iterator i = list.begin(); i != list.end(); i++)
			{
				if(nodes[*i]->get_left() == left)
				{
					index = *i;
					return true;
				}
			}
			return false;
		}
		else
			return false;
	}

	bool no_parent(std::vector<dag_node*>& nodes, unsigned int index)
	{
		for(unsigned int i = 0; i < nodes.size(); i++)
		{
			if(!nodes[i]->is_enabled())continue;
			if(!nodes[i]->is_leaf())
			{
				if(nodes[i]->get_left() == index)
					return false;
				if(nodes[i]->has_two_children() && nodes[i]->get_right() == index)
					return false;
			}
		}
		return true;
	}

	/*
		��ýڵ���Ϊindex�Ľڵ����ҪĿ���������������object_operands��
		���object_operands���иýڵ㣬��ֱ�ӷ���
		����ýڵ��Ƿ��Žڵ㣬���������
		���򣬱�������һ������share�ýڵ㣬����ѡ�����ʱ��������
	*/

	quaternion* new_instruction(quaternion::operation oper, symbol* arg1, symbol* arg2, symbol* arg3)
	{
		switch(oper)
		{
		case quaternion::ADD:
		case quaternion::SUB:
		case quaternion::MUL:
		case quaternion::DIV:
		case quaternion::FADD:
		case quaternion::FSUB:
		case quaternion::FMUL:
		case quaternion::FDIV:
			return quaternion::new_quaternion(oper, arg1, arg2, arg3);
		case quaternion::FTOI:
		case quaternion::ITOF:
		case quaternion::INV:
		case quaternion::FINV:
		case quaternion::ASSIGN:
			return quaternion::new_quaternion(oper, arg1, arg3);
		default:
			assert(_SHOULD_NOT_HAPPEN);
			return NULL;
		}
	}
}

dag::dag(instructions*& ins)
{
	build(ins);
}

void dag::build(instructions*& ins)
{
	instructions* new_ins = new instructions(ins->tables);
	last_oper = 0;
	for(unsigned int i = 0; i < ins->blocks.size(); i++)
	{
		function_block* block = ins->blocks[i];
		for(unsigned int j = 0; j < block->blocks.size(); j++)
		{
			basic_block* b_block = block->blocks[j];
			for(unsigned int k = 0; k < b_block->ins.size(); k++)
			{
				quaternion* q = b_block->ins[k];
				quaternion* nq = NULL;
				quaternion::operation oper = q->get_operation();
				symbol* arg1 = q->get_arg(0);
				symbol* arg2 = q->get_arg(1);
				symbol* arg3 = q->get_arg(2);
				unsigned int index1;
				unsigned int index2;
				unsigned int index3;
				switch(oper)
				{
				case quaternion::ASSIGN:
					if(!find_symbol(arg1, symbols, index1))
					{
						index1 = nodes.size();
						nodes.push_back(new dag_node(arg1));
						renew_sharing(arg1, index1);
					}
					renew_sharing(arg2, index1);
					assign_info[last_oper].push_back(std::pair<symbol*, unsigned int>(arg2, index1));
					break;
				//��Ԫ����
				case quaternion::ADD:
				case quaternion::SUB:
				case quaternion::MUL:
				case quaternion::DIV:
				case quaternion::FADD:
				case quaternion::FSUB:
				case quaternion::FMUL:
				case quaternion::FDIV:
					if(!find_symbol(arg1, symbols, index1))
					{
						index1 = nodes.size();
						nodes.push_back(new dag_node(arg1));
						renew_sharing(arg1, index1);
					}
					if(!find_symbol(arg2, symbols, index2))
					{
						index2 = nodes.size();
						nodes.push_back(new dag_node(arg2));
						renew_sharing(arg2, index2);
					}
					if(!find_operator(oper, operators, nodes, index1, index2, index3))
					{
						index3 = nodes.size();
						nodes.push_back(new dag_node(oper, index1, index2));
						operators[oper].insert(index3);
						last_oper = nodes.size();
					}
					renew_sharing(arg3, index3);
					assign_info[last_oper].push_back(std::pair<symbol*, unsigned int>(arg3, index3));
					break;
				//��Ԫ����
				case quaternion::FTOI:
				case quaternion::ITOF:
				case quaternion::INV:
				case quaternion::FINV:
					if(!find_symbol(arg1, symbols, index1))
					{
						index1 = nodes.size();
						nodes.push_back(new dag_node(arg1));
						renew_sharing(arg1, index1);
					}
					if(!find_operator(oper, operators, nodes, index1, index2))
					{
						index2 = nodes.size();
						nodes.push_back(new dag_node(oper, index1));
						operators[oper].insert(index2);
						last_oper = nodes.size();
					}
					renew_sharing(arg2, index2);
					assign_info[last_oper].push_back(std::pair<symbol*, unsigned int>(arg2, index2));
					break;
				case quaternion::CALL:
				case quaternion::RET:
					export_quaternions(*new_ins);
					if(arg2 == NULL)
						nq = quaternion::new_quaternion(oper, arg1);
					else
						nq = quaternion::new_quaternion(oper, arg1, arg2);
					new_ins->new_instruction(nq);
					break;
				case quaternion::FUN:
				case quaternion::PARAM:
				case quaternion::ENDF:
				case quaternion::JMP:
				case quaternion::LABEL:
				case quaternion::READ:
				case quaternion::WRITE:
				case quaternion::PUSH:
					export_quaternions(*new_ins);
					nq = quaternion::new_quaternion(oper, arg1);
					new_ins->new_instruction(nq);
					break;
				case quaternion::JGT:
				case quaternion::JGE:
				case quaternion::JLT:
				case quaternion::JLE:
				case quaternion::JEQ:
				case quaternion::JNE:
				case quaternion::FJGT:
				case quaternion::FJGE:
				case quaternion::FJLT:
				case quaternion::FJLE:
				case quaternion::FJEQ:
				case quaternion::FJNE:
					export_quaternions(*new_ins);
					nq = quaternion::new_quaternion(oper, arg1, arg2, arg3);					
					new_ins->new_instruction(nq);
					break;
				case quaternion::NEW_LINE:
                    export_quaternions(*new_ins);
                    nq = quaternion::new_quaternion(oper);                                  
                    new_ins->new_instruction(nq);
                    break;
				default:
					assert(_SHOULD_NOT_HAPPEN);
				}
				q->disable();
			}
			export_quaternions(*new_ins);
		}
		export_quaternions(*new_ins);
	}
	//�ͷ�ins;
	delete ins;
	ins = new_ins;
}

void dag::export_quaternions(instructions& ins)
{
	//�����Ĳ�������ջ
	std::vector<unsigned int> oper_stack;

	//ĳ���ڵ�Ľ������ڵ㣬���������ڸýڵ�ķ��ţ���Ӧ��ʹ��assign��ø���
	std::map<unsigned int, symbol*> object_operands;

	//for(std::map<symbol*, unsigned int>::iterator iter = symbols.begin(); iter != symbols.end(); iter++)
	//	symbols_sharing_operand[iter->second].push_back(iter->first);

	//����2
	while(!operators.empty())
	{
		bool found = false;
		unsigned int node_index = 0;
		//����3
		for(std::map<quaternion::operation, std::set<unsigned int>>::iterator iter = operators.begin(); iter != operators.end(); iter++)
		{
			std::set<unsigned int>& node_set = iter->second;
			for(std::set<unsigned int>::iterator iter1 = node_set.begin(); iter1 != node_set.end(); iter1++)
			{
				if(no_parent(nodes, *iter1) && (*iter1 > node_index || !found))
				{
					node_index = *iter1;
					found = true;
					//break;
				}
			}
			//if(found)break;
		}
		if(found)
		{
			//����4
			do
			{
				std::map<quaternion::operation, std::set<unsigned int>>::iterator iter = operators.find(nodes[node_index]->get_operation());
				assert(iter != operators.end());

				//ɾ���Ѿ���ջ�Ľڵ�
				std::set<unsigned int>& node_set = iter->second;
				std::set<unsigned int>::iterator iter1 = node_set.find(node_index);
				node_set.erase(iter1);
				if(node_set.empty())operators.erase(iter);

				oper_stack.push_back(node_index);
				nodes[node_index]->set_enable(false);
				node_index = nodes[node_index]->get_left();
			}
			while(!nodes[node_index]->is_leaf() && no_parent(nodes, node_index));
		}
		else
		{
			assert(operators.empty());
		}
	}
	
	/*
		����ջ�����µ���Ԫʽ������5
	*/

	sharing_assign(ins, object_operands, 0);

	for(unsigned int i = oper_stack.size(); i > 0; i--)
	{
		dag_node* node = nodes[oper_stack[i - 1]];
		quaternion::operation oper = node->get_operation();
		quaternion* q = NULL;
		symbol* arg1 = NULL;
		symbol* arg2 = NULL;
		symbol* arg3 = NULL;

		assert(_IS_OPERATION(node));
		arg1 = get_primative_operand(object_operands, node->get_left());
		if(node->has_two_children())
			arg2 = get_primative_operand(object_operands, node->get_right());
		arg3 = get_primative_operand(object_operands, oper_stack[i - 1]);
		update_origin(arg3, oper_stack[i - 1]);
		q = new_instruction(oper, arg1, arg2, arg3);
		ins.new_instruction(q);
		sharing_assign(ins, object_operands, oper_stack[i - 1] + 1);
	}
	flush();
}

void dag::flush()
{
	symbols.clear();
	operators.clear();
	nodes.clear();
	parents.clear();
	symbols_sharing_operand.clear();
	assign_info.clear();
	where_origin_is.clear();
	last_oper = 0;
}

void dag::renew_sharing(symbol* sym, unsigned int index)
{
	symbols_sharing_operand[index].push_back(sym);
	if(symbols.find(sym) != symbols.end())
	{
		unsigned int i = symbols[sym];
		std::vector<symbol*>::iterator iter = symbols_sharing_operand[i].begin();
		while(*iter != sym && iter != symbols_sharing_operand[i].end())
			iter++;
		assert(iter != symbols_sharing_operand[i].end());
		symbols_sharing_operand[i].erase(iter);
	}
	symbols[sym] = index;
}

symbol* dag::get_primative_operand(std::map<unsigned int, symbol*>& object_operands, unsigned int index)
{
	std::map<unsigned int, symbol*>::iterator iter = object_operands.find(index);
	if(iter != object_operands.end())
	{
		return iter->second;
	}
	else if(nodes[index]->is_leaf())
	{
		std::multimap<symbol*, symbol*>::iterator lower = where_origin_is.lower_bound(nodes[index]->get_symbol());
		std::multimap<symbol*, symbol*>::iterator upper = where_origin_is.upper_bound(nodes[index]->get_symbol());
		if(lower != upper)
			return lower->second;
		else
			return nodes[index]->get_symbol();
	}
	else
	{
		std::map<unsigned int, std::vector<symbol*>>::iterator iter1 = symbols_sharing_operand.find(index);
		assert(iter1 != symbols_sharing_operand.end());
		std::vector<symbol*>& list = iter1->second;
		for(unsigned int i = 0; i < list.size(); i++)
		{
			//���ȷ��ط���ʱ����
			if(!utility::is_temp(list[i]))
			{
				return object_operands[index] = list[i];
			}
		}
		return object_operands[index] = list[0];
	}
	assert(_SHOULD_NOT_HAPPEN);
	return NULL;
}

void dag::update_origin(symbol* sym, unsigned int index)
{
	for(std::multimap<symbol*, symbol*>::iterator iter = where_origin_is.begin(); iter != where_origin_is.end(); iter++)
	{
		if(iter->second == sym)
		{
			where_origin_is.erase(iter);
			break;
		}
	}
	if(nodes[index]->is_leaf())
	{
		symbol* source = nodes[index]->get_symbol();
		where_origin_is.insert(std::pair<symbol*, symbol*>(source, sym));
	}

}

void dag::sharing_assign(instructions& ins, std::map<unsigned int, symbol*>& object_operands, unsigned int index)
{
	std::vector<std::pair<symbol*, unsigned int>> iter = assign_info[index];
	for(unsigned int i = 0; i < iter.size(); i++)
	{
		symbol* arg1 = iter[i].first;
		symbol* arg2 = get_primative_operand(object_operands, iter[i].second);
		update_origin(arg1, iter[i].second);
		if(arg1 != arg2)
			ins.assign(arg1, arg2);
	}
}

#undef _NOT_NULL
#undef _SHOULD_NOT_HAPPEN 
#undef _IS_SYMBOL
#undef _IS_OPERATION