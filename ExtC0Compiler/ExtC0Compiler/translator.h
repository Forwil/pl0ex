#ifndef _COMPILER_TRANSLATOR
#define _COMPILER_TRANSLATOR

#include <iostream>
#include <string>
#include "instructions.h"
#include "quaternion.h"
#include "runtime_manager.h"
#include "object_code.h"
#include "data_stream.h"
#include "global_symbol_table.h"
#include "live_variable.h"

class translator
{
public:
	translator(instructions*, global_symbol_table*, data_stream*);
	void translate(std::ostream&);
private:
	instructions* ins;
	global_symbol_table* table;
	data_stream* ds;
	runtime_manager ra;
	object_code obj_code;

	void header(std::ostream&);
	void const_pool(std::ostream&);
	void body(std::ostream&);

	void translate(function_block*);
	void translate(function_block*, quaternion*);

	void translate_assign(function_block*, quaternion*);
	void translate_inv(function_block*, quaternion*);
	void translate_add(function_block*, quaternion*);
	void translate_sub(function_block*, quaternion*);
	void translate_mul(function_block*, quaternion*);
	void translate_div(function_block*, quaternion*);
	void translate_finv(function_block*, quaternion*);
	void translate_fadd(function_block*, quaternion*);
	void translate_fsub(function_block*, quaternion*);
	void translate_fmul(function_block*, quaternion*);
	void translate_fdiv(function_block*, quaternion*);
	void translate_ftoi(function_block*, quaternion*);
	void translate_itof(function_block*, quaternion*);
	void translate_jgt(function_block*, quaternion*);
	void translate_jge(function_block*, quaternion*);
	void translate_jlt(function_block*, quaternion*);
	void translate_jle(function_block*, quaternion*);
	void translate_jeq(function_block*, quaternion*);
	void translate_jne(function_block*, quaternion*);
	void translate_fjgt(function_block*, quaternion*);
	void translate_fjge(function_block*, quaternion*);
	void translate_fjlt(function_block*, quaternion*);
	void translate_fjle(function_block*, quaternion*);
	void translate_fjeq(function_block*, quaternion*);
	void translate_fjne(function_block*, quaternion*);
	void translate_jmp(function_block*, quaternion*);
	void translate_fun(function_block*, quaternion*);
	void translate_call(function_block*, quaternion*);
	void translate_push(function_block*, quaternion*);
	void translate_param(function_block*, quaternion*);
	void translate_ret(function_block*, quaternion*);
	void translate_endf(function_block*, quaternion*);
	void translate_label(function_block*, quaternion*);
	void translate_read(function_block*, quaternion*);
	void translate_write(function_block*, quaternion*);
	void translate_new_line(function_block*, quaternion*);

	void translate_arithmetic(function_block*, quaternion*, asm_ins);
	void translate_branch(function_block*, quaternion*, asm_ins);
	void translate_farithmetic(function_block*, quaternion*, asm_ins);
	void translate_fbranch(function_block*, quaternion*);
	
	void release_local_register(unsigned int);
	void release_local_register(reg_name, unsigned int);
	void occupy_local_register(symbol*, unsigned int, bool);
	void occupy_local_register(symbol*, reg_name, unsigned int, bool);
	void write_reg_back(symbol*, reg_name, unsigned int, bool, unsigned int);
	bool is_alive(symbol*, unsigned int, bool, unsigned int);
};
#endif