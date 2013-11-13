#ifndef _COMPILER_ERROR_LIST
#define _COMPILER_ERROR_LIST

#include "error.h"
#include <vector>

class error_list
{
public:
	void add_error(error::error_number, std::string, token::line_number);
	void add_warning(error::error_number, std::string, token::line_number);
	bool no_error() const;
	friend void operator<<(std::ostream&, const error_list&);
protected:
	std::vector<error*> errors;
	std::vector<error*> warnings;
};

#endif