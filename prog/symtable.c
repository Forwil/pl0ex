#include "symtable.h"
#include<stdio.h>
extern int nowlevel;

void init_sym_table()
{
	sym_namep = 1;
	sym_tablep = 1;
	sym_stringp = 1;
}

char *new_sym_name(char a[])
{
	int i = 0;
	char * r;
	r = &sym_name[sym_namep];
	while(a[i])
	{
		sym_name[sym_namep] = a[i];
		sym_namep += 1;
		i += 1;
	}
	sym_name[sym_namep] = '\0';
	sym_namep += 1;
	return r;
}

int insert_sym_table(char a[],int kind)
{
	int i;
	sym_tables[sym_tablep].kind = kind;
	sym_tables[sym_tablep].x = 0;
	sym_tables[sym_tablep].level = nowlevel;
	sym_tables[sym_tablep].name = new_sym_name(a);
	// set "last" linker
	i = sym_tablep - 1;
	while(sym_tables[i].level > nowlevel) 
		i = sym_tables[i].last;
	sym_tables[sym_tablep].last = i;
	//printf("\t\t\t\t\t\t%s %d %d %d\n",a,sym_tablep,i,nowlevel);
	sym_tablep += 1;	
	return sym_tablep - 1;
}
/*  
void uplevel_sym_table(int ind)
{
	int i;
	sym_tables[ind].level += 1;
	i = ind - 1;
	while(i > 0 && sym_tables[i].level > sym_tables[ind].level) 
		i -= 1;
	sym_tables[sym_tablep].last = i;
}

*/


void settype_sym_table(int ind,int x,int type)
{
	sym_tables[ind].type = type;
	if (type == t_string)
		sym_tables[ind].x = sym_stringp++;
	else
		sym_tables[ind].x  = x;
}

int new_temp_var_sym_table()
{
	static int i = 0;
	int t;
	char s[20];
	sprintf(s,"_t%d",i++);
	t = insert_sym_table(s,k_var);
	settype_sym_table(t,0,t_integer);
	return t;
}

int new_temp_const_sym_table(int x)
{
	int i;
	i = insert_sym_table("",k_const);
	settype_sym_table(i,x,t_integer);
	return i;
}

int find_sym_table(char a[])
{
	int i;
//	printf("\nlooking for %s\n",a);
	i = sym_tablep - 1;
	while(sym_tables[i].level > nowlevel)
		i = sym_tables[i].last;
	while(i)
	{
//		printf("\t\t%d\n",i);
		if (strcmp(sym_tables[i].name,a) == 0)
			return i;
		i = sym_tables[i].last;
	}
	return 0;	
}
