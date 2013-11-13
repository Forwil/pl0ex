#ifndef _COMPILER_TEMP_VARIABLE_MANAGER
#define _COMPILER_TEMP_VARIABLE_MANAGER
#include "symbol.h"

class temp_variable_manager
{
public:
	temp_variable_manager();
	symbol* next_temp(symbol::value_type);
private:
	unsigned int counter;
};

#endif