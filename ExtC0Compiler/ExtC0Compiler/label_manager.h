#ifndef _COMPLIER_LABEL_MANAGER
#define _COMPILER_LABEL_MANAGER
#include "symbol.h"

class label_manager
{
public:
	label_manager();
	symbol* next_label();
private:
	unsigned int counter;
};

#endif