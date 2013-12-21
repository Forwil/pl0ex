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
#include "mips.h"
#include <stdio.h>
FILE *fout;
int using[MAXREG];
int get_reg()
{
	int i;
	for(i = 0;i<MAXREG;i++)
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
		fprintf(fout,"\t li,%s,%d\n",reg_name[b],sym_tables[ind].x);
	}
	else
	{
		a = get_mem(ind,nowlevel,&base);
		fprintf(fout,"\t lw,%s,%d(%s)\n",reg_name[b],a,reg_name[base]);
		rel_reg(base);
	}
	if(sym_tables[ind].kind == k_point)
	{
		fprintf(fout,"\t lw,%s,0(%s)\n",reg_name[b],reg_name[b]);	
	}
	return b;	
}

int get_mem(int ind,int nowlevel,int *base)
{
	int delt_level,a,i;
	delt_level = nowlevel - sym_tables[ind].level;
	a = get_reg();
	fprintf(fout,"\t move,%s,%s\n",reg_name[a],reg_name[reg_fp]);
	*base = a;
	if(sym_tables[ind].kind == k_func)
		return -OFFSET;

	for(i = 0;i< delt_level;i++)
		fprintf(fout,"\t lw,%s,-8(%s)\n",reg_name[a],reg_name[a]);	
	return -(sym_tables[ind].mem + OFFSET);	
}

void store_into_mem(int reg,int ind,int nowlevel)
{
	int a,base,b;
	a = get_mem(ind,nowlevel,&base);
	if(sym_tables[ind].kind == k_func)
	{
		fprintf(fout,"\t sw,%s,-16(%s)\n",reg_name[reg],reg_name[base]);
	}
	else if(sym_tables[ind].kind == k_point)
	{
		b = get_reg();
		fprintf(fout,"\t lw,%s,%d(%s)\n",reg_name[b],a,reg_name[base]);
		fprintf(fout,"\t sw,%s,0(%s)\n",reg_name[reg],reg_name[b]);
		rel_reg(b);
	}
	else
		fprintf(fout,"\t sw,%s,%d(%s)\n",reg_name[reg],a,reg_name[base]);
	rel_reg(base);
}

void set_string()
{
	int i;
	struct sym_table t;
	fprintf(fout,"\t.data\n");
	for (i = 1; i<four_tablep;i++)
		if(four_codes[i].type == four_write && sym_tables[four_codes[i].des].type == t_string)
		{
			t = sym_tables[four_codes[i].des];
			fprintf(fout,"_s%d:\t.ascii\t\"%s\"\n",t.x,t.name);		
		}
}

void init_all_mem()
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
				sym_tables[i].mem += 4 * 1;
			else
				sym_tables[i].mem += 4 * sym_tables[i].x;
		}
		t = i;
		while(sym_tables[i].level == sym_tables[t].level)
			t = sym_tables[t].last;
		sym_tables[t].size = sym_tables[i].mem;
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
	//fout = stdout;
	fout = fopen("result.asm","w");
	set_string();
	init_all_mem();
}

void gen_mips()
{
	int i,a,b,c,base,d;
	struct four_expression t;
	fprintf(fout,"\t.text\n");
	fprintf(fout,"main:\n");
	fprintf(fout,"\t move,$fp,$sp\n");
	fprintf(fout,"\t sw,$fp,-4($sp)\n");
	fprintf(fout,"\t sw,$fp,-8($sp)\n");
	fprintf(fout,"\t b,_main\n");
	for(i = 1;i< four_tablep;i++)
	{
		t = four_codes[i];	
		printf("\t#%d",i);
		out_one(t);
		switch (t.type)
		{
			case four_add: case four_sub: case four_mul: case four_div: 
			case four_big: case four_bige: case four_smo: case four_smoe:
			case four_eq: case four_neq:
					a = get_into_reg(t.src1,t.level);
					b = get_into_reg(t.src2,t.level);
					c = get_reg();
					if (t.type == four_add)
						fprintf(fout,"\t add,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					if (t.type == four_sub)
						fprintf(fout,"\t sub,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					if (t.type == four_mul)
					{
						fprintf(fout,"\t mult,%s,%s\n",reg_name[a],reg_name[b]);
						fprintf(fout,"\t mflo,%s\n",reg_name[c]);
					}
					if (t.type == four_div)
					{
						fprintf(fout,"\t div,%s,%s\n",reg_name[a],reg_name[b]);
						fprintf(fout,"\t mflo,%s\n",reg_name[c]);
					}
					if (t.type == four_big)
						fprintf(fout,"\t sgt,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					if (t.type == four_bige)
						fprintf(fout,"\t sge,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					if (t.type == four_smo)
						fprintf(fout,"\t slt,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					if (t.type == four_smoe)
						fprintf(fout,"\t sle,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					if (t.type == four_eq)
						fprintf(fout,"\t seq,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	
					if (t.type == four_neq)
						fprintf(fout,"\t sne,%s,%s,%s\n",reg_name[c],reg_name[a],reg_name[b]);	

					d = get_mem(t.des,t.level,&base);
					fprintf(fout,"\t sw,%s,%d(%s)\n",reg_name[c],d,reg_name[base]);
					rel_reg(base);
					rel_reg(a);
					rel_reg(b);
					rel_reg(c);
					break;

			case four_jmp:
					fprintf(fout,"\t b,$L%d\n",four_codes[t.des].des);	
					break;

			case four_jz:
					a = get_into_reg(t.src1,t.level);
					fprintf(fout,"\t beqz,%s,$L%d\n",reg_name[a],four_codes[t.des].des);
					rel_reg(a);
					break;

			case four_bec:
					if(sym_tables[t.des].x == 0
					|| sym_tables[t.des].kind == k_func 
					|| sym_tables[t.des].kind == k_point)
					{
						a = get_into_reg(t.src1,t.level);
						store_into_mem(a,t.des,t.level);
						rel_reg(a);
					}
					else
					{
						a = get_mem(t.des,t.level,&base);
						d = get_reg();
						b = get_into_reg(t.src2,t.level);
						c = get_reg();
						fprintf(fout,"\t addi,%s,%s,%d\n",reg_name[d],reg_name[base],a);
						fprintf(fout,"\t li,%s,%d\n",reg_name[c],4);
						fprintf(fout,"\t mult,%s,%s\n",reg_name[c],reg_name[b]);
						fprintf(fout,"\t mflo,%s\n",reg_name[c]);
						fprintf(fout,"\t add,%s,%s,%s\n",reg_name[d],reg_name[c],reg_name[d]);
						a = get_into_reg(t.src1,t.level);
						fprintf(fout,"\t sw,%s,0(%s)\n",reg_name[a],reg_name[d]);		
						rel_reg(a);
						rel_reg(base);
						rel_reg(b);
						rel_reg(c);
						rel_reg(d);
					}
					break;
			case four_call:
					get_mem(t.src1,t.level,&base);
					a = get_reg();
					fprintf(fout,"\t lw,%s,-8(%s)\n",reg_name[a],reg_name[base]);
					rel_reg(base);
					fprintf(fout,"\t sw,$fp,-4($sp)\n");
					fprintf(fout,"\t sw,%s,-8($sp)\n",reg_name[a]);	
					fprintf(fout,"\t sw,%s,-12($sp)\n",reg_name[reg_ra]);
					fprintf(fout,"\t sw,%s,-16($sp)\n",reg_name[reg_v0]);
					fprintf(fout,"\t move,$fp,$sp\n");
					fprintf(fout,"\t jal,%s\n",sym_tables[t.src1].name);
					fprintf(fout,"\t lw,$fp,-4($sp)\n");
					fprintf(fout,"\t lw,%s,-12($sp)\n",reg_name[reg_ra]);
					if(t.des !=0)
					{
						b = get_mem(t.des,t.level,&base);
						fprintf(fout,"\t sw,$v0,%d(%s)\n",b,reg_name[base]);
						rel_reg(base);
					}				
					rel_reg(a);	
					fprintf(fout,"\t lw,%s,-16($sp)\n",reg_name[reg_v0]);
					break;

			case four_read:
					a = get_reg();
					fprintf(fout,"\t li,$v0,5\n");
					fprintf(fout,"\t syscall\n");
					fprintf(fout,"\t move %s,$v0\n",reg_name[a]);
					store_into_mem(a,t.des,t.level);
					rel_reg(a);
					break;
			case four_write:
					if (sym_tables[t.des].kind == k_var
					  ||(sym_tables[t.des].kind == k_const&&
						 sym_tables[t.des].type == t_integer))
					{
						a = get_into_reg(t.des,t.level);
						fprintf(fout,"\t li,$v0,1\n");
						fprintf(fout,"\t move,$a0,%s\n",reg_name[a]);
						fprintf(fout,"\t syscall\n");
					}
					else
					{
						fprintf(fout,"\t li,$v0,4\n");
						fprintf(fout,"\t la,$a0,_s%d\n",sym_tables[t.des].x);	
						fprintf(fout,"\t syscall\n");
					}
					break;

			case four_push:
					b = get_into_reg(t.des,t.level);
					fprintf(fout,"\t sw,%s,%d($sp)\n",reg_name[b],-(t.src1*4 + OFFSET));
					rel_reg(b);
					break;

			case four_end:
					if (sym_tables[t.des].kind == k_func)
					{
						a = get_into_reg(t.des,t.level);
						fprintf(fout,"\t move,$v0,%s\n",reg_name[a]);
					}
					fprintf(fout,"\t addi,$sp,$sp,%d\n",(OFFSET + sym_tables[t.des].size));
					if (t.des)
						fprintf(fout,"\t jr,$ra\n");
					break;

			case four_getarr:
					a = get_mem(t.src1,t.level,&base);
					d = get_reg();
					b = get_into_reg(t.src2,t.level);
					c = get_reg();
					fprintf(fout,"\t addi,%s,%s,%d\n",reg_name[d],reg_name[base],a);
					fprintf(fout,"\t li,%s,%d\n",reg_name[c],4);
					fprintf(fout,"\t mult,%s,%s\n",reg_name[c],reg_name[b]);
					fprintf(fout,"\t mflo,%s\n",reg_name[c]);
					fprintf(fout,"\t add,%s,%s,%s\n",reg_name[d],reg_name[c],reg_name[d]);
					fprintf(fout,"\t lw,%s,0(%s)\n",reg_name[c],reg_name[d]);		
					store_into_mem(c,t.des,t.level);
					rel_reg(base);
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
						fprintf(fout,"\t addi,%s,%s,%d\n",reg_name[d],reg_name[base],a);
						fprintf(fout,"\t li,%s,%d\n",reg_name[c],4);
						fprintf(fout,"\t mult,%s,%s\n",reg_name[c],reg_name[b]);
						fprintf(fout,"\t mflo,%s\n",reg_name[c]);
						fprintf(fout,"\t add,%s,%s,%s\n",reg_name[d],reg_name[c],reg_name[d]);
						store_into_mem(d,t.des,t.level);
						rel_reg(b);
						rel_reg(c);
						rel_reg(d);
					}
					else
					{
						a = get_mem(t.src1,t.level,&base);
						b = get_reg();
						fprintf(fout,"\t addi,%s,%s,%d\n",reg_name[b],reg_name[base],a);
						store_into_mem(b,t.des,t.level);
						rel_reg(b);
					}
					rel_reg(base);
					break;
			case four_label:
					fprintf(fout,"$L%d:\n",t.des);
					break;
			case four_enter:
					fprintf(fout,"%s:\n",sym_tables[t.des].name);
					fprintf(fout,"\t addi,$sp,$sp,%d\n",-( OFFSET + sym_tables[t.des].size));
					break;
			default:
					error();//shabi
		}
	}
	fprintf(fout,"\t li,$v0,10\n");
	fprintf(fout,"\t syscall");
	fclose(fout);
}
