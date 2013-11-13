#include "utility.h"
#include "symbol_table.h"

using namespace std;

namespace utility
{	
	string to_string(char value)
	{
		stringstream ss(stringstream::in | stringstream::out);
		ss << value;
		return ss.str();
	}

	string to_string(int value)
	{
		stringstream ss(stringstream::in | stringstream::out);
		ss << value;
		return ss.str();
	}

	string to_string(float value)
	{
		stringstream ss(stringstream::in | stringstream::out);
		int* int_ptr = (int*)&value;
		ss << "0" << hex << *int_ptr << "H";
		return ss.str();
	}

	string to_string(string value)
	{
		stringstream ss(stringstream::in | stringstream::out);
		for(unsigned int i = 0; i < value.size(); i++)
		{
			if(value[i] == '%')
				ss << "%%";
			else
				ss << value[i];
		}
		ss << ", 0";
		return ss.str();
	}

	string to_value_string(symbol* sym)
	{
		switch(sym->get_value_type())
		{
		case symbol::CHAR:
		case symbol::INT:
			return sym->get_value();
		case symbol::FLOAT:
			{
				stringstream ss(stringstream::in | stringstream::out);
				float value;
				ss << sym->get_value();
				ss >> value;
				return to_string(value);
			}
		case symbol::STRING:
			return to_string(sym->get_value());
		default:
			return "";
		}
	}

	bool is_global(symbol* sym)
	{
		return (sym->get_scope()->get_name() == "@global");
	}
	bool is_literal(symbol* sym)
	{
		return (sym->get_symbol_type() == symbol::LITERAL);
	}
	bool is_const(symbol* sym)
	{
		return (sym->get_symbol_type() == symbol::CONST);
	}
	bool is_label(symbol* sym)
	{
		return (sym->get_symbol_type() == symbol::LABEL);
	}
	bool is_function(symbol* sym)
	{
		return (sym->get_symbol_type() == symbol::FUNCTION);
	}
	bool is_variable(symbol* sym)
	{
		return (sym->get_symbol_type() == symbol::VARIABLE);
	}
	bool is_argument(symbol* sym)
	{
		return (sym->get_symbol_type() == symbol::ARGUMENT);
	}
	bool is_temp(symbol* sym)
	{
		return(sym->get_id()[0] == '$');
	}

	bool is_char(symbol* sym)
	{
		return (sym->get_value_type() == symbol::CHAR);
	}

	bool is_int(symbol* sym)
	{
		return (sym->get_value_type() == symbol::INT);
	}

	bool is_float(symbol* sym)
	{
		return (sym->get_value_type() == symbol::FLOAT);
	}

	bool is_string(symbol* sym)
	{
		return (sym->get_value_type() == symbol::STRING);
	}

	namespace tokenizer
	{
		bool is_letter(char ch)
		{
			return isalpha(ch) || ch == '_';
		}

		bool is_non_zero(char ch)
		{
			return (ch >= '1' && ch <= '9');
		}

		bool is_digit(char ch)
		{
			return is_non_zero(ch) || ch == '0';
		}

		bool is_white_space(char ch)
		{
			if(isspace(ch))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool is_add_operator(char ch)
		{
			return (ch == '+' || ch == '-');
		}

		bool is_multiply_operator(char ch)
		{
			return (ch == '*' || ch == '/');
		}

		bool is_char(char ch)
		{
			return (is_letter(ch) || is_digit(ch) || is_add_operator(ch) || is_multiply_operator(ch));
		}

		bool is_separator(char ch)
		{
			return (is_white_space(ch) ||
					is_add_operator(ch)||
					is_multiply_operator(ch)||
					ch == '(' ||
					ch == ')' ||
					ch == '{' ||
					ch == '}' ||
					ch == ',' ||
					ch == ';' ||
					ch == '>' ||
					ch == '<' ||
					ch == '!' ||
					ch == '=' ||
					ch == '"' ||
					ch == '\'');
		}
	}
}