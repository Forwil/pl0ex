#ifndef _COMPILER_NODE
#define _COMPILER_NODE

#include "token.h"
#include "symbol.h"
#include "instructions.h"
#include "global_symbol_table.h"
#include "error_list.h"
#include <vector>
#include <iostream>

class constance_propagation;

/*
	语法树节点的基类
	定义了对语法树进行属性翻译时需要用到的visit的6种原型
	visit方法产生四元式，其中可能产生语义错误，并记录到错误列表中
*/
class node
{
public:
	/*
		语法树的类型，其中有很多是无用的
	*/
	const static enum ast_tree_type
	{
		PROGRAM,
		CONSTANCE_DECLARATION,
		CONSTANCE_DEFINITION,
		CONSTANCE_DECLARATOR,
		LITERALS,
		VARIABLE_DECLARATION,
		VARIABLE_DEFINITION,
		TYPE_IDENTIFIER,
		FUNCTION_DEFINITION,
		ARGUMENT_DECLARATION,
		ARGUMENT_LIST,
		ARGUMENT_DECLARATOR,
		FUNCTION_BODY,
		COMPOND_STATEMENT,
		PROCEDURE_DEFINITION,
		PROCEDURE_BODY,
		MAIN_FUNCTION,
		EXPRESSION,
		TERM,
		FACTOR,
		STATEMENT,
		BLOCK,
		ASSIGN_STATEMENT,
		CONDITION_STATEMENT,
		CONDITION,
		LOOP_STATEMENT,
		WHILE_STATEMENT,
		FOR_STATEMENT,
		INCREASE_STATEMENT,
		INVOKE_STATEMENT,
		PARAMETER_LIST,
		STATEMENTS,
		READ_STATEMENT,
		WRITE_STATEMENT,
		RETURN_STATEMENT,
		LEFT_PARENTHESE,
		RIGHT_PARENTHESE,
		LEFT_BRACKET,
		RIGHT_BRACKET,
		SEMICOLON,
		COMMA,
		ADD_OPERATOR,
		MULTIPLY_OPERATOR,
		RELATION_OPERATOR,
		ASSIGN_OPERATOR,
		_VOID,
		_MAIN,
		_CHAR,
		_INT,
		_FLOAT,
		_CONST,
		_FOR,
		_WHILE,
		_IF,
		_ELSE,
		_RETURN,
		_SCANF,
		_PRINTF,
		CHAR_CONST,
		INTEGER,
		REAL,
		STRING_CONST,
		STEP,
		IDENTIFIER
	};

	/*
		向当前节点添加孩子节点
	*/
	void add(node* child);

	/*
		program
		constance_declaration
		constance_definition
		variable_declaration
		variable_definition
		function_definition
		function_body
		compond_statement
		procedure_definition
		procedure_body
		main_function
		statement
		block
		assign_statement
		condition_statement
		while_statement
		for_statement
		increase_statement
		invoke_statement
		read_statement
		write_statement
		return_statement
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&);
	/*
		constance_declarator
		type_node(_int, _float, _char)
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, symbol::value_type&);

	/*
		argument_declaration
		argument_declarator
		parameter_list
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, symbol::arguments*);

	/*
		identifier_node(identifier)
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, std::string&);
	
	/*
		condition
		expression
		term
		factor
		invoke_statement
		identifier_node
		literal_node(char_const, integer, float, string_const, step)
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
	
	/*
		operator_node(add_operator, multiply_operator, relation_operator)
	*/
	virtual void visit(global_symbol_table&, instructions&, error_list&, token& oper);
	
	/*
		获取当前语法成分的起始行号
	*/
	token::line_number get_line() const;

	friend void operator<<(std::ostream&, const node&);
protected:
	node(){}
	ast_tree_type tree_type;
	std::vector<node*> children;
	token::line_number line;

	virtual void to_string(std::ostream&, int) const;
};

#endif