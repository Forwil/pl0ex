#include "parser.h"
#include "ast.h"

namespace 
{
#define _IS_FUNCTION(name, value) bool is_##name(token current)\
{\
	return current.get_type() == token::value;\
}

	_IS_FUNCTION(left_parenthese, LEFT_PARENTHESE)
	_IS_FUNCTION(right_parenthese, RIGHT_PARENTHESE)
	_IS_FUNCTION(left_bracket, LEFT_BRACKET)
	_IS_FUNCTION(right_bracket, RIGHT_BRACKET)
	_IS_FUNCTION(semicolon, SEMICOLON)
	_IS_FUNCTION(comma, COMMA)
	_IS_FUNCTION(add_operator, ADD_OPERATOR)
	_IS_FUNCTION(multiply_operator, MULTIPLY_OPERATOR)
	_IS_FUNCTION(relation_operator, RELATION_OPERATOR)
	_IS_FUNCTION(assign_operator, ASSIGN_OPERATOR)
	_IS_FUNCTION(_void, VOID)
	_IS_FUNCTION(_main, MAIN)
	_IS_FUNCTION(_int, INT)
	_IS_FUNCTION(_float, FLOAT)
	_IS_FUNCTION(_char, CHAR)
	_IS_FUNCTION(_const, CONST)
	_IS_FUNCTION(_for, FOR)
	_IS_FUNCTION(_while, WHILE)
	_IS_FUNCTION(_if, IF)
	_IS_FUNCTION(_else, ELSE)
	_IS_FUNCTION(_return, RETURN)
	_IS_FUNCTION(_scanf, SCANF)
	_IS_FUNCTION(_printf, PRINTF)
	_IS_FUNCTION(unsigned_integer, UNSIGNED_INTEGER_CONST)
	_IS_FUNCTION(unsigned_real, UNSIGNED_REAL_CONST)
	_IS_FUNCTION(signed_integer, SIGNED_INTEGER_CONST)
	_IS_FUNCTION(signed_real, SIGNED_REAL_CONST)
	_IS_FUNCTION(char_const, CHAR_CONST)
	_IS_FUNCTION(string_const, STRING_CONST)
	_IS_FUNCTION(identifier, IDENTIFIER)

	bool is_step(token& current)
	{
		return current.get_type() == token::UNSIGNED_INTEGER_CONST;
	}

	bool is_literal(token& current)
	{
		return is_unsigned_integer(current) || is_unsigned_real(current) || is_signed_integer(current) || is_signed_real(current) || is_char_const(current);
	}

	bool is_type_identifier(token& current)
	{
		return is__int(current)|| is__float(current) || is__char(current);
	}

	bool is_variable_declaration(token& current, tokenizer& t)
	{
		return  is_type_identifier(current) && (is_comma(t.look_ahead(2)) || is_semicolon(t.look_ahead(2)));
	}

	bool is_function_or_procedure_declaration(token& current, tokenizer& t)
	{
		return is__int(current) || is__float(current) || is__char(current) || (is__void(current) && !is__main(t.look_ahead(1)));
	}

	bool is_main_function(token& current, tokenizer& t)
	{
		return is__void(current) && is__main(t.look_ahead(1));
	}

	bool is_loop_statement(token& current, tokenizer& t)
	{
		return is__for(current) || is__while(current);
	}

	bool is_expression(token& current, tokenizer& t)
	{
		return is_add_operator(current) || is_identifier(current) || is_literal(current) || is_left_parenthese(current);
	}

	bool is_statement(token& current, tokenizer& t)
	{
		return is__if(current) || is_loop_statement(current, t) || is__return(current) || is_identifier(current) || is__scanf(current) || is__printf(current) || is_left_bracket(current) || is_semicolon(current);
	}
#undef _IS_FUNCTION
};

#define _PROCEDURE_HEAD(name) void parser::name(node* parent)\
{\

#define _CHILD_INSERTION(name) node* child = new ast::name(current.get_line());\
	parent->add(child);

#define _LEAF_CHILD_INSERTION(name) node* child = new ast::name(current);\
	parent->add(child);

#define _LEAF_NODE_FUNCTOIN_BODY(name) 	if(is_##name(current))\
	{\
		current = t.next_token();\
	}\
	else\
	{\
		error(#name " expected.");\
	}\
}

/*
	非叶普通节点
*/
#define _NODE_PROCEDURE(name) _PROCEDURE_HEAD(name)\
	_CHILD_INSERTION(name)

/*
	肯定只包含一个孩子节点的节点
*/
#define _UNARY_PROCEDURE(name) _PROCEDURE_HEAD(name)


#define _LEAF_NODE_HEAD(name) _PROCEDURE_HEAD(name)\
	_LEAF_CHILD_INSERTION(name)

/*
	有用叶节点
*/
#define _LEAF_NODE_PROCEDURE(name)	_LEAF_NODE_HEAD(name)\
	_LEAF_NODE_FUNCTOIN_BODY(name)

/*
	无用叶节点
*/
#define _USELESS_LEAF_NODE_PROCEDURE(name) _PROCEDURE_HEAD(name)\
	_LEAF_NODE_FUNCTOIN_BODY(name)

parser::parser(tokenizer _t, error_list& _el):t(_t), el(_el)
{
	ast = NULL;
}

node* parser::parse()
{
	if(ast == NULL)
	{
		current = t.next_token();
		program();
	}
	return ast;
}

void parser::program()
{
	node* parent = new ast::program(current.get_line());
	if(is__const(current))
	{
		constance_declaration(parent);
	}
	if(is_variable_declaration(current, t))
	{
		variable_declaration(parent);
	}
	while(is_function_or_procedure_declaration(current, t))
	{
		if(is__void(current))
		{
			procedure_definition(parent);
		}
		else
		{
			function_definition(parent);
		}
	}
	if(is_main_function(current, t))
	{
		main_function(parent);
	}
	else
	{
		error("main function expected.");
	}
	this->ast = parent;
}

_NODE_PROCEDURE(constance_declaration)
	_const(child);
	do
	{
		constance_definition(child);
		semicolon(child);
	}while(is_assign_operator(t.look_ahead(2)));
}

_NODE_PROCEDURE(constance_definition)
	type_identifier(child);
	while(1)
	{
		constance_declarator(child);
		if(is_comma(current))
			comma(child);
		else
			break;
	}
}

_NODE_PROCEDURE(constance_declarator)
	identifier(child);
	assign_operator(child);
	literals(child);
}

_UNARY_PROCEDURE(literals)
	if(is_char_const(current))
	{
		char_const(parent);
	}
	else if(is_unsigned_integer(current) || is_add_operator(current) && is_unsigned_integer(t.look_ahead(1)))
	{
		integer(parent);
	}
	else if(is_unsigned_real(current) || is_add_operator(current) && is_unsigned_real(t.look_ahead(1)))
	{
		real(parent);
	}
	else 
	{
		error("constance expected.");
	}
}

_NODE_PROCEDURE(variable_declaration)
	do
	{
		variable_definition(child);
		semicolon(child);
	}while(is_variable_declaration(current, t));
}

_NODE_PROCEDURE(variable_definition)
	type_identifier(child);
	while(1)
	{
		identifier(child);
		if(is_comma(current))
			comma(child);
		else
			break;
	}
}

_UNARY_PROCEDURE(type_identifier)
	if(is__int(current))
	{
		_int(parent);
	}
	else if(is__float(current))
	{
		_float(parent);
	}
	else if(is__char(current))
	{
		_char(parent);
	}
	else
	{
		error("type identifier expected.");
	}
}

_NODE_PROCEDURE(function_definition)
	type_identifier(child);
	identifier(child);
	argument_declaration(child);
	function_body(child);
}

_NODE_PROCEDURE(argument_declaration)
	left_parenthese(child);
	argument_list(child);
	right_parenthese(child);
}

_UNARY_PROCEDURE(argument_list)
	if(is_type_identifier(current))
	{
		while(1){
			argument_declarator(parent);
			if(is_comma(current))
				comma(parent);
			else
				break;
		}
	}
}

_NODE_PROCEDURE(argument_declarator)
	type_identifier(child);
	identifier(child);
}

_NODE_PROCEDURE(function_body)
	left_bracket(child);
	compond_statement(child);
	right_bracket(child);
}

_NODE_PROCEDURE(compond_statement)
	if(is__const(current))
	{
		constance_declaration(child);
	}
	if(is_type_identifier(current))
	{
		variable_declaration(child);
	}
	statements(child);
}

_NODE_PROCEDURE(procedure_definition)
	_void(child);
	identifier(child);
	argument_declaration(child);
	procedure_body(child);
}

_NODE_PROCEDURE(procedure_body)
	left_bracket(child);
	compond_statement(child);
	right_bracket(child);
}

_NODE_PROCEDURE(main_function)
	_void(child);
	_main(child);
	left_parenthese(child);
	right_parenthese(child);
	function_body(child);
}

_NODE_PROCEDURE(expression)
	if(is_add_operator(current))
	{
		add_operator(child);
	}
	term(child);
	while(is_add_operator(current))
	{
		add_operator(child);
		term(child);
	}
}

_NODE_PROCEDURE(term)
	factor(child);
	while(is_multiply_operator(current))
	{
		multiply_operator(child);
		factor(child);
	}
}

_NODE_PROCEDURE(factor)
	if(is_identifier(current))
	{
		if(is_left_parenthese(t.look_ahead(1)))
		{
			invoke_statement(child);
		}
		else 
		{
			identifier(child);
		}
	}
	else if(is_left_parenthese(current))
	{
		left_parenthese(child);
		expression(child);
		right_parenthese(child);
	}
	else if(is_char_const(current))
	{
		char_const(child);
	}
	else if(is_unsigned_integer(current) || is_add_operator(current) && is_unsigned_integer(t.look_ahead(1)))
	{
		integer(child);
	}
	else if(is_unsigned_real(current) || is_add_operator(current) && is_unsigned_real(t.look_ahead(1)))
	{
		real(child);
	}
	else 
	{
		error();
	}
}

_NODE_PROCEDURE(statement)
	if(is__if(current))
	{
		condition_statement(child);
	}
	else if(is_loop_statement(current, t))
	{
		loop_statement(child);
	}
	else if(is__return(current))
	{
		return_statement(child);
		semicolon(child);
	}
	else if(is_identifier(current))
	{
		if(is_assign_operator(t.look_ahead(1)))
		{
			assign_statement(child);
			semicolon(child);
		}
		else
		{
			invoke_statement(child);
			semicolon(child);
		}
	}
	else if(is__scanf(current))
	{
		read_statement(child);
		semicolon(child);
	}
	else if(is__printf(current))
	{
		write_statement(child);
		semicolon(child);
	}
	else if(is_left_bracket(current))
	{
		block(child);
	}
	else if(is_semicolon(current))
	{
		semicolon(child);
	}
	else
	{
		error("statement expected.");
	}
}

_NODE_PROCEDURE(block)
	left_bracket(child);
	statements(child);
	right_bracket(child);
}

_NODE_PROCEDURE(assign_statement)
	identifier(child);
	assign_operator(child);
	expression(child);
}

_NODE_PROCEDURE(condition_statement)
	_if(child);
	left_parenthese(child);
	condition(child);
	right_parenthese(child);
	statement(child);
	if(is__else(current))
	{
		_else(child);
		statement(child);
	}
}

_NODE_PROCEDURE(condition)
	expression(child);
	if(is_relation_operator(current))
	{
		relation_operator(child);
		expression(child);
	}
}

_UNARY_PROCEDURE(loop_statement)
	if(is__while(current))
	{
		while_statement(parent);
	}
	else if(is__for(current))
	{
		for_statement(parent);
	}
	else 
	{
		error("for or while expected.");
	}
}

_NODE_PROCEDURE(while_statement)
	_while(child);
	left_parenthese(child);
	condition(child);
	right_parenthese(child);
	statement(child);
}

_NODE_PROCEDURE(for_statement)
	_for(child);
	left_parenthese(child);
	assign_statement(child);
	semicolon(child);
	condition(child);
	semicolon(child);
	increase_statement(child);
	right_parenthese(child);
	statement(child);
}

_NODE_PROCEDURE(increase_statement)
	identifier(child);
	assign_operator(child);
	identifier(child);
	add_operator(child);
	step(child);
}

_NODE_PROCEDURE(invoke_statement)
	identifier(child);
	left_parenthese(child);
	parameter_list(child);
	right_parenthese(child);
}

_NODE_PROCEDURE(parameter_list)
	if(is_expression(current, t))
	{
		while(1)
		{
			expression(child);
			if(is_comma(current))
				comma(child);
			else
				break;
		}
	}
}

_UNARY_PROCEDURE(statements)
	do
	{
		statement(parent);
	}while(is_statement(current, t));
}

_NODE_PROCEDURE(read_statement)
	_scanf(child);
	left_parenthese(child);
	while(1)
	{
		identifier(child);
		if(is_comma(current))
			comma(child);
		else
			break;
	}
	right_parenthese(child);
}

_NODE_PROCEDURE(write_statement)
	_printf(child);
	left_parenthese(child);
	if(is_string_const(current))
	{
		string_const(child);
		if(is_comma(current))
			comma(child);
	}
	if(is_expression(current, t))
	{
		expression(child);
	}
	right_parenthese(child);
}

_NODE_PROCEDURE(return_statement)
	_return(child);
	if(is_left_parenthese(current))
	{
		left_parenthese(child);
		expression(child);
		right_parenthese(child);
	}
}


/*
	词法成分
*/
_USELESS_LEAF_NODE_PROCEDURE(left_parenthese)
_USELESS_LEAF_NODE_PROCEDURE(right_parenthese)
_USELESS_LEAF_NODE_PROCEDURE(left_bracket)
_USELESS_LEAF_NODE_PROCEDURE(right_bracket)
_USELESS_LEAF_NODE_PROCEDURE(semicolon)
_USELESS_LEAF_NODE_PROCEDURE(comma)
_LEAF_NODE_PROCEDURE(add_operator)
_LEAF_NODE_PROCEDURE(multiply_operator)
_LEAF_NODE_PROCEDURE(relation_operator)
_USELESS_LEAF_NODE_PROCEDURE(assign_operator)
_USELESS_LEAF_NODE_PROCEDURE(_void)
_USELESS_LEAF_NODE_PROCEDURE(_main)
_LEAF_NODE_PROCEDURE(_int)
_LEAF_NODE_PROCEDURE(_float)
_LEAF_NODE_PROCEDURE(_char)
_USELESS_LEAF_NODE_PROCEDURE(_const)
_USELESS_LEAF_NODE_PROCEDURE(_for)
_USELESS_LEAF_NODE_PROCEDURE(_while)
_USELESS_LEAF_NODE_PROCEDURE(_if)
_USELESS_LEAF_NODE_PROCEDURE(_else)
_USELESS_LEAF_NODE_PROCEDURE(_return)
_USELESS_LEAF_NODE_PROCEDURE(_scanf)
_USELESS_LEAF_NODE_PROCEDURE(_printf)

_LEAF_NODE_HEAD(integer)
	std::string value;
	if(is_add_operator(current))
	{
		value += current.get_value();
		current = t.next_token();
	}
	value += current.get_value();
	current = t.next_token();
	token _token(value, token::SIGNED_INTEGER_CONST, current.get_line());
	((leaf_node*)child)->set_token(_token);
}

_LEAF_NODE_HEAD(real)	
	std::string value;
	if(is_add_operator(current))
	{
		value += current.get_value();
		current = t.next_token();
	}
	value += current.get_value();
	current = t.next_token();
	token _token(value, token::SIGNED_REAL_CONST, current.get_line());
	((leaf_node*)child)->set_token(_token);
}

_LEAF_NODE_PROCEDURE(step)
_LEAF_NODE_PROCEDURE(char_const)
_LEAF_NODE_PROCEDURE(string_const)
_LEAF_NODE_PROCEDURE(identifier)

void parser::error()
{
	std::cout<<"error: line:"<<current.get_line()<<std::endl;
}

void parser::error(std::string msg)
{
	std::cout<<"error: line:"<<current.get_line()<<":"<<msg<<std::endl;
}