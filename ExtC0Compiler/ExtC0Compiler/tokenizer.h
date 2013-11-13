#ifndef _COMPILER_TOKENIZER
#define _COMPILER_TOKENIZER

#include "token.h"
#include <iostream>
#include <deque>

/*
	词法分析器，输入是一个输入流，无论在任何情况下，都不会发生错误
*/
class tokenizer
{
public:

	/*
		构造指定输入流的词法分析器
	*/
	tokenizer(std::istream& _input);
	/*
		获得输入流中的下一个token，该方法会优先从已经预读的队列中返回token，
		如果队列为空，则从输入流中读入下一个token，
		如果输入流已经结束，则返回NULL_TOKEN
		该方法自动跳过空白字符，并且在读入输入的过程中能够统计行号
	*/
	token next_token();
	/*
		向前看i个字符，下标从1开始
	*/
	token look_ahead(unsigned int);
private:
	const static unsigned int NUMBER_OF_RESERVE = 13;
	/*
		保留字表
	*/
	const static std::string RESERVE[NUMBER_OF_RESERVE];

	std::istream& input;
	std::deque<token> ahead_tokens;
	
	token::line_number line_num;
	
	/*
		从输入流中获得下一个token
	*/
	token _next_token();

	/*
		判断输入流中是否还包含有token，自动跳过空白字符，
		当遇到eof时返回true，遇到其他字符时返回false
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
		使用二分查找判断一个字符串是否是保留字
		如果是，则返回其对应的token_type，使用时需要强制类型转换，如果不是则返回0
	*/
	int is_reserve(std::string);
};

#endif
