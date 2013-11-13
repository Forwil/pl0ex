#ifndef _COMPILER_TOKENIZER
#define _COMPILER_TOKENIZER

#include "token.h"
#include <iostream>
#include <deque>

/*
	�ʷ���������������һ�����������������κ�����£������ᷢ������
*/
class tokenizer
{
public:

	/*
		����ָ���������Ĵʷ�������
	*/
	tokenizer(std::istream& _input);
	/*
		����������е���һ��token���÷��������ȴ��Ѿ�Ԥ���Ķ����з���token��
		�������Ϊ�գ�����������ж�����һ��token��
		����������Ѿ��������򷵻�NULL_TOKEN
		�÷����Զ������հ��ַ��������ڶ�������Ĺ������ܹ�ͳ���к�
	*/
	token next_token();
	/*
		��ǰ��i���ַ����±��1��ʼ
	*/
	token look_ahead(unsigned int);
private:
	const static unsigned int NUMBER_OF_RESERVE = 13;
	/*
		�����ֱ�
	*/
	const static std::string RESERVE[NUMBER_OF_RESERVE];

	std::istream& input;
	std::deque<token> ahead_tokens;
	
	token::line_number line_num;
	
	/*
		���������л����һ��token
	*/
	token _next_token();

	/*
		�ж����������Ƿ񻹰�����token���Զ������հ��ַ���
		������eofʱ����true�����������ַ�ʱ����false
	*/
	bool has_more_token();
	
	token identifier(char);
	token number(char);
	token char_const(char);
	token string_const(char);
	token add_operator(char);
	token multiply_operator(char);
	token assign_or_equal_operator(char);
	token relation_operator(char);
	token left_parenthese(char);
	token right_parenthese(char);
	token left_bracket(char);
	token right_bracket(char);
	token semicolon(char);
	token comma(char);

	/*
		ʹ�ö��ֲ����ж�һ���ַ����Ƿ��Ǳ�����
		����ǣ��򷵻����Ӧ��token_type��ʹ��ʱ��Ҫǿ������ת������������򷵻�0
	*/
	int is_reserve(std::string);
};

#endif
