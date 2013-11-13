#include "ast.h"
#include "error.h"
#include "error_list.h"
#include <string>

/*
	���򣬱����ó���ĸ�������
*/
void ast::program::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	tables.new_symbol_table(symbol::function_symbol(symbol::VOID, "@global", NULL));
	node::visit(tables, ins, el);
	tables.pop_symbol_table();
}

/*
	�������壬���Ȼ��һ�����ͱ�ʶ����Ȼ�������������������
*/
void ast::constance_definition::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol::value_type type;

	(*iter)->visit(tables, ins, el, type); //���ͱ�ʶ�����ۺ�����
	iter++;

	for(; iter != this->children.end(); iter++)
		(*iter)->visit(tables, ins, el, type);//�������������̳�����
}

/*
	���������������һ����ʶ������������ֵ��������ӽ����ű�
*/
void ast::constance_declarator::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol::value_type& type)
{
	std::vector<node*>::iterator iter = this->children.begin();
	std::string id;
	symbol* value;

	(*iter)->visit(tables, ins, el, id); //��ʶ�����ۺ�����
	iter++;
	(*iter)->visit(tables, ins, el, value); //����ֵ���ۺ�����
	iter++;

	//�������Ͳ�ƥ��
	if(type != value->get_value_type())
	{
		el.add_error(error::CONSTANCE_TYPE_MISMATCH, id + " = " + value->get_value(), this->get_line());
	}

	//�ظ�����
	if(!tables.add_symbol(type, id, value->get_value()))
	{
		el.add_error(error::DUPLICATED_CONSTANCE_DECLARATION, id, this->get_line());
	}
}

/*
	�������壬���һ�����ͱ�ʶ����Ȼ�����������ʶ��������ӽ����ű�
*/
void ast::variable_definition::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol::value_type type;

	(*iter)->visit(tables, ins, el, type); //���ͱ�ʶ�����ۺ�����
	iter++;
	
	for(; iter != this->children.end(); iter++)
	{
		std::string id;
		(*iter)->visit(tables, ins, el, id);//��ʶ�����ۺ�����
		if(!tables.add_symbol(type, id))
		{
			el.add_error(error::DUPLICATED_VARIABLE_DECLARATION, id, this->get_line());
		}
	}
}

void ast::function_definition::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol::value_type type;
	std::string id;
	symbol::arguments* args = new symbol::arguments();
	symbol* fun;

	(*iter)->visit(tables, ins, el, type);//���ͱ�ʶ�����ۺ�����
	iter++;
	(*iter)->visit(tables, ins, el, id);//��ʶ�����ۺ�����
	iter++;
	
	if(!tables.add_symbol(type, id, args, fun))
	{
		el.add_error(error::DUPLICATED_FUNCTION_DECLARATION, id, this->get_line());
	}
	ins.fun(fun);
	tables.new_symbol_table(fun);
	
	(*iter)->visit(tables, ins, el, args);//�βΣ��ۺ�����
	iter++;
	(*iter)->visit(tables, ins, el);//������
	iter++;
	
	ins.end_fun(fun);
	tables.pop_symbol_table();
}

void ast::argument_declaration::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol::arguments* args)
{
	for(std::vector<node*>::iterator iter = this->children.begin(); iter != this->children.end(); iter++)
	{
		(*iter)->visit(tables, ins, el, args);//�������������ۺ�����
	}
}

void ast::argument_declarator::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol::arguments* args)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol::value_type type;
	std::string id;

	(*iter)->visit(tables, ins, el, type);//���ͱ�ʶ�����ۺ�����
	iter++;
	(*iter)->visit(tables, ins, el, id);//��ʶ�����ۺ�����
	iter++;

	if(!tables.add_argument(type, id, args))
	{
		el.add_error(error::DUPLICATED_ARGUMENT_DECLARATION, id, this->get_line());
	}
	ins.param(args->back());
}

void ast::procedure_definition::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	std::string id;
	symbol::arguments* args = new symbol::arguments();
	symbol* fun;

	(*iter)->visit(tables, ins, el, id);//��ʶ�����ۺ�����
	iter++;
	
	if(!tables.add_symbol(symbol::VOID, id, args, fun))
	{
		el.add_error(error::DUPLICATED_FUNCTION_DECLARATION, id, this->get_line());
	}
	ins.fun(fun);
	tables.new_symbol_table(fun);
	
	(*iter)->visit(tables, ins, el, args);//�βΣ��ۺ�����
	iter++;
	(*iter)->visit(tables, ins, el);//������
	iter++;
	
	ins.end_fun(fun);
	tables.pop_symbol_table();
}

void ast::main_function::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* main;

	tables.add_symbol(symbol::VOID, "main", new symbol::arguments(), main);	
	ins.fun(main);
	tables.new_symbol_table(main);
	
	(*iter)->visit(tables, ins, el);//������
	iter++;
	
	ins.end_fun(main);
	tables.pop_symbol_table();
}

void ast::expression::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& result)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* arg1;
	symbol* arg2;
	token oper("+", token::ADD_OPERATOR);

	//��ǰ��������
	if(!(this->children.size() % 2))
	{
		(*iter)->visit(tables, ins, el, oper);
		iter++;
	}
	(*iter)->visit(tables, ins, el, arg1); //��ۺ�����
	iter++;
	if(oper.get_value() == "-")
		ins.inverse(arg1, arg1);
	result = arg1;
	while(iter != this->children.end())
	{
		(*iter)->visit(tables, ins, el, oper); //��������ۺ�����
		iter++;
		(*iter)->visit(tables, ins, el, arg2); //��ۺ�����
		iter++;
		ins.arithmetic_operation(arg1, oper, arg2, result);
		arg1 = result;
	}
}

void ast::term::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& result)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* arg1;
	symbol* arg2;
	token oper;

	(*iter)->visit(tables, ins, el, arg1);//���ӣ��ۺ�����
	iter++;
	result = arg1;
	while(iter != this->children.end())
	{
		(*iter)->visit(tables, ins, el, oper);//��������ۺ�����
		iter++;
		(*iter)->visit(tables, ins, el, arg2);//���ӣ��ۺ�����
		iter++;
		ins.arithmetic_operation(arg1, oper, arg2, result);
		arg1 = result;
	}
}

void ast::factor::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& result)
{
	std::vector<node*>::iterator iter = this->children.begin();
	(*iter)->visit(tables, ins, el, result);//��ʶ�����������á����ʽ������ֵ���ۺ�����
	iter++;
}

void ast::assign_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* arg1;
	symbol* arg2;

	(*iter)->visit(tables, ins, el, arg1); //��ʶ�����ۺ�����
	iter++;
	(*iter)->visit(tables, ins, el, arg2); //���ʽ���ۺ�����
	iter++;

	if(arg1->get_symbol_type() != symbol::VARIABLE && arg1->get_symbol_type() != symbol::ARGUMENT)
	{
		el.add_error(error::INVALID_LEFT_VALUE, arg1->get_id(), this->get_line());
	}
	if(arg1->get_value_type() != arg2->get_value_type())
	{
		el.add_warning(error::ASSIGN_TYPE_MISMATCH, arg1->get_id(), this->get_line());
	}

	ins.assign(arg1, arg2);
}

void ast::condition_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* else_label;
	symbol* end_label;
	
	(*iter)->visit(tables, ins, el, else_label); //�������ۺ�����
	iter++;

	
	(*iter)->visit(tables, ins, el);	//��� then����
	iter++;
	
	if(iter != this->children.end())//else����
	{
		tables.new_label(end_label);
		ins.jump(end_label);
		ins.add_label(else_label);

		(*iter)->visit(tables, ins, el);//���
		iter++;
		
		ins.add_label(end_label);
	}
	else
	{
		ins.add_label(else_label);
	}
}

void ast::condition::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& else_label)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* arg1;
	token oper;
	symbol* arg2;

	(*iter)->visit(tables, ins, el, arg1); //���ʽ���ۺ�����
	iter++;
	if(iter != this->children.end())
	{
		(*iter)->visit(tables, ins, el, oper);//��������ۺ�����
		iter++;
		(*iter)->visit(tables, ins, el, arg2);//���ʽ���ۺ�����
		iter++;
	}
	else
	{
		oper = token("!=", token::RELATION_OPERATOR);
		arg2 = symbol::literal_symbol(symbol::INT, "0");
		tables.add_literal(arg2);
	}
	ins.branch(arg1, oper, arg2, else_label);
}

void ast::while_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* loop_label;
	symbol* end_label;

	tables.new_label(loop_label);
	ins.add_label(loop_label);

	(*iter)->visit(tables, ins, el, end_label);//�������ۺ�����
	iter++;

	(*iter)->visit(tables, ins, el);//���

	ins.jump(loop_label);
	ins.add_label(end_label);
}

void ast::for_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* loop_label;
	symbol* end_label;

	(*iter)->visit(tables, ins, el); //��ֵ���
	iter++;

	tables.new_label(loop_label);
	ins.add_label(loop_label);

	(*iter)->visit(tables, ins, el, end_label);//�������ۺ�����
	iter++;

	node* increasement = *iter;
	iter++;
	
	(*iter)->visit(tables, ins, el);//���
	iter++;

	increasement->visit(tables, ins, el); //�������

	ins.jump(loop_label);
	ins.add_label(end_label);
}

void ast::increase_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* arg1;
	symbol* arg2;
	token oper;
	symbol* step;

	(*iter)->visit(tables, ins, el, arg1);//��ʶ�����ۺ�����
	iter++;

	(*iter)->visit(tables, ins, el, arg2);//��ʶ�����ۺ�����
	iter++;

	(*iter)->visit(tables, ins, el, oper);//��������ۺ�����
	iter++;

	(*iter)->visit(tables, ins, el, step);//����ֵ���ۺ�����
	iter++;

	if(step->get_value() == "0")
	{
		el.add_error(error::ZERO_STEP, step->get_value(), this->get_line());
	}
	ins.assign(arg1, arg2, oper, step);
}

void ast::invoke_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* id;
	symbol::arguments* params = new symbol::arguments();

	(*iter)->visit(tables, ins, el, id);//��ʶ��
	iter++;

	//Ϊ��ʹ�βα���֪������ԭ�ͣ����Ӹò���
	params->push_back(id);
	(*iter)->visit(tables, ins, el, params);//ʵ�α�
	iter++;

	ins.call(id);
}

void ast::invoke_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& result)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* id;
	symbol::arguments* params = new symbol::arguments();

	(*iter)->visit(tables, ins, el, id);//��ʶ��
	iter++;

	params->push_back(id);
	(*iter)->visit(tables, ins, el, params);//ʵ�α�
	iter++;

	if(id->get_symbol_type() == symbol::FUNCTION && id->get_value_type() == symbol::VOID)
	{
		el.add_error(error::NO_RETURN_VALUE, id->get_id(), this->get_line());
	}
	ins.call(id, result);
}

void ast::parameter_list::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol::arguments* params)
{
	symbol* id = params->back();
	symbol::arguments* args;
	bool flag = true;
	params->pop_back();
	if(id->get_symbol_type() == symbol::FUNCTION)
	{
		args = id->get_arguments();
		symbol::arguments::reverse_iterator iter1 = args->rbegin();
		for(std::vector<node*>::reverse_iterator iter = this->children.rbegin(); iter != this->children.rend(); iter++)
		{
			symbol* arg;
			(*iter)->visit(tables, ins, el, arg);//���ʽ���ۺ�����
			params->insert(params->begin(), arg);
			if(iter1 != args->rend())
			{
				//TODO, UTILITY
				symbol::value_type type1 = arg->get_value_type();
				symbol::value_type type2 = (*iter1)->get_value_type();
				if(type1 == symbol::FLOAT && type2 != symbol::FLOAT)
				{
					ins.ftoi(arg, arg);
					el.add_warning(error::ARGUMENT_TYPE_MISMATCH, arg->get_id(), this->get_line());
				}
				else if(type1 != symbol::FLOAT && type2 == symbol::FLOAT)
				{
					ins.itof(arg, arg);
					el.add_warning(error::ARGUMENT_TYPE_MISMATCH, arg->get_id(), this->get_line());
				}
				iter1++;
			}
			ins.add_parameter(arg);
		}
		if(args->size() != params->size())
		{
			//����������һ��
			el.add_error(error::ARGUMENT_AMOUNT_MISMATCH, id->get_id(), this->get_line());
		}
	}
	else
	{
		//��������Ϊ��������
		el.add_error(error::USE_VARIABLE_AS_FUNCTION, id->get_id(), this->get_line());
	}
}

void ast::read_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	for(std::vector<node*>::iterator iter = this->children.begin(); iter != this->children.end(); iter++)
	{
		symbol* args;
		(*iter)->visit(tables, ins, el, args);//��ʶ�����ۺ�����
		ins.read(args);
	}
}

void ast::write_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	for(std::vector<node*>::iterator iter = this->children.begin(); iter != this->children.end(); iter++)
	{
		symbol* args;
		(*iter)->visit(tables, ins, el, args);//��ʶ�����ַ������ۺ�����
		ins.write(args);
	}
	ins.new_line();
}

void ast::return_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* scope = tables.get_scope();
	if(iter != this->children.end())
	{
		symbol* arg;
		(*iter)->visit(tables, ins, el, arg);
		iter++;
		if(arg->get_value_type() != scope->get_value_type())
		{
			el.add_warning(error::RETURN_TYPE_MISMATCH, scope->get_id(), this->get_line());
		}
		if(scope->get_value_type() == symbol::VOID)
		{
			el.add_error(error::CAN_NOT_RET_VALUE, scope->get_id(), this->get_line());
		}
		ins.ret(scope, arg);
	}
	else
	{
		if(scope->get_value_type() != symbol::VOID)
		{
			el.add_error(error::NEED_RET_VALUE, scope->get_id(), this->get_line());
		}
		ins.ret(scope);
	}

}