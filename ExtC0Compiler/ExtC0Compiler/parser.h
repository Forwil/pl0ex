#ifndef _COMPILER_PARSER
#define _COMPILER_PARSER
#include <iostream>
#include "node.h"
#include "tokenizer.h"
#include "error_list.h"

/*
	语法分析器，输入是错误列表和词法分析器
	输出是语法树的根节点	
	使用递归下降子程序法构造语法树，会出现语法错误
	在出现语法错误时会根据当前所处的状态尝试进行恢复，并将错误记录到错误列表中
	
	调用parse方法来获得语法树的根节点
*/
class parser
{
public:
	parser(tokenizer _t, error_list& _el);

	node* parse();
private:
	error_list& el;

	tokenizer t;//词法分析器
	token current; //当前token

	node* ast;

	/*
		以下递归过程共分为三种
		1.普通过程：可能包含多个孩子节点的节点
		2.Unary过程：确定只包含一个孩子的节点，这样的节点可以在语法树中省略，除了个别特殊的节点之外
		3.Useless过程：无用的叶节点，这些成分只在语法分析时起到语法成分、分隔的作用，在语法树中没有用处
	*/
	void program();
	void constance_declaration(node*);	
	void constance_definition(node*);
	void constance_declarator(node*);
	void literals(node*);				//unary
	void variable_declaration(node*);
	void variable_definition(node*);
	void type_identifier(node*);		//unary
	void function_definition(node*);
	void argument_declaration(node*);
	void argument_list(node*);			//unary
	void argument_declarator(node*);
	void function_body(node*);
	void compond_statement(node*);
	void procedure_definition(node*);
	void procedure_body(node*);
	void main_function(node*);
	void expression(node*);
	void term(node*);
	void factor(node*);
	void statement(node*);
	void block(node*);
	void assign_statement(node*);
	void condition_statement(node*);
	void condition(node*);
	void loop_statement(node*);		//unary
	void while_statement(node*);
	void for_statement(node*);
	void increase_statement(node*);
	void invoke_statement(node*);
	void parameter_list(node*);
	void statements(node*);			//unary
	void read_statement(node*);
	void write_statement(node*);
	void return_statement(node*);

	void left_parenthese(node*);	//useless
	void right_parenthese(node*);	//useless
	void left_bracket(node*);		//useless
	void right_bracket(node*);		//useless
	void semicolon(node*);			//useless
	void comma(node*);				//useless

	void add_operator(node*);
	void multiply_operator(node*);
	void relation_operator(node*);
	void assign_operator(node*);	//useless

	void _void(node*);				//useless
	void _main(node*);				//useless
	void _int(node*);
	void _float(node*);
	void _char(node*);
	void _const(node*);				//useless
	void _for(node*);				//useless
	void _while(node*);				//useless
	void _if(node*);				//useless
	void _else(node*);				//useless
	void _return(node*);			//useless
	void _scanf(node*);				//useless
	void _printf(node*);			//useless

	void integer(node*);
	void real(node*);
	void step(node*);
	void char_const(node*);
	void string_const(node*);

	void identifier(node*);

	void error();
	void error(std::string);
};
#endif