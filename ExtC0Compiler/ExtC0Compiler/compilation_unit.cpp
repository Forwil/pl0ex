#include <fstream>
#include <cstdlib>
#include "compilation_unit.h"
#include "constance_propagation.h"
#include "dag.h"
#include "data_stream.h"
#include "conflict_graph.h"
#include "translator.h"

compilation_unit::compilation_unit(std::istream& _input, std::string _name):input(_input)
{
	name = _name;
	table = new global_symbol_table();
	ins = new instructions(*table);
	el = new error_list();
	par = new parser(_input, *el);
}

void compilation_unit::compile(unsigned int compile_switch)
{
	ast = par->parse();
	ast->visit(*table, *ins, *el);
	if(el->no_error())
	{	
		data_stream* ds = NULL;
		if(compile_switch & LOCAL_COMMON_EXPRESSION_ELLIMINATION)
		{
			dag d(ins);
		}
		if(compile_switch & DATA_STREAM_ANALYSIS)
		{
			ds =  new data_stream(ins);
		}
		if(compile_switch & OUTPUT_MIDDLE_CODE)
		{
			std::ofstream middle((name + ".quat").c_str());
			middle << *ins;
			middle.close();
		}
		if(compile_switch & GENERATE_CODE)
		{
			std::ofstream asm_code((name + ".asm").c_str());
			translator t(ins, table, ds);
			t.translate(asm_code);
			asm_code.close();
		}
		if(compile_switch & MAKE)
		{
			system(("ml /c /coff " + name + ".asm").c_str());
			system(("link /subsystem:console " + name + ".obj").c_str());
		}
		if(compile_switch & RUN)
		{
			system(name.c_str());
		}
	}
	else
	{
		std::cout << *el;
	}
}