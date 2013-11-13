#include "ast.h"
#include "error.h"
#include "error_list.h"
#include <string>

/*
	程序，遍历该程序的各个部分
*/
void ast::program::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	tables.new_symbol_table(symbol::function_symbol(symbol::VOID, "@global", NULL));
	node::visit(tables, ins, el);
	tables.pop_symbol_table();
}

/*
	常量定义，首先获得一个类型标识符，然后遍历各个常量定义器
*/
void ast::constance_definition::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol::value_type type;

	(*iter)->visit(tables, ins, el, type); //类型标识符，综合属性
	iter++;

	for(; iter != this->children.end(); iter++)
		(*iter)->visit(tables, ins, el, type);//常量定义器，继承属性
}

/*
	常量定义器，获得一个标识符，及其字面值，并且添加进符号表
*/
void ast::constance_declarator::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol::value_type& type)
{
	std::vector<node*>::iterator iter = this->children.begin();
	std::string id;
	symbol* value;

	(*iter)->visit(tables, ins, el, id); //标识符，综合属性
	iter++;
	(*iter)->visit(tables, ins, el, value); //字面值，综合属性
	iter++;

	//常量类型不匹配
	if(type != value->get_value_type())
	{
		el.add_error(error::CONSTANCE_TYPE_MISMATCH, id + " = " + value->get_value(), this->get_line());
	}

	//重复定义
	if(!tables.add_symbol(type, id, value->get_value()))
	{
		el.add_error(error::DUPLICATED_CONSTANCE_DECLARATION, id, this->get_line());
	}
}

/*
	变量定义，获得一个类型标识符，然后遍历各个标识符，并添加进符号表
*/
void ast::variable_definition::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol::value_type type;

	(*iter)->visit(tables, ins, el, type); //类型标识符，综合属性
	iter++;
	
	for(; iter != this->children.end(); iter++)
	{
		std::string id;
		(*iter)->visit(tables, ins, el, id);//标识符，综合属性
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

	(*iter)->visit(tables, ins, el, type);//类型标识符，综合属性
	iter++;
	(*iter)->visit(tables, ins, el, id);//标识符，综合属性
	iter++;
	
	if(!tables.add_symbol(type, id, args, fun))
	{
		el.add_error(error::DUPLICATED_FUNCTION_DECLARATION, id, this->get_line());
	}
	ins.fun(fun);
	tables.new_symbol_table(fun);
	
	(*iter)->visit(tables, ins, el, args);//形参，综合属性
	iter++;
	(*iter)->visit(tables, ins, el);//函数体
	iter++;
	
	ins.end_fun(fun);
	tables.pop_symbol_table();
}

void ast::argument_declaration::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol::arguments* args)
{
	for(std::vector<node*>::iterator iter = this->children.begin(); iter != this->children.end(); iter++)
	{
		(*iter)->visit(tables, ins, el, args);//参数定义器，综合属性
	}
}

void ast::argument_declarator::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol::arguments* args)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol::value_type type;
	std::string id;

	(*iter)->visit(tables, ins, el, type);//类型标识符，综合属性
	iter++;
	(*iter)->visit(tables, ins, el, id);//标识符，综合属性
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

	(*iter)->visit(tables, ins, el, id);//标识符，综合属性
	iter++;
	
	if(!tables.add_symbol(symbol::VOID, id, args, fun))
	{
		el.add_error(error::DUPLICATED_FUNCTION_DECLARATION, id, this->get_line());
	}
	ins.fun(fun);
	tables.new_symbol_table(fun);
	
	(*iter)->visit(tables, ins, el, args);//形参，综合属性
	iter++;
	(*iter)->visit(tables, ins, el);//函数体
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
	
	(*iter)->visit(tables, ins, el);//函数体
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

	//有前置正负号
	if(!(this->children.size() % 2))
	{
		(*iter)->visit(tables, ins, el, oper);
		iter++;
	}
	(*iter)->visit(tables, ins, el, arg1); //项，综合属性
	iter++;
	if(oper.get_value() == "-")
		ins.inverse(arg1, arg1);
	result = arg1;
	while(iter != this->children.end())
	{
		(*iter)->visit(tables, ins, el, oper); //运算符，综合属性
		iter++;
		(*iter)->visit(tables, ins, el, arg2); //项，综合属性
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

	(*iter)->visit(tables, ins, el, arg1);//因子，综合属性
	iter++;
	result = arg1;
	while(iter != this->children.end())
	{
		(*iter)->visit(tables, ins, el, oper);//运算符，综合属性
		iter++;
		(*iter)->visit(tables, ins, el, arg2);//因子，综合属性
		iter++;
		ins.arithmetic_operation(arg1, oper, arg2, result);
		arg1 = result;
	}
}

void ast::factor::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& result)
{
	std::vector<node*>::iterator iter = this->children.begin();
	(*iter)->visit(tables, ins, el, result);//标识符、函数调用、表达式、字面值，综合属性
	iter++;
}

void ast::assign_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* arg1;
	symbol* arg2;

	(*iter)->visit(tables, ins, el, arg1); //标识符，综合属性
	iter++;
	(*iter)->visit(tables, ins, el, arg2); //表达式，综合属性
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
	
	(*iter)->visit(tables, ins, el, else_label); //条件，综合属性
	iter++;

	
	(*iter)->visit(tables, ins, el);	//语句 then部分
	iter++;
	
	if(iter != this->children.end())//else部分
	{
		tables.new_label(end_label);
		ins.jump(end_label);
		ins.add_label(else_label);

		(*iter)->visit(tables, ins, el);//语句
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

	(*iter)->visit(tables, ins, el, arg1); //表达式，综合属性
	iter++;
	if(iter != this->children.end())
	{
		(*iter)->visit(tables, ins, el, oper);//运算符，综合属性
		iter++;
		(*iter)->visit(tables, ins, el, arg2);//表达式，综合属性
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

	(*iter)->visit(tables, ins, el, end_label);//条件，综合属性
	iter++;

	(*iter)->visit(tables, ins, el);//语句

	ins.jump(loop_label);
	ins.add_label(end_label);
}

void ast::for_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* loop_label;
	symbol* end_label;

	(*iter)->visit(tables, ins, el); //赋值语句
	iter++;

	tables.new_label(loop_label);
	ins.add_label(loop_label);

	(*iter)->visit(tables, ins, el, end_label);//条件，综合属性
	iter++;

	node* increasement = *iter;
	iter++;
	
	(*iter)->visit(tables, ins, el);//语句
	iter++;

	increasement->visit(tables, ins, el); //增量语句

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

	(*iter)->visit(tables, ins, el, arg1);//标识符，综合属性
	iter++;

	(*iter)->visit(tables, ins, el, arg2);//标识符，综合属性
	iter++;

	(*iter)->visit(tables, ins, el, oper);//运算符，综合属性
	iter++;

	(*iter)->visit(tables, ins, el, step);//字面值，综合属性
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

	(*iter)->visit(tables, ins, el, id);//标识符
	iter++;

	//为了使形参表能知道函数原型，增加该操作
	params->push_back(id);
	(*iter)->visit(tables, ins, el, params);//实参表
	iter++;

	ins.call(id);
}

void ast::invoke_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el, symbol*& result)
{
	std::vector<node*>::iterator iter = this->children.begin();
	symbol* id;
	symbol::arguments* params = new symbol::arguments();

	(*iter)->visit(tables, ins, el, id);//标识符
	iter++;

	params->push_back(id);
	(*iter)->visit(tables, ins, el, params);//实参表
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
			(*iter)->visit(tables, ins, el, arg);//表达式，综合属性
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
			//参数数量不一致
			el.add_error(error::ARGUMENT_AMOUNT_MISMATCH, id->get_id(), this->get_line());
		}
	}
	else
	{
		//将变量作为函数调用
		el.add_error(error::USE_VARIABLE_AS_FUNCTION, id->get_id(), this->get_line());
	}
}

void ast::read_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	for(std::vector<node*>::iterator iter = this->children.begin(); iter != this->children.end(); iter++)
	{
		symbol* args;
		(*iter)->visit(tables, ins, el, args);//标识符，综合属性
		ins.read(args);
	}
}

void ast::write_statement::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	for(std::vector<node*>::iterator iter = this->children.begin(); iter != this->children.end(); iter++)
	{
		symbol* args;
		(*iter)->visit(tables, ins, el, args);//标识符、字符串，综合属性
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