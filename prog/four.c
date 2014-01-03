#include "four.h"
#include "symtable.h"
#include <stdio.h>
int nowlevel;

int insert_four(int type,int src1,int src2,int des)
{
	four_codes[four_tablep].type = type;
	four_codes[four_tablep].src1 = src1;
	four_codes[four_tablep].src2 = src2;
	four_codes[four_tablep].des = des;
	four_codes[four_tablep].level = nowlevel;
	four_tablep += 1;
	///out_one(four_codes[four_tablep-1]);
	return four_tablep - 1;
}

int new_label_four()
{
	int t;
	t = insert_four(four_label,0,0,four_labelp);
	four_labelp += 1;
	return t; 	
}

int new_enter_four(int func)
{
	int t;
	t = insert_four(four_enter,0,0,func);
	return t;	
}

void set_des_four(int ind,int des)
{
	four_codes[ind].des = des;
}

void init_four()
{
	four_tablep = 1;
	four_labelp = 0;
	four_betterp = 1;
	four_blockp = 1;
}

char * out[]=
{
"",
"ADD",
"SUB",
"MUL",
"DIV",
"BIG",
"SMO",
"BIGE",
"SMOE",
"EQ",
"NEQ",
"JMP",
"JZ",
"BEC",
"CALL",
"READ",
"WRITE",
"PUSH",
"POP",
"END",
"GETARR",
"GETADD",
"LABEL",
"ENTER"
};

void out_val(int x)
{
	printf(",");
	if (x == 0)
		printf("-");
	else if (sym_tables[x].kind == k_var ||
			 sym_tables[x].kind == k_func||
			 sym_tables[x].kind == k_proc)
		printf("%s",sym_tables[x].name);
	else if (sym_tables[x].kind == k_const)
	{
		if (sym_tables[x].type == t_string)
			printf("\"%s\"",sym_tables[x].name);
		else if (sym_tables[x].type == t_char)
			printf("%c",sym_tables[x].x);
		else
			printf("%d",sym_tables[x].x);
	}
	else if (sym_tables[x].kind == k_point)
		printf("(%s)",sym_tables[x].name);
}

void out_one(struct four_expression a)
{
	if(a.type == four_label||
	   a.type == four_enter)
		printf("\t%s",out[a.type]);
	else
		printf("\t\t%s",out[a.type]);

	if(a.type == four_jmp ||
	   a.type == four_jz)
	{
		out_val(a.src1);
		printf(",L%d",four_codes[a.des].des);		
	}
	else if (a.type == four_label)
	{
		printf(" L%d:",a.des);
	}
	else if (a.type == four_enter)
	{
		printf(" %s:",sym_tables[a.des].name);
	}
	else if (a.type == four_call)
	{
		printf(",%s,%d",sym_tables[a.src1].name,a.src2);
		out_val(a.des);
	}
	else
	{
		if(a.type !=four_push)
		{
			out_val(a.src1);
			out_val(a.src2);
		}
		out_val(a.des);
	}
	printf("\n");
}

void out_all_four()
{
	int i;
	for(i = 1;i < four_tablep;i ++)
	{
		printf("%d",i);
		out_one(four_codes[i]);
	}
}

void out_all_four2()
{
	int i;
	for(i = 1;i < four_betterp;i ++)
	{
		printf("%d",i);
		out_one(four_better[i]);
	}
}

void gen_block()
{
	int i,t;
	for(i = 1;i<four_tablep;i++)
	{
		if (four_codes[i].type == four_label 
		  ||four_codes[i].type == four_enter)
		{
			if (four_blocks[four_blockp].start != 0)
			{
				four_blocks[four_blockp].end = i - 1;
				four_blockp += 1;
			}
			four_blocks[four_blockp].start = i;
		}
		if (four_codes[i].type == four_jmp
		  ||four_codes[i].type == four_jz)
		{
			if (four_blocks[four_blockp].start !=0)
			{
				four_blocks[four_blockp].end = i;
				four_blockp += 1;
				four_blocks[four_blockp].start = i + 1;
			}
		}
		if (four_codes[i].type == four_end)
		{
			if (four_blocks[four_blockp].start !=0)
			{
				four_blocks[four_blockp].end = i;
				four_blockp += 1;
			}
		}
	}
	if(four_blocks[four_blockp].start != 0)
	{
		four_blocks[four_blockp].end = four_tablep - 1;
		four_blockp += 1;
	}

	for(i = 1;i<four_blockp;i++)
	{
		t = four_blocks[i].start;
		four_blocks[i].level = four_codes[t].level;
		printf("%d %d\n",four_blocks[i].start,four_blocks[i].end);
		build_dag(four_blocks[i].start,four_blocks[i].end,four_blocks[i].level);
	}
	out_all_four2();
}
