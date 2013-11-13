#ifndef _COMPILER_CONSTANCE_PROPAGATION
#define _COMPILER_CONSTANCE_PROPAGATION

#include "instructions.h"

class constance_propagation
{
public:
	static void propagate(instructions&);
};

#endif