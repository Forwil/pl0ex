#include "label_manager.h"
#include <sstream>

label_manager::label_manager()
{
	counter = 0;
}

symbol* label_manager::next_label()
{
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	ss << "label" << counter++;
	return symbol::label_symbol(ss.str());
}