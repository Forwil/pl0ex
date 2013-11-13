#include "error.h"

error::error(error_number num, std::string _msg, token::line_number _line)
{
	error_num = num;
	msg = _msg;
	line = _line;
}

bool error::operator<(const error& e)
{
	return line < e.line;
}

void operator<<(std::ostream& out, const error& e)
{
	out << "[line: " << e.line << ", error: " << e.error_num << "] " << e.msg;
}