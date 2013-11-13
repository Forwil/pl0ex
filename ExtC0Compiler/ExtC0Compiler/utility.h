#ifndef _COMPILER_UTILITY
#define _COMPILER_UTILITY

#include "symbol.h"
#include <string>
#include <sstream>
namespace utility
{
	std::string to_string(char value);
	std::string to_string(int value);
	std::string to_string(float value);
	std::string to_string(std::string value);
	std::string to_value_string(symbol*);

	bool is_global(symbol*);
	bool is_literal(symbol*);
	bool is_const(symbol*);
	bool is_label(symbol*);
	bool is_function(symbol*);
	bool is_variable(symbol*);
	bool is_argument(symbol*);
	bool is_temp(symbol*);
	bool is_char(symbol*);
	bool is_int(symbol*);
	bool is_float(symbol*);
	bool is_string(symbol*);

	namespace tokenizer
	{
		bool is_letter(char ch);
		bool is_non_zero(char ch);
		bool is_digit(char ch);
		bool is_white_space(char ch);
		bool is_add_operator(char ch);
		bool is_multiply_operator(char ch);
		bool is_char(char ch);
		bool is_separator(char ch);
	}
}

#endif