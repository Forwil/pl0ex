#include "token.h"
#include <cassert>
#include <cctype>

#define _NO_LINE_INITIALIZED line > 0

namespace
{
	const std::string TOKEN_TYPE_STRING_MAP[] = 
	{
		"IDENTIFIER",
		"CHAR",
		"CONST",
		"ELSE",
		"FLOAT",
		"FOR",
		"IF",
		"INT",
		"MAIN",
		"PRINTF",
		"RETURN",
		"SCANF",
		"VOID",
		"WHILE",
		"SIGNED_INTEGER_CONST",
		"SIGNED_REAL_CONST",
		"UNSIGNED_INTEGER_CONST",
		"UNSIGNED_REAL_CONST",
		"CHAR_CONST",
		"STRING_CONST",
		"ADD_OPERATOR",
		"MULTIPLY_OPERATOR",
		"RELATION_OPERATOR",
		"ASSIGN_OPERATOR",
		"LEFT_PARENTHESE",
		"RIGHT_PARENTHESE",
		"LEFT_BRACKET",
		"RIGHT_BRACKET",
		"SEMICOLON",
		"COMMA",
		"NEW_LINE",
		"NULL_TOKEN",
		//¥ÌŒÛTOKEN
		"INVALID_CHAR",
		"INVALID_INT",
		"INVALID_REAL",
		"INVALID_STRING",
		"ZERO_PREFIX_NUMBER",
		"UNKNOWN_TOKEN",
	};
}

token::token(){}

token::token(std::string _value, token::token_type _type, token::line_number _line)
{
	value = _value;
	type = _type;
	line = _line;
	if(type != CHAR_CONST && type != STRING_CONST)
	{
		for(unsigned int i = 0; i < value.size(); i++)
			value[i] = tolower(value[i]);
	}
}

token::token(std::string _value, token::token_type _type):value(_value), type(_type)
{
	value = _value;
	type = _type;
	line = 0;
}

std::string token::get_value() const
{
	return this->value;
}

token::token_type token::get_type() const 
{
	return this->type;
}

token::line_number token::get_line() const 
{
	assert(_NO_LINE_INITIALIZED);
	return this->line;
}

void operator<<(std::ostream& out, const token& _token)
{
	out << "[" << TOKEN_TYPE_STRING_MAP[_token.type] << ":" << _token.value << "]@" << _token.line << std::endl;
}

#undef _NO_LINE_INITIALIZED
