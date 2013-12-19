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
#include "symtable.h"
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
"$sp",
"$fp",
"$ra"
};

int get_reg()
{
}

int set_into_reg(int ind)
{
	
}

int get_mem(int ind,int *base)
{

}

int store_to_mem(int reg,int ind)
{

}

void set_string()
{
	int i;
	struct sym_table t;
	printf("\t\t.data\n");
	for (i = 1; i<four_tablep;i++)
		if(four_codes[i].type == four_write && sym_tables[four_codes[i].des].type == t_string)
		{
			t = sym_tables[four_codes[i].des];
			printf("_s%d:\t.ascii\t\"%s\"\n",t.x,t.name);		
		}
}

void set_all_mem();
{
	int i;
	for(i = 1;i<
}

void init_mips()
{
	set_string();
	set_all_mem();
}

void gen_mips()
{
	int i,a,b,c,base,d;
	struct four_expression t;
	printf("\t\t.text\n");
	for(i = 1;i< four_tablep;i++)
	{
		t = four_codes[i];	
		switch (t.type)
		{
			case four_add:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t add,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					break;
			case four_sub:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t sub,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					break;
			case four_mul:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t mult,%s,%s\n",reg_name[a],reg_name[b]);
					printf("\t\t mflo,%s\n",reg_name[c]);
					break;
			case four_div:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t div,%s,%s\n",reg_name[a],reg_name[b]);
					printf("\t\t mflo,%s\n",reg_name[c]);
					break;
			case four_big:	
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t sgt,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					break;
			case four_bige:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t sge,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					break;
			case four_smo:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t slt,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					break;
			case four_smoe:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t sle,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					break;
			case four_eq:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t seq,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					break;
			case four_neq:
					a = set_into_reg(t.src1);
					b = set_into_reg(t.src2);
					c = set_into_reg(t.des);
					printf("\t\t sne,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					break;

			case four_jmp:
					printf("\t\t b,$L%d\n",sym_tables[t.des].x);	
					break;
			case four_jz:
					a = set_into_reg(t.src1);
					printf("\t\t beqz,%s,%L%d\n",reg_name[a],sym_tables[t.des].x);
					break;

			case four_bec:
					if(sym_tables[t.des].x == 0)
					{
						a = set_into_reg(t.src1);
						c = set_into_reg(t.des);
						printf("\t\t move,%s,%s\n",reg_name[c],reg_name[a]);
					}
					else
					{
						a = get_men(t.src1,&base);
						d = get_reg();
						b = set_into_reg(t.src2);
						c = get_reg();
						printf("\t\t addi,%s,%s,%d\n",d,base,a);
						printf("\t\t add,%s,%s,%s\n",d,b,d);
						printf("\t\t lw,%s,0(%s)\n",c,d);		
					}
					break;
			case four_call:
					break;

			case four_read:
					a = set_into_reg(t.des);
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
					printf("$L%d:\n",t.des);
					break;
			case four_enter:
					printf("%s:\n",sym_tables[t.des].name);
					break;
			default:
					printf("%d\n",t.type);
				e	rror();//shabi
		}
	}
}
