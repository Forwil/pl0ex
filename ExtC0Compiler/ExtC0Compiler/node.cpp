#include "node.h"
#include <cassert>

#define _INVALID_VISIT_INVOKE 0

const std::string NODE_TYPE_STRING_MAP[] = 
{
	"PROGRAM",
	"CONSTANCE_DECLARATION",
	"CONSTANCE_DEFINITION",
	"CONSTANCE_DECLARATOR",
	"LITERALS",
	"VARIABLE_DECLARATION",
	"VARIABLE_DEFINITION",
	"TYPE_IDENTIFIER",
	"FUNCTION_DEFINITION",
	"ARGUMENT_DECLARATION",
	"ARGUMENT_LIST",
	"ARGUMENT_DECLARATOR",
	"FUNCTION_BODY",
	"COMPOND_STATEMENT",
	"PROCEDURE_DEFINITION",
	"PROCEDURE_BODY",
	"MAIN_FUNCTION",
	"EXPRESSION",
	"TERM",
	"FACTOR",
	"STATEMENT",
	"BLOCK",
	"ASSIGN_STATEMENT",
	"CONDITION_STATEMENT",
	"CONDITION",
	"LOOP_STATEMENT",
	"WHILE_STATEMENT",
	"FOR_STATEMENT",
	"INCREASE_STATEMENT",
	"INVOKE_STATEMENT",
	"PARAMETER_LIST",
	"STATEMENTS",
	"READ_STATEMENT",
	"WRITE_STATEMENT",
	"RETURN_STATEMENT",
	"LEFT_PARENTHESE",
	"RIGHT_PARENTHESE",
	"LEFT_BRACKET",
	"RIGHT_BRACKET",
	"SEMICOLON",
	"COMMA",
	"ADD_OPERATOR",
	"MULTIPLY_OPERATOR",
	"RELATION_OPERATOR",
	"ASSIGN_OPERATOR",
	"_VOID",
	"_MAIN",
	"_CHAR",
	"_INT",
	"_FLOAT",
	"_CONST",
	"_FOR",
	"_WHILE",
	"_IF",
	"_ELSE",
	"_RETURN",
	"_SCANF",
	"_PRINTF",
	"CHAR_CONST",
	"INTEGER",
	"REAL",
	"STRING_CONST",
	"STEP",
	"IDENTIFIER"
};

void node::add(node* child)
{
	children.push_back(child);
}


void operator<<(std::ostream& out, const node& _node)
{
	_node.to_string(out, 0);
}

/*
	默认的遍历函数
*/
void node::visit(global_symbol_table& tables, instructions& ins, error_list& el)
{
	for(std::vector<node*>::iterator iter = children.begin(); iter != children.end(); iter++)
		(*iter)->visit(tables, ins, el);
}

void node::visit(global_symbol_table&, instructions&, error_list&, symbol::value_type&)
{
	assert(_INVALID_VISIT_INVOKE);
}

void node::visit(global_symbol_table&, instructions&, error_list&, symbol::arguments*)
{
	assert(_INVALID_VISIT_INVOKE);
}

void node::visit(global_symbol_table&, instructions&, error_list&, std::string&)
{
	assert(_INVALID_VISIT_INVOKE);
}

void node::visit(global_symbol_table&, instructions&, error_list&, symbol*&)
{
	assert(_INVALID_VISIT_INVOKE);
}

void node::visit(global_symbol_table&, instructions&, error_list&, token& oper)
{
	assert(_INVALID_VISIT_INVOKE);
}

token::line_number node::get_line() const 
{
	return line;
}

void node::to_string(std::ostream& out, int indent) const
{
	for(int i = 0; i < indent; i++) out << "  ";
	out << "[" << NODE_TYPE_STRING_MAP[tree_type] << "]" << std::endl;
	for(unsigned int i = 0; i < children.size(); i++)
		children[i]->to_string(out, indent + 1);
}
