/*
 * =====================================================================================
 *
 *       Filename:  mips.c
 *
 *    Description:  MIPS generater
 *
 *        Version:  1.0
 *        Created:  12/19/2013 11:45:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  forwil 
 *        Company:  BUAA
 *
 * =====================================================================================
 */

#include "four.h"

FILE *fout;
char *reg_name[] = 
{
"$zero",
"$at",
"$v0",
"$v1",
"$a0",
"$a1",
"$a2",
"$a3",
"$t0",
"$t1",
"$t2",
"$t3",
"$t4",
"$t5",
"$t6",
"$t7",
"$s0",
"$s1",
"$s2",
"$s3",
"$s4",
"$s5",
"$s6",
"$s7",
"$t8",
"$t9",
"$k0",
"$k1",
"$gp",
"$s8",
"$sp",
"$ra"
}

int get_reg(int ind)
{
	
}

int get_mem(int ind,int *base)
{

}

int store_mem(int reg,int ind)
{

}

void init_gen()
{
	int i;
	struct sym_table t;
	printf("\t\t.data\n");
	for (i = 0; i<four_tablep;i++)
		if(four_codes[i].type == four_write && sym_tables[four_codes[i].des].type == t_string)
		{
			t = sym_tables[four_codes[i].des];
			printf("_s%d:\t.ascii\t%s\n",t.x,t.name);		
		}
}

void gen_mips()
{
	int i;
	printf("\t\t.text\n");
	init_gen();
	for(i = 0;i< four_tablep;i++)
	switch (four_codes[i].type)
	{
		case four_add:
				break;
		case four_sub:
				break;
		case four_mul:
				break;
		case four_div:
				break;
		case four_big:
				break;
		case four_bige:
				break;
		case four_smo:
				break;
		case four_smoe:
				break;
		case four_eq:
				break;
		case four_neq:
				break;

		case four_jmp:
				break;
		case four_jz:
				break;

		case four_bec:
				break;
		case four_call:
				break;

		case four_read:
				break;
		case four_write:
				break;

		case four_push:
				break;
		case four_end:
				break;

		case four_getarr:
				break;
		case four_getadd:
				break;
		case four_label:
				break;
		case four_enter:
				break;
		default:
			error();//shabi
	}
}
