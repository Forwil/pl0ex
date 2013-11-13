#ifndef _COMPILER_PARSER
#define _COMPILER_PARSER
#include <iostream>
#include "node.h"
#include "tokenizer.h"
#include "error_list.h"

/*
	�﷨�������������Ǵ����б�ʹʷ�������
	������﷨���ĸ��ڵ�	
	ʹ�õݹ��½��ӳ��򷨹����﷨����������﷨����
	�ڳ����﷨����ʱ����ݵ�ǰ������״̬���Խ��лָ������������¼�������б���
	
	����parse����������﷨���ĸ��ڵ�
*/
class parser
{
public:
	parser(tokenizer _t, error_list& _el);

	node* parse();
private:
	error_list& el;

	tokenizer t;//�ʷ�������
	token current; //��ǰtoken

	node* ast;

	/*
		���µݹ���̹���Ϊ����
		1.��ͨ���̣����ܰ���������ӽڵ�Ľڵ�
		2.Unary���̣�ȷ��ֻ����һ�����ӵĽڵ㣬�����Ľڵ�������﷨����ʡ�ԣ����˸�������Ľڵ�֮��
		3.Useless���̣����õ�Ҷ�ڵ㣬��Щ�ɷ�ֻ���﷨����ʱ���﷨�ɷ֡��ָ������ã����﷨����û���ô�
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