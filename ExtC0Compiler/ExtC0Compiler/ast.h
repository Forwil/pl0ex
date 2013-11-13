#ifndef _COMPILER_AST
#define _COMPILER_AST

#include "token.h"
#include "node.h"
#include "leaf_node.h"
#include "literal_node.h"
#include "type_node.h"
#include "identifier_node.h"
#include "operator_node.h"
#include "global_symbol_table.h"
#include "instructions.h"
#include "error_list.h"

/*
	抽象语法树的所有节点，保证语法树中的每个节点都是有用节点，除了statement外，其子节点的个数都不止一个
*/

/*
	分支节点
*/
#define _NODE_CLASS(class_name, _tree_type) class class_name:public node\
{\
public:\
	class_name(token::line_number _line){line = _line; tree_type = _tree_type;}

/*
	各种类型叶节点
*/
#define _SPECIAL_LEAF_NODE_CLASS(class_name, _tree_type, super_class) class class_name:public super_class\
{\
public:\
	class_name(token _token){leaf_token = _token; line = _token.get_line(); tree_type = _tree_type;}


namespace ast
{
	_NODE_CLASS(program, PROGRAM)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(constance_declaration, CONSTANCE_DECLARATION)};
	_NODE_CLASS(constance_definition, CONSTANCE_DEFINITION)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);	
	};
	_NODE_CLASS(constance_declarator, CONSTANCE_DECLARATOR)
	public:
		void visit(global_symbol_table&, instructions&, error_list&, symbol::value_type&);	
	};
	_NODE_CLASS(variable_declaration, VARIABLE_DECLARATION)};
	_NODE_CLASS(variable_definition, VARIABLE_DEFINITION)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(function_definition, FUNCTION_DEFINITION)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(argument_declaration, ARGUMENT_DECLARATION)
	public:
		void visit(global_symbol_table&, instructions&, error_list&, symbol::arguments*);
	};
	_NODE_CLASS(argument_declarator, ARGUMENT_DECLARATOR)
	public:
		void visit(global_symbol_table&, instructions&, error_list&, symbol::arguments*);
	};
	_NODE_CLASS(function_body, FUNCTION_BODY)};
	_NODE_CLASS(compond_statement, COMPOND_STATEMENT)};
	_NODE_CLASS(procedure_definition, PROCEDURE_DEFINITION)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(procedure_body, PROCEDURE_BODY)};
	_NODE_CLASS(main_function, MAIN_FUNCTION)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(expression, EXPRESSION)
	public:
		void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
	};
	_NODE_CLASS(term, TERM)
	public:
		void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
	};
	_NODE_CLASS(factor, FACTOR)
	public:
		void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
	};
	_NODE_CLASS(statement, STATEMENT)};
	_NODE_CLASS(block, BLOCK)};
	_NODE_CLASS(assign_statement, ASSIGN_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(condition_statement, CONDITION_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(condition, CONDITION)
	public:
		void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
	};
	_NODE_CLASS(while_statement, WHILE_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(for_statement, FOR_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(increase_statement, INCREASE_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(invoke_statement, INVOKE_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
		void visit(global_symbol_table&, instructions&, error_list&, symbol*&);
	};
	_NODE_CLASS(parameter_list, PARAMETER_LIST)
	public:
		void visit(global_symbol_table&, instructions&, error_list&, symbol::arguments*);
	};
	_NODE_CLASS(read_statement, READ_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(write_statement, WRITE_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};
	_NODE_CLASS(return_statement, RETURN_STATEMENT)
	public:
		void visit(global_symbol_table&, instructions&, error_list&);
	};

	/*
		运算符
	*/
	_SPECIAL_LEAF_NODE_CLASS(add_operator, ADD_OPERATOR, operator_node)};
	_SPECIAL_LEAF_NODE_CLASS(multiply_operator, MULTIPLY_OPERATOR, operator_node)};
	_SPECIAL_LEAF_NODE_CLASS(relation_operator, RELATION_OPERATOR, operator_node)};
	
	/*
		类型标识符
	*/
	_SPECIAL_LEAF_NODE_CLASS(_char, _CHAR, type_node)};
	_SPECIAL_LEAF_NODE_CLASS(_int, _INT, type_node)};
	_SPECIAL_LEAF_NODE_CLASS(_float, _FLOAT, type_node)};

	/*
		字面值
	*/
	_SPECIAL_LEAF_NODE_CLASS(char_const, CHAR_CONST, literal_node)};
	_SPECIAL_LEAF_NODE_CLASS(integer, INTEGER, literal_node)};
	_SPECIAL_LEAF_NODE_CLASS(real, REAL, literal_node)};
	_SPECIAL_LEAF_NODE_CLASS(string_const, STRING_CONST, literal_node)};
	_SPECIAL_LEAF_NODE_CLASS(step, STEP, literal_node)};

	//标识符
	_SPECIAL_LEAF_NODE_CLASS(identifier, IDENTIFIER, identifier_node)};
};

#endif