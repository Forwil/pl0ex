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

#define	reg_zero	0
#define	reg_at		1
#define reg_v0		2
#define reg_v1		3
#define reg_a0		4
#define reg_a1		5
#define reg_a2		6
#define reg_a3		7
#define reg_t0		8
#define reg_t1		9
#define reg_t2		10
#define reg_t3		11
#define reg_t4		12
#define reg_t5		13
#define reg_t6		14
#define reg_t7		15
#define reg_s0		16
#define reg_s1		17
#define reg_s2		18
#define reg_s3		19
#define reg_s4		20
#define reg_s5		21
#define reg_s6		22
#define reg_s7		23
#define reg_t8		24
#define reg_t9		25
#define reg_k0		26
#define reg_k1		27
#define reg_gp		28
#define reg_sp		29
#define reg_fp		30
#define reg_ra		31

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

void rel_reg()
{
}

int get_into_reg(int ind,int nowlevel)
{
	int base,a,b;
	b = get_reg();
	if(sym_tables[ind].type == k_const)
	{
		printf("\t\t li,%s,%d\n",reg_name[b],sym_tables[ind].x);
	}
	else
	{
		a = get_mem(ind,nowlevel,&base);
		printf("\t\t lw,%s,%d(%s)\n",reg_name[b],a,reg_name[base]);
	}
	if(sym_tables[ind].type == k_point)
	{
		printf("\t\t lw,%s,0(%s)\n",reg_name[b],reg_name[b]);	
	}
	return b;	
}

int get_mem(int ind,int nowlevel,int *base)
{
	int delt_level,a,i;
	delt_level = nowlevel - sym_tables[ind].level;
	a = get_reg();
	printf("\t\t move,%s,%s\n",reg_name[a],reg_name[reg_fp]);
	if (delt_level)
	{
		for(i = 0;i< delt_level;i++)
		{
			printf("\t\t lw,%s,%d(%s)\n",reg_name[a],-4,reg_name[a]);	
		}
	}
	*base = a;
	return -(sym_tables[ind].mem + 12);	
}

void store_to_mem(int reg,int ind,int nowlevel)
{
	int a,base,b;
	a = get_mem(ind,nowlevel,&base);
	if(sym_tables[ind].kind == k_point)
	{
		b = get_reg();
		printf("\t\t lw,%s,%d(%s)\n",reg_name[b],a,reg_name[base]);
		printf("\t\t sw,%s,0(%s)\n",reg_name[reg],reg_name[b]);
	}
	else
		printf("\t\t sw,%s,%d(%s)\n",reg_name[reg],a,reg_name[base]);
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

void set_all_mem()
{
	int i,last;
	for(i = 1;i< sym_tablep;i++)
	{	
		last = sym_tables[i].last;
		if(sym_tables[last].level != sym_tables[i].level)
			sym_tables[i].mem = 0;
		else
			sym_tables[i].mem = sym_tables[last].mem;
		if(sym_tables[i].kind == k_var||
		   sym_tables[i].kind == k_func||
		   sym_tables[i].kind == k_point)
		{
			if (sym_tables[i].x == 0)
				sym_tables[i].mem += 4;
			else
				sym_tables[i].mem += 4*sym_tables[i].x;
		}
	}
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
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t add,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;
			case four_sub:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t sub,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;
			case four_mul:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t mult,%s,%s\n",reg_name[a],reg_name[b]);
					printf("\t\t mflo,%s\n",reg_name[c]);
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);	
					break;
			case four_div:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t div,%s,%s\n",reg_name[a],reg_name[b]);
					printf("\t\t mflo,%s\n",reg_name[c]);
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;
			case four_big:	
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t sgt,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;
			case four_bige:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t sge,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;
			case four_smo:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t slt,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;
			case four_smoe:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t sle,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;
			case four_eq:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t seq,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
				break;
			case four_neq:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t\t sne,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;

			case four_jmp:
					printf("\t\t b,$L%d\n",sym_tables[t.des].x);	
					break;
			case four_jz:
					a = get_into_reg(t.src1,t.level);
					printf("\t\t beqz,%s,%L%d\n",reg_name[a],sym_tables[t.des].x);
					rel_reg(a);
					break;

			case four_bec:
					if(sym_tables[t.des].x == 0)
					{
						a = get_into_reg(t.src1,t.level);
						c = get_mem(t.des,t.level,&base);
						printf("\t\t sw,%s,%d(%s)\n",reg_name[a],a,reg_name[base]);
						rel_reg(a);
					}
					else
					{
						a = get_mem(t.src1,t.level,&base);
						d = get_reg();
						b = get_into_reg(t.src2,t.level);
						c = get_reg();
						printf("\t\t addi,%s,%s,%d\n",reg_name[d],reg_name[base],a);
						printf("\t\t li,%s,%d\n",reg_name[c],4);
						printf("\t\t mult,%s,%s\n",reg_name[c],reg_name[b]);
						printf("\t\t mflo,%s\n",reg_name[c]);
						printf("\t\t add,%s,%s,%s\n",reg_name[d],reg_name[c],reg_name[d]);
						printf("\t\t lw,%s,0(%s)\n",reg_name[c],reg_name[d]);		
						rel_reg(b);
						rel_reg(c);
						rel_reg(d);
					}
					break;
			case four_call:
					break;

			case four_read:
					a = get_into_reg(t.des,t.level);
					printf("\t\t li,$v0,5\n");
					printf("\t\t syscall\n");
					printf("\t\t move %s,$v0\n",reg_name[a]);
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
					error();//shabi
		}
	}
}
