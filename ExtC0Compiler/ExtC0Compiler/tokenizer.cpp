#include "tokenizer.h"
#include "utility.h"
#include <cctype>

const std::string tokenizer::RESERVE[] = 
{
	"char",
	"const",
	"else",
	"float",
	"for",
	"if",
	"int",
	"main",
	"printf",
	"return",
	"scanf",
	"void",
	"while"
};

tokenizer::tokenizer(std::istream& _input):input(_input)
{
	line_num = 1;
}

bool tokenizer::has_more_token()
{
	char ptr;
	while(input.get(ptr) && utility::tokenizer::is_white_space(ptr))
		if(ptr == '\n')line_num++;
	if(input.eof())
	{
		return false;
	}
	else
	{
		input.unget();
		return true;
	}
}

token tokenizer::next_token()
{
	if(!ahead_tokens.empty())
	{
		token next = ahead_tokens.front();
		ahead_tokens.pop_front();
		return next;
	}
	else if(!has_more_token())
	{
		return token("", token::NULL_TOKEN);
	}	
	else 
	{
		return _next_token();
	}
}
token tokenizer::_next_token()
{
	char ptr;
	while(input.get(ptr) && utility::tokenizer::is_white_space(ptr))
		if(ptr == '\n')line_num++;
	if(utility::tokenizer::is_letter(ptr))
	{
		return identifier(ptr);
	}
	else if(utility::tokenizer::is_digit(ptr))
	{
		return number(ptr);
	}
	else
	{
		switch(ptr)
		{
		case '+':
		case '-':
			return add_operator(ptr);
		case '*':
		case '/':
			return multiply_operator(ptr);
		case '=':
			return assign_or_equal_operator(ptr);
		case '>':
		case '<':
		case '!':
			return relation_operator(ptr);
		case '(':
			return left_parenthese(ptr);
		case ')':
			return right_parenthese(ptr);
		case '{':
			return left_bracket(ptr);
		case '}':
			return right_bracket(ptr);
		case ';':
			return semicolon(ptr);
		case ',':
			return comma(ptr);
		case '\'':
			return char_const(ptr);
		case '"':
			return string_const(ptr);
		default:
			return token("" + ptr, token::UNKNOWN_TOKEN);
		}
	}
}

token tokenizer::identifier(char ptr)
{
	std::string value;
	do
	{
		value += ptr;
	}while(input.get(ptr) && (utility::tokenizer::is_letter(ptr) || utility::tokenizer::is_digit(ptr)));
	input.unget();
	return token(value, (token::token_type)is_reserve(value), line_num);
}

token tokenizer::number(char ptr)
{
	std::string value;
	token::token_type type = token::UNSIGNED_INTEGER_CONST;
	bool first_digit = true;
	bool is_zero = false;
	bool real_part = false;

	do
	{
		value += ptr;
		if(first_digit)
		{
			if(ptr == '0')is_zero = true;
			first_digit = false;
		}
		else 
		{
			if(is_zero)
			{
				if(ptr == '0')
					type = token::ZERO_PREFIX_NUMBER;
				is_zero = false;
			}
			switch(type)
			{
			case token::UNSIGNED_INTEGER_CONST:
				if(!utility::tokenizer::is_digit(ptr))
				if(ptr == '.')
					type = token::UNSIGNED_REAL_CONST;
				else
					type = token::INVALID_INT;
				break;
			case token::INVALID_INT:
				if(ptr == '.')
					type = token::INVALID_REAL;
				break;
			case token::UNSIGNED_REAL_CONST:
				if(utility::tokenizer::is_digit(ptr))
					real_part = true;
				else
					type = token::INVALID_REAL;
				break;
			}		
		}
	}while(input.get(ptr) && !utility::tokenizer::is_separator(ptr));
	input.unget();
	
	if(type == token::UNSIGNED_REAL_CONST && !real_part)type = token::INVALID_REAL;

	return token(value, type, line_num);
}

token tokenizer::char_const(char ptr)
{
	//TODO 转义符
	std::string value;
	value += ptr;
	input.get(ptr);
	value += ptr;
	if(!(utility::tokenizer::is_add_operator(ptr) || utility::tokenizer::is_multiply_operator(ptr) || utility::tokenizer::is_letter(ptr) || utility::tokenizer::is_digit(ptr)))
		return token(value, token::INVALID_CHAR, line_num);
	input.get(ptr);
	if(ptr != '\'')
	{
		input.unget();
		return token(value, token::INVALID_CHAR, line_num);
	}
	else value += ptr;
	return token(value, token::CHAR_CONST, line_num);
}

token tokenizer::string_const(char ptr)
{
	//TODO 转义符
	std::string value;
	value += ptr;
	while(input.get(ptr) && ptr != '"')
	{
		if(ptr == '\n') return token(value, token::INVALID_STRING, line_num);
		value += ptr;
	}
	value += ptr;
	return token(value, token::STRING_CONST, line_num);
}

token tokenizer::add_operator(char ptr)
{
	std::string value;
	value += ptr;
	return token(value, token::ADD_OPERATOR, line_num);
}

token tokenizer::multiply_operator(char ptr)
{
	std::string value;
	value += ptr;
	return token(value, token::MULTIPLY_OPERATOR, line_num);
}

token tokenizer::assign_or_equal_operator(char ptr)
{
	std::string value;
	value += ptr;
	input.get(ptr);
	if(ptr == '=')
	{
		value += ptr;
		return token(value, token::RELATION_OPERATOR, line_num);
	}
	else
	{
		input.unget();
		return token(value, token::ASSIGN_OPERATOR, line_num);
	}
}

token tokenizer::relation_operator(char ptr)
{
	std::string value;
	value += ptr;
	input.get(ptr);
	if(ptr == '=')
		value += ptr;
	else
		input.unget();
	return token(value, token::RELATION_OPERATOR, line_num);
}

token tokenizer::left_parenthese(char ptr)
{
	std::string value;
	value += ptr;
	return token(value, token::LEFT_PARENTHESE, line_num);
}

token tokenizer::right_parenthese(char ptr)
{
	std::string value;
	value += ptr;
	return token(value, token::RIGHT_PARENTHESE, line_num);
}

token tokenizer::left_bracket(char ptr)
{
	std::string value;
	value += ptr;
	return token(value, token::LEFT_BRACKET, line_num);
}

token tokenizer::right_bracket(char ptr)
{
	std::string value;
	value += ptr;
	return token(value, token::RIGHT_BRACKET, line_num);
}

token tokenizer::semicolon(char ptr)
{
	std::string value;
	value += ptr;
	return token(value, token::SEMICOLON, line_num);
}

token tokenizer::comma(char ptr)
{
	std::string value;
	value += ptr;
	return token(value, token::COMMA, line_num);
}

int tokenizer::is_reserve(std::string value)
{
	int l = 0;
	int r = tokenizer::NUMBER_OF_RESERVE - 1;
	int mid;
	while(l <= r)
	{
		mid = (l + r) / 2;
		if(tokenizer::RESERVE[mid] == value)return mid + 1;
		else if(tokenizer::RESERVE[mid] < value) l = mid + 1;
		else r = mid - 1;
	}
	return 0;
}

token tokenizer::look_ahead(unsigned int index)
{
	std::deque<token>::size_type i = ahead_tokens.size();
	while(i < index)
	{
		ahead_tokens.push_back(_next_token());
		i++;
	}
	return ahead_tokens[index - 1];
}