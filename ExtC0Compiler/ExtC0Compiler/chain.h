#ifndef _COMPILER_CHAIN
#define _COMPILER_CHAIN

#include "bit_set.h"

class chain
{
	chain(unsigned int, bit_set);
private:
	unsigned int definition_point;
	bit_set use_points;
};

#endif