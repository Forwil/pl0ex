#ifndef _COMPILER_ERROR
#define _COMPILER_ERROR

#include "token.h"
class error
{
public:
	const static enum error_number
	{
		//语法错误
		//语义错误
		DUPLICATED_CONSTANCE_DECLARATION, //
		DUPLICATED_VARIABLE_DECLARATION, //
		DUPLICATED_FUNCTION_DECLARATION,//
		DUPLICATED_ARGUMENT_DECLARATION,//
		UNDEFINED_IDENTIFIER,//
		ZERO_STEP,
		CONSTANCE_TYPE_MISMATCH,
		ARGUMENT_AMOUNT_MISMATCH,
		INVALID_LEFT_VALUE,
		//在表达式中调用方法
		NO_RETURN_VALUE,
		USE_VARIABLE_AS_FUNCTION,
		//RET相关，未实现
		NEED_RET_VALUE,
		CAN_NOT_RET_VALUE,
		//WARNING
		ARGUMENT_TYPE_MISMATCH,
		ASSIGN_TYPE_MISMATCH,
		RETURN_TYPE_MISMATCH,
	};
	error(error_number, std::string, token::line_number);
	bool operator<(const error&);
	friend void operator<<(std::ostream&, const error&);
protected:
	error_number error_num;
	std::string msg;
	token::line_number line;
};

#endif