#include <stdio.h>
char *message[]=
{
"",
"missing '",
"missing \"",
"identity error type",
"missing RBP",
"error sym",
"must be ident",
"arguments error",
"redefined",
"missing COLON",
"must be const",
"missing BECOME",
"missing LBP",
"missing ARRAY",
"missing RP",
"missing SEM",
"missing LP",
"undefine ident",
"can't be const",
"can't be proc",
"must be array",
"must be proc",
"missing THEN",
"missing UNTIL",
"missing END",
"can't read",
"missing DO",
"must be func",
"empty factor",
"missing BEGIN",
"missing OF",
"missing PERIOD",
};

void my_error(int id)
{
	char c;
	
	printf("\n-----------------------------");
	printf("\n----  Error : %s ",message[id]);
	printf("\n-----------------------------");
	c = getchar();
}


