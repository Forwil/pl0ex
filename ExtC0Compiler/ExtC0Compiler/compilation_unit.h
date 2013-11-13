#ifndef _COMPILER_COMPILATION_UNIT
#define _COMPILER_COMPILATION_UNIT

#include <string>
#include "error_list.h"
#include "parser.h"
#include "ast.h"
#include "quaternion.h"
#include "global_symbol_table.h"
#include "instructions.h"

const static enum compilation_switch
{
	LOCAL_COMMON_EXPRESSION_ELLIMINATION = 1,
	DATA_STREAM_ANALYSIS = 2,
	REGISTER_ALLOCATE = 4,
	OUTPUT_MIDDLE_CODE = 8,
	GENERATE_CODE = 16,
	MAKE = 32,
	RUN = 64,
};

class compilation_unit
{
public:
	compilation_unit(std::istream&, std::string);
	void compile(unsigned int);
private:
	std::istream& input;
	error_list* el;
	parser* par;
	node* ast;
	global_symbol_table* table;
	instructions* ins;
	std::string name;
};

#endif