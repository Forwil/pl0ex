#include "error_list.h"

void error_list::add_error(error::error_number num, std::string msg, token::line_number line)
{
	error* e = new error(num, msg, line);
	errors.push_back(e);
}

void error_list::add_warning(error::error_number num, std::string msg, token::line_number line)
{
	error* e = new error(num, msg, line);
	warnings.push_back(e);
}

bool error_list::no_error()const
{
	return errors.size() == 0;
}

void operator<<(std::ostream& out, const error_list& el)
{
	for(unsigned int i = 0; i < el.errors.size(); i++)
	{
		out << *el.errors[i];
		out << std::endl;
	}
}
