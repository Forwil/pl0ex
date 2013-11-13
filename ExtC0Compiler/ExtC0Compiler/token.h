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
		//����TOKEN
		INVALID_CHAR,
		INVALID_INT,
		INVALID_REAL,
		INVALID_STRING,
		ZERO_PREFIX_NUMBER,
		UNKNOWN_TOKEN,
	};


	/*
		��leaf_node���õ���Ĭ�Ϲ��캯��
	*/
	token();
	/*
		����һ�������кŵ�token
	*/
	token(std::string, token::token_type, token::line_number);
	/*
		����һ���������кŵ�token������һЩ��ʱ��token���߿�tokenʱ�õ�
	*/
	token(std::string, token::token_type);

	/*
		���token���ַ���ֵ���ַ��������������ţ��ַ�������˫����
	*/
	std::string get_value() const;
	/*
		���token�����ͣ���ֵΪtoken_type��38��ֵ֮һ
	*/
	token_type get_type() const;
	/*
		���token�����к�
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