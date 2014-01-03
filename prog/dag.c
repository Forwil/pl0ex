#include<stdio.h>
#include "four.h"
#define MAXP	200
#define MAXT	200

struct point
{
	int op;
	int src1;
	int src2;
};

struct table
{
	int id;
	int to;
	int link;
};

int dag_pointp,dag_tablep;
struct point dag_points[MAXP];
struct table dag_tables[MAXT];

void init_dag()
{
	dag_pointp = 1;
	dag_tablep = 1;
}

int create_point_dag(int type,int a,int b)
{
	dag_points[dag_pointp].op = type;
	dag_points[dag_pointp].src1 = a;
	dag_points[dag_pointp].src2 = b;
	dag_pointp += 1;
	return dag_pointp - 1;
}

int create_table_dag(int id,int to,int link)
{
	dag_tables[dag_tablep].id = id;
	dag_tables[dag_tablep].to = to;
	dag_tables[dag_tablep].link = link;
	dag_tablep += 1;
	return dag_tablep - 1;	
}

int exist(int op,int src1, int src2)
{
	int i;
	for(i = dag_pointp-1;i>0;i--)
		if (op  == dag_points[i].op && 
			src1== dag_points[i].src1&&
			src2== dag_points[i].src2)
		return i;
	return 0;
}


int find_table_dag(int src)
{
	int i;
	for(i = dag_tablep-1;i > 0;i--)
		if (src == dag_tables[i].id)
			return i;
	return 0;
}

int point_to_table(int to)
{
	int i;
	for(i = dag_tablep-1;i>0;i--)
		if (to == dag_tables[i].to)
			return i;
	return 0;
}

int get_point_dag(int src)
{
	int i;
	if (src == 0 )
		return 0;
	i = find_table_dag(src);
//	printf("\n%d %d\n",src,i);
	if (i == 0)
	{
		i = create_point_dag(four_var,0,0);
		i = create_table_dag(src,i,0);
	}
//	printf("%d\n",dag_tables[i].to);
	return dag_tables[i].to;
}

void new_better(int type,int src1,int src2,int des,int level)
{
	four_better[four_betterp].type = type;
	four_better[four_betterp].src1 = src1;
	four_better[four_betterp].src2 = src2;
	four_better[four_betterp].des  = des;
	four_better[four_betterp].level= level;
	four_betterp += 1;
}

void new_point_dag(int type,int a,int b,int des)
{
	int i,t;
	switch(type)
	{
		case four_add: case four_sub: case four_mul: case four_div:
		case four_big: case four_bige:case four_smo: case four_smoe:
		case four_eq:  case four_neq: case four_getarr: case four_getadd: case four_bec:
			if ((t = exist(type,a,b)) == 0)
				t = create_point_dag(type,a,b);	
			i = point_to_table(t);
			if(i)
				create_table_dag(des,t,i);
			else
				create_table_dag(des,t,0);
			break;
		case four_read:
			t = create_point_dag(type,0,0);
			create_table_dag(des,t,0);
			break;
		case four_push:case four_write:
			t = create_point_dag(type,a,b);
			break;
	}
}

void restore_all(int level)
{
	int a,b,c,i,k;
	struct point t;
	for(i=1;i<dag_pointp;i++)
	{
		t = dag_points[i];
		switch(t.op){
		case four_add: case four_sub: case four_mul: case four_div:
		case four_big: case four_bige:case four_smo: case four_smoe:
		case four_eq:  case four_neq: case four_getarr: case four_getadd: case four_bec:		
			a = dag_tables[point_to_table(t.src1)].id;
			b = dag_tables[point_to_table(t.src2)].id;
			k = point_to_table(i);
			c = dag_tables[k].id;
			new_better(t.op,a,b,c,level);
			while(dag_tables[k].link!=0)
			{
				k = dag_tables[k].link;
				a = dag_tables[k].id;
				new_better(four_bec,c,0,a,level);	
			}
		break;
		case four_read:
			a = dag_tables[point_to_table(i)].id;		
			new_better(t.op,0,0,a,level);
		break;
		case four_push:case four_write:
			a = dag_tables[point_to_table(t.src1)].id;
			b = t.src2;
			new_better(t.op,b,0,a,level);
		break;
		}
	}
}

void build_dag(int st,int en,int level)
{
	int i,a,b;
	struct four_expression t;
	init_dag();
	for (i = st;i<=en;i++)
	{
		t = four_codes[i];
//		printf("%d\n",t.type);
		switch(t.type){
		case four_add: case four_sub: case four_mul: case four_div:
		case four_big: case four_bige:case four_smo: case four_smoe:
		case four_eq:  case four_neq: case four_getarr: case four_getadd: case four_bec:
			a = get_point_dag(t.src1);
			b = get_point_dag(t.src2);
			new_point_dag(t.type,a,b,t.des);
			break;	
		case four_read:
			new_point_dag(t.type,0,0,t.des);
			break;
		case four_push: case four_write:
			a = get_point_dag(t.des);
			b = t.src1;
			new_point_dag(t.type,a,b,0);
			break;
		default:
			if (t.type == four_call || t.type == four_end || t.type == four_jz || t.type==four_jmp)
			{
				restore_all(level);
				init_dag();
			}
			new_better(four_codes[i].type,four_codes[i].src1,four_codes[i].src2,four_codes[i].des,four_codes[i].level);
			break;
		}
	}
	restore_all(level);
}

