#ifndef _COMPILER_TOKEN
#define _COMPILER_TOKEN

#include <string>

struct token
{
public:
	typedef unsigned int line_number;
	
	const static enum token_type{
		IDENTIFIER,
		CHAR,
		CONST,
		ELSE,
		FLOAT,
		FOR,
		IF,
		INT,
		MAIN,
		PRINTF,
		RETURN,
		SCANF,
		VOID,
		WHILE,
		SIGNED_INTEGER_CONST,
		SIGNED_REAL_CONST,
		UNSIGNED_INTEGER_CONST,
		UNSIGNED_REAL_CONST,
		CHAR_CONST,
		STRING_CONST,
		ADD_OPERATOR,
		MULTIPLY_OPERATOR,
		RELATION_OPERATOR,
		ASSIGN_OPERATOR,
		LEFT_PARENTHESE,
		RIGHT_PARENTHESE,
		LEFT_BRACKET,
		RIGHT_BRACKET,
		SEMICOLON,
		COMMA,
		NEW_LINE,
		NULL_TOKEN,
		//错误TOKEN
		INVALID_CHAR,
		INVALID_INT,
		INVALID_REAL,
		INVALID_STRING,
		ZERO_PREFIX_NUMBER,
		UNKNOWN_TOKEN,
	};


	/*
		在leaf_node中用到的默认构造函数
	*/
	token();
	/*
		构造一个包含行号的token
	*/
	token(std::string, token::token_type, token::line_number);
	/*
		构造一个不包含行号的token，构造一些临时的token或者空token时用到
	*/
	token(std::string, token::token_type);

	/*
		获得token的字符串值，字符变量包含单引号，字符串包含双引号
	*/
	std::string get_value() const;
	/*
		获得token的类型，其值为token_type的38个值之一
	*/
	token_type get_type() const;
	/*
		获得token所在行号
	*/
	line_number get_line() const;

	friend void operator<<(std::ostream&, const token&);
private:
	std::string value;
	token_type type;
	line_number line;
	//unsigned int column;
};

#endif