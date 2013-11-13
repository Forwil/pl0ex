#include <iostream>
#include <sstream>
#include <fstream>
#include "compilation_unit.h"
#include "bit_set.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cout << "[usage]:" << endl;
		cout << argv[0] << " filename {-dag | -dsa | -m | -g | -ml | -r}" << endl;
	}
	else
	{
		char* name = argv[1];
		ifstream fin(name);
		compilation_unit cu(fin, "result");
		int options;
		options = LOCAL_COMMON_EXPRESSION_ELLIMINATION | DATA_STREAM_ANALYSIS | OUTPUT_MIDDLE_CODE | GENERATE_CODE | MAKE | RUN;
		for(int i = 2; i < argc; i++)
		{
			if(!strcmp(argv[i], "-dag"))
				options &= (~LOCAL_COMMON_EXPRESSION_ELLIMINATION);
			else if(!strcmp(argv[i], "-dsa"))
				options &= (~DATA_STREAM_ANALYSIS);
			else if(!strcmp(argv[i], "-m"))
				options &= (~OUTPUT_MIDDLE_CODE);
			else if(!strcmp(argv[i], "-g"))
				options &= (~GENERATE_CODE);
			else if(!strcmp(argv[i], "-ml"))
				options &= (~MAKE);
			else if(!strcmp(argv[i], "-r"))
				options &= (~RUN);
		}
		cu.compile((compilation_switch)options);
	}
}
