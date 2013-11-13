#include "temp_variable_manager.h"
#include <sstream>

temp_variable_manager::temp_variable_manager()
{
	counter = 0;
}
symbol* temp_variable_manager::next_temp(symbol::value_type st)
{
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	ss << "$temp" << counter++;
	return symbol::variable_symbol(st, ss.str());
}