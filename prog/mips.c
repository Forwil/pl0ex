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
int using[32];


int get_reg()
{
	int i;
	for(i = 0;i<32;i++)
		if (using[i] == 0)
		{
			using[i] = 1;
			return i;
		}
}

void rel_reg(int i)
{
	using[i] = 0;
}

int get_into_reg(int ind,int nowlevel)
{
	int base,a,b;
	b = get_reg();
	if(sym_tables[ind].kind == k_const)
	{
		printf("\t li,%s,%d\n",reg_name[b],sym_tables[ind].x);
	}
	else
	{
		a = get_mem(ind,nowlevel,&base);
		printf("\t lw,%s,%d(%s)\n",reg_name[b],a,reg_name[base]);
	}
	if(sym_tables[ind].kind == k_point)
	{
		printf("\t lw,%s,0(%s)\n",reg_name[b],reg_name[b]);	
	}
	return b;	
}

int get_mem(int ind,int nowlevel,int *base)
{
	int delt_level,a,i;
	delt_level = nowlevel - sym_tables[ind].level;
	a = get_reg();
	printf("\t move,%s,%s\n",reg_name[a],reg_name[reg_fp]);
	if (delt_level)
	{
		for(i = 0;i< delt_level;i++)
		{
			printf("\t lw,%s,%d(%s)\n",reg_name[a],-4,reg_name[a]);	
		}
	}
	*base = a;
	rel_reg(a);
	return -(sym_tables[ind].mem + 16);	
}

void store_to_mem(int reg,int ind,int nowlevel)
{
	int a,base,b;
	a = get_mem(ind,nowlevel,&base);
	if(sym_tables[ind].kind == k_func)
	{
		// it doesn't need store		
	}
	else if(sym_tables[ind].kind == k_point)
	{
		b = get_reg();
		printf("\t lw,%s,%d(%s)\n",reg_name[b],a,reg_name[base]);
		printf("\t sw,%s,0(%s)\n",reg_name[reg],reg_name[b]);
		rel_reg(b);
	}
	else
		printf("\t sw,%s,%d(%s)\n",reg_name[reg],a,reg_name[base]);
}

void set_string()
{
	int i;
	struct sym_table t;
	printf("\t.data\n");
	for (i = 1; i<four_tablep;i++)
		if(four_codes[i].type == four_write && sym_tables[four_codes[i].des].type == t_string)
		{
			t = sym_tables[four_codes[i].des];
			printf("_s%d:\t.ascii\t\"%s\"\n",t.x,t.name);		
		}
}

void set_all_mem()
{
	int i,last,t;
	for(i = 1;i< sym_tablep;i++)
	{	
		last = sym_tables[i].last;
		if(sym_tables[last].level != sym_tables[i].level)
			sym_tables[i].mem = 0;
		else
			sym_tables[i].mem = sym_tables[last].mem;
		if(sym_tables[i].kind == k_var||
		   sym_tables[i].kind == k_point)
		{
			if (sym_tables[i].x == 0)
				sym_tables[i].mem += 4;
			else
				sym_tables[i].mem += 4*sym_tables[i].x;
		}
		t = i;
		while(sym_tables[i].level == sym_tables[t].level)
			t = sym_tables[t].level;
		sym_tables[i].size = sym_tables[i].mem;
//		printf("%s %d %d %d\n",sym_tables[i].name,sym_tables[i].level,sym_tables[i].mem,sym_tables[i].father);
	}
}

void init_mips()
{
	using[reg_zero] = 1; 
	using[reg_at] = 1; 
	using[reg_v0] = 1; 
	using[reg_v1] = 1; 
	using[reg_a0] = 1; 
	using[reg_a1] = 1; 
	using[reg_a2] = 1; 
	using[reg_a3] = 1; 
	using[reg_gp] = 1; 
	using[reg_sp] = 1; 
	using[reg_fp] = 1; 
	using[reg_ra] = 1; 
	set_string();
	set_all_mem();
}

void gen_mips()
{
	int i,a,b,c,base,d;
	struct four_expression t;
	printf("\t.text\n");
	for(i = 1;i< four_tablep;i++)
	{
		t = four_codes[i];	
//		printf("\t\t");
//		out_one(t);
		switch (t.type)
		{
			case four_add:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t add,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
					break;
			case four_sub:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t sub,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
					break;
			case four_mul:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t mult,%s,%s\n",reg_name[a],reg_name[b]);
					printf("\t mflo,%s\n",reg_name[c]);
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);	
					store_to_mem(c,t.des,t.level);
					break;
			case four_div:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t div,%s,%s\n",reg_name[a],reg_name[b]);
					printf("\t mflo,%s\n",reg_name[c]);
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
					break;
			case four_big:	
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t sgt,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
					break;
			case four_bige:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t sge,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
					break;
			case four_smo:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t slt,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
					break;
			case four_smoe:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t sle,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
					break;
			case four_eq:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t seq,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
				break;
			case four_neq:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_into_reg(t.des,t.level);
					printf("\t sne,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					store_to_mem(c,t.des,t.level);
					break;

			case four_jmp:
					printf("\t b,$L%d\n",four_codes[t.des].des);	
					break;
			case four_jz:
					a = get_into_reg(t.src1,t.level);
					printf("\t beqz,%s,$L%d\n",reg_name[a],four_codes[t.des].des);
					rel_reg(a);
					break;

			case four_bec:
					if(sym_tables[t.des].x == 0 || sym_tables[t.des].kind == k_func || sym_tables[t.des].kind == k_point)
					{
						a = get_into_reg(t.src1,t.level);
						c = get_mem(t.des,t.level,&base);
						if(sym_tables[t.des].kind == k_point)
						{
							b = get_reg();
							printf("\t lw,%s,%d(%s)\n",reg_name[b],c,reg_name[base]);
							printf("\t sw,%s,0(%s)\n",reg_name[a],reg_name[b]);
							rel_reg(b);
						}
						else	
							printf("\t sw,%s,%d(%s)\n",reg_name[a],c,reg_name[base]);
						rel_reg(a);
					}
					else
					{
						a = get_mem(t.des,t.level,&base);
						d = get_reg();
						b = get_into_reg(t.src2,t.level);
						c = get_reg();
						printf("\t addi,%s,%s,%d\n",reg_name[d],reg_name[base],a);
						printf("\t li,%s,%d\n",reg_name[c],4);
						printf("\t mult,%s,%s\n",reg_name[c],reg_name[b]);
						printf("\t mflo,%s\n",reg_name[c]);
						printf("\t add,%s,%s,%s\n",reg_name[d],reg_name[c],reg_name[d]);
						a = get_into_reg(t.src1,t.level);
						printf("\t sw,%s,0(%s)\n",reg_name[a],reg_name[d]);		
						rel_reg(b);
						rel_reg(c);
						rel_reg(d);
					}
					break;
			case four_call:
					printf("\t addi,$sp,$sp,%d\n",4*sym_tables[t.src1].x);
					get_mem(t.src1,t.level,&base);
					a = get_reg();
					printf("\t lw,%s,-4(%s)\n",reg_name[a],reg_name[base]);
					printf("\t sw,$fp,-4($sp)\n");
					printf("\t sw,%s,-8($sp)\n",reg_name[a]);	
					printf("\t sw,%s,-12($sp)\n",reg_name[reg_ra]);
					printf("\t sw,%s,-16($sp)\n",reg_name[reg_v0]);
					printf("\t move,$fp,$sp\n");
					printf("\t addi,$sp,$sp,%d\n",-(sym_tables[t.src1].size+4));
					printf("\t jal,%s\n",sym_tables[t.src1].name);
					printf("\t addi,$sp,$sp,%d\n",(sym_tables[t.src1].size+4));
					if(t.des !=0)
					{
						b = get_mem(t.des,t.level,&base);
						printf("\t sw,$v0,%d(%s)\n",b,reg_name[base]);
					}
					printf("\t lw,$fp,-4($sp)\n");
					printf("\t lw,%s,-12($sp)\n",reg_name[reg_ra]);
					printf("\t lw,%s,-16($sp)\n",reg_name[reg_v0]);
					rel_reg(a);	
					break;

			case four_read:
					a = get_reg();
					printf("\t li,$v0,5\n");
					printf("\t syscall\n");
					printf("\t move %s,$v0\n",reg_name[a]);
					store_to_mem(a,t.des,t.level);
					rel_reg(a);
					break;
			case four_write:
					if (sym_tables[t.des].kind == k_var)
					{
						a = get_into_reg(t.des,t.level);
						printf("\t li,$v0,1\n");
						printf("\t move,$a0,%s\n",reg_name[a]);
						printf("\t syscall\n");
					}
					else
					{
						printf("\t li,$v0,4\n");
						printf("\t la,$a0,_s%d\n",sym_tables[t.des].x);	
						printf("\t syscall\n");
					}
					break;

			case four_push:
					a = get_mem(t.des,t.level,&base);
					b = get_reg();
					printf("\t lw,%s,%d(%s)\n",reg_name[b],a,reg_name[base]);
					if(sym_tables[t.des].kind == k_point)
					{
						printf("\t lw,%s,0(%s)\n",reg_name[b],reg_name[b]);					
					}
					printf("\t sw,%s,-16($sp)\n",reg_name[b]);
					printf("\t addi,$sp,$sp,-4\n");	
					rel_reg(b);
					break;
			case four_end:
					if (t.des)
					{
						a = get_into_reg(t.des,t.level);
						printf("\t move,$v0,%s\n",reg_name[a]);
					}
					printf("\t jr,$ra\n");
					break;

			case four_getarr:
					a = get_mem(t.src1,t.level,&base);
					d = get_reg();
					b = get_into_reg(t.src2,t.level);
					c = get_reg();
					printf("\t addi,%s,%s,%d\n",reg_name[d],reg_name[base],a);
					printf("\t li,%s,%d\n",reg_name[c],4);
					printf("\t mult,%s,%s\n",reg_name[c],reg_name[b]);
					printf("\t mflo,%s\n",reg_name[c]);
					printf("\t add,%s,%s,%s\n",reg_name[d],reg_name[c],reg_name[d]);
					printf("\t lw,%s,0(%s)\n",reg_name[c],reg_name[d]);		
					store_to_mem(c,t.des,t.level);
					rel_reg(b);
					rel_reg(c);
					rel_reg(d);
					break;
			case four_getadd:
					if (t.src2)
					{
						a = get_mem(t.src1,t.level,&base);
						d = get_reg();
						b = get_into_reg(t.src2,t.level);
						c = get_reg();
						printf("\t addi,%s,%s,%d\n",reg_name[d],reg_name[base],a);
						printf("\t li,%s,%d\n",reg_name[c],4);
						printf("\t mult,%s,%s\n",reg_name[c],reg_name[b]);
						printf("\t mflo,%s\n",reg_name[c]);
						printf("\t add,%s,%s,%s\n",reg_name[d],reg_name[c],reg_name[d]);
						store_to_mem(d,t.des,t.level);
						rel_reg(b);
						rel_reg(c);
						rel_reg(d);
					}
					else
					{
						a = get_mem(t.src1,t.level,&base);
						b = get_reg();
						printf("\t addi,%s,%s,%d\n",reg_name[b],reg_name[base],a);
						store_to_mem(b,t.des,t.level);
						rel_reg(b);
					}
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
