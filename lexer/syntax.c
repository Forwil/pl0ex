/*
 * =====================================================================================
 *
 *       Filename:  syntax.c
 *
 *    Description:  syntaxer of PL0EX
 *
 *        Version:  1.0
 *        Created:  11/27/2013 01:31:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Forwil, 
 *        Company:  BUAA
 *
 * =====================================================================================
 */

#include<name.h>
#include<stdio.h>
#define MAXVARDEC 100
#define MAXSYMTABLE 200
#define MAXSYMNAME	1000
#define MAXFOURCODE	2000

#define k_var		1
#define k_const		2
#define k_func		3
#define k_proc		4
#define k_point		5

#define t_char 		1
#define t_integer 	2
#define t_string 	3

#define four_add	1
#define four_sub	2
#define four_mul	3
#define four_div	4

#define four_big	5
#define four_smo	6
#define four_bige	7
#define four_smoe	8
#define four_eq		9
#define four_neq	10

#define four_jmp	11
#define four_jz		12
#define four_bec	13
#define four_call	14

#define four_read	15
#define four_write	16
#define four_push	17
#define four_pop	18
#define four_end	19

#define four_getarr 20

struct sym_table
{
	char *name;
	int kind; 	// 
	int type;	//
	int x;	// 
			//
			//
			//
	int level;
	int last;
	int reg;// when we want to generate machine code... we will use that.
	int mem;	
}

struct four_expression
{
	int type;
	int src1;
	int src2;
	int des;
}

struct four_expression four_codes[MAXFOURCODE];
int four_codep = 1;

char sym_name[MAXSYMNAME];
int sym_namep = 1,sym_tablep = 1,nowlevel = -1;
struct sym_table sym_tables[MAXSYMTABLE];

extern void getsym();
extern void lexer_init();
extern int symtype;
extern char sym[MAXSYM];
extern int num;

int insert_four(int type,int src1,int src2,int des)
{
	four_codes[four_codep].type = type;
	four_codes[four_codep].src1 = src1;
	four_codes[four_codep].src2 = src2;
	four_codes[four_codep].des = des;
	four_codep += 1;
	return four_codep - 1;
}
void set_des_four(int ind,int des)
{
	four_codes[ind].des = des;
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

int insert_sym_table(char a[],int kind);
{
	int i;
	sym_tables[sym_tablep].kind = kind;
	sym_tables[sym_tablep].x = 0;
	sym_tables[sym_tablep].level = nowlevel;
	sym_tables[sym_tablep].name = new_sym_name(a);
	// set "last" linker
	i = sym_tablep - 1;
	while(i > 0 && sym_tables[i].level < nowlevel) i -= 1;
	sym_tables[sym_tablep].last = i;
	
	sym_tablep += 1;	
	return sym_tablep - 1;
}

int new_temp_var_sym_table();
{
	int i;
	i = insert_sym_table("",k_var);
	settype_sym_table(i,0,t_integer);
	return i;
}

int new_temp_const_sym_table(int x);
{
	int i;
	i = insert_sym_table("",k_const);
	settype_sym_table(i,x,t_integer);
	return i;
}

void settype_sym_table(int ind,int x,int type)
{
	sym_tables[ind].type = type;
	sym_tables[ind].x  = x;
}

void uplevel_sym_table(int ind)
{
	sym_tables[ind].level += 1;
}

void find_sym_table(char a[])
{
	int i;
	i = sym_tablep - 1;
	while(i)
	{
		if (strcmp(sym_tables[i].name,a) == 0)
			return i;
		i = sym_tables[i].last;
	}
	return 0;	
}

int const_value()
{
	int t;
	if (symtype == PLUS)
	{
		getsym();
		t = num;
	}
	else if (symtype == MINUS)
	{
		getsym();
		t = -num;
	}
	else if (symtype == T_CHAR)
	{
		t = num;
	}
	else
		error(); // unknown const value
	return t;
}

void const_declare()
{
	char ident[MAXSYM];
	int t,p;
	if (symtype == T_IDENT)
	{
		strcpy(ident,sym);
		getsym();
		if(symtype == BECOME)
		{
			getsym();
			t = const_value();
			p = insert_sym_table(ident,k_const);
			if(symtype == T_CHAR)
				settype_sym_table(p,t,t_char);
			else
				settype_sym_table(p,t,t_integer);
			getsym();
		}
		else
			error();	// missing BECOME
	}	
}

void get_type(struct sym_table *t)
{
	if (symtype == INT)
	{
		t->type = t_integer;
		t->x = 0;
	}
	else if (symtype == CHAR)
	{
		t->type= t_char;
		t->x = 0;
	}
	else if (symtype == ARRAY)
	{
		getsym();
		if (symtype == LBP)
		{
			getsym();
			if(symtype == T_CONST)
			{
				t->x = num;
				getsym();
				if (symtype == RBP)
					getsym();
				else
					error(); // missing LBP
				if (symtype == OF)
					getsym();
				else
					error(); // missing OF
				if (symtype == INT)
					t->type = t_integer;
				else if (symtype == CHAR)
					t->type = t_char;
				else 
					error(); // unknown type	
			}
			else
				error(); //must be a unsigned integer
		}
		else
			error(); //missing L big p	
	}
	else
		error(); // missing ARRAY
	getsym();
}

void var_declare()
{
	int p[MAXVARDEC];
	int nvdec = 0,i;
	struct sym_table t;
	while (symtype == T_IDENT)
	{
		//insert var indent to sym_table
		p[nvdec] = insert_sym_table(sym,k_var); // no demain yet!
		nvdec += 1;
		getsym();
		if (symtype == COMMA)
			getsym();
		else if (symtype == COLON)
		{
			getsym();
			get_type(&t);
			for(i = 0;i < nvdec;i++)
				settype_sym_table(p[i],t.x,t.type);// set all var to t_type
			break;			
		}
		else
			error();// missing COMMA
	}
}

void proc_declare()
{
	char ident[MAXSYM];
	int p,x;
	getsym();
	if (symtype == T_IDENT)
	{
		strcmp(ident,sym);
		p = insert_sym_table(sym,k_proc);
		nowfather = p;
		getsym();
		if (symtype == LP)
		{
			getsym();
			x = form_arguments();//deal with form_arguments
			settype_sym_table(p,x,0);
			if (symtype == RP)
				getsym();
			else
				error();//missing RP
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
			part_pro(p);
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
		}	
		else
			error();//missing LP
	}
	else
		error();//missing ident	
}

void func_declare()
{
	int p;
	const sym_table t;
	getsym();
	if (symtype == T_IDENT)
	{
		p = insert_sym_table(sym,k_func); // unknown x
		nowfather = p;
		getsym();
		if (symtype == LP)
		{
			getsym();
			t.x = form_arguments();//deal with form_arguments
			if (symtype == RP)
				getsym();
			else
				error();// missing RP
			if (symtype == COLON)
				getsym();
			else
				error();//missing COLON
			if (symtype == INT)
				t.type = t_integer;
			else if (symtype == CHAR)
				t.type = t_char;
			else
				error();//unknown type
			settype_sym_table(p,t.x,t.type);
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
			part_pro(p);
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
		}	
		else
			error();//missing LP
	}
	else
		error();//missing ident	
}

void real_arguments()
{
	int t;
	t = expression();
	insert_four(four_push,0,0,t);
	while(symtype == COMMA)
	{
		getsym();
		t = expression();
		insert_four(four_push,0,0,t);
	}	
}

void form_arguments()
{
	int kind = 0,count = 0,t,i,nvdec;
	int p[MAXVARDEC];
	if (symtype != VAR && symtype != T_IDENT)
		error();//error type of form arguments
	while(symtype == VAR || symtype == T_IDENT)
	{
		nvdec = 0;
		kind = k_var;
		if (symtype == VAR)
		{
			getsym();
			kind = k_point;
			//mark some...
		}
		while (symtype == T_IDENT)
		{
			// do something about the ident
			p[nvdec]= insert_sym_table(sym,kind);
			nvdec += 1;
			count += 1;
			getsym();
			if (symtype == COMMA)
				getsym();
		}
		if (symtype == COLON)
			getsym();
		else
			error();//missing COLON
		if (symtype == INT)
			t = t_integer;
		else if(symtype == CHAR)
			t = t_char;
		else
			error();// unknow type
		for(i = 0;i < nvdec;i++)
		{
			settype_sym_table(p[i],0,t);
			uplevel_sym_table(p[i]);	// the arguements must be level up
		}
		if (symtype ==SEM)
			getsym();
		else
			break;
	}
	return count;
}

void statement()
{
	int a,b,c,d,i,t1,t2;
	if (symtype == T_IDENT)
	{
		a = find_sym_table(sym);
		if (a == 0)
			error();// can't find ident
		if (sym_tables[a].kind == k_const)
			error();// it can't be const
		getsym();
		if (symtype == BECOME)
		{
			getsym();// do something for BECOME
			b = expression();
			insert_four(four_bec,b,0,a);
		}
		else if (symtype == LBP)
		{
			getsym();// do something for ARRAY-INDEX
			b = expression();
			if (symtype == RBP)
				getsym();
			else
				error();//missing RBP
			if (symtype == BECOME)
				getsym();
			else
				error();//missing BECOME
			c = expressiong();
			insert_four(four_bec,c,b,a);
			// do something for BECOME 
		}
		else if (symtype == LP)
		{
			getsym();
			if (symtype == RP)
				getsym();
			else
			{
				real_arguments();
				if (symtype == RP)
					getsym();
				else
					error();// missing RP
			}
			// do something for CALL
			b = sym_tables[a].x;
			insert_four(four_call,a,b,0);
		}
		else
			error();//error BECOME statement
	}
	else if (symtype == IF)
	{
		getsym();
		a = condition();
		t1 = insert_four(four_jz,a,0,0);
		// do something for condition
		if (symtype == THEN)
			getsym();
		else
			error();// missing THEN
		statement();
		if (symtype == ELSE)
		{
			t2 = insert_four(four_jmp,0,0,0);
			set_des_four(t1,four_tablep);
			getsym();
			statement();
			set_des_four(t2,four_tablep);
			// do something for IF-THEN-ELSE
		}
		else
		{
			set_des_four(t1,four_tablep);
			// do something for IF-THEN
		}
	}
	else if (symtype == REPEAT)
	{
		getsym();
		t1 = four_tablep;
		statement();
		if (symtype == UNTIL)
			getsym();
		else
			error();//missing UNTIL
		a = condiction();	
		insert_four(four_jz,a,0,t1);
		// do some thing for REPEAT-UNTIL
	}
	else if (symtype == BEGIN)
	{
		getsym();
		statement();
		while(symtype == SEM)
		{
			getsym();
			statement();
		}
		if (symtype == END)
			getsym();
		else
			error();//missing END
	}
	else if (symtype == READ)
	{
		getsym();
		if (symtype == LP)
			getsym();
		else
			error();//missing LP
		if (symtype !=T_IDENT)
			error();//missing ident
		while(symtype == T_IDENT)
		{
			// do something for T_IDENT
			a = find_sym_table(sym);
			insert_four(four_read,0,0,a);
			getsym();
			if (symtype == COMMA)
				getsym();
			else if (symtype == RP)
			{
				getsym();
				break;
			}
			else
				error();// missing COMMA or RP
		}
		
	}
	else if (symtype == WRITE)
	{
		getsym();
		if (symtype == LP)
			getsym();
		else
			error();//missing LP
		if (symtype == T_STRING)
		{	
			a = insert_sym_table(sym,k_const);
			settype_sym_table(a,0,t_string);
			insert_four(four_write,0,0,a);
			//do something for string
			getsym();
			if (symtype == COMMA)
			{
				getsym();
				a = expression();
				insert_four(four_write,0,0,a);
				// do some for expressiong
			}
			else if (symtype == RP)
				getsym();
			else
				error();// missing type
		}
		else
		{
			a = expression; 
			insert_four(four_write,0,0,a);
			// do something for expression
		}	
	}
	else if (symtype == FOR)
	{
		getsym();
		if (symtype == T_IDENT)
		{
			// save ident
			a = find_sym_table(sym);
			getsym();
			if (symtype == BECOME)
				getsym();
			else
				error();// missing BECOME
			b = expression();
			insert_four(four_bec,b,0,a);
			if (symtype == TO)
			{
				// mark for TO
				t1 = 1;
			}
			else if (symtype == DOWNTO)
			{
				// mark for DOWNTO
				t1 = -1;
			}
			else
				error();// unknow type for FOR
			c = new_temp_const_sym_table(t1);
			t1 = four_tablep;	
			d = expression();
			t2 = new_temp_var_sym_table();
			insert_four(four_smoe,a,d,t2);
			b = insert_four(four_jz,t2,0,0);
			if (symtype == DO)
				getsym();
			else
				error();// missing DO
			statement();
			insert_four(four_add,a,c,a);
			insert_four(four_jmp,0,0,t1);	
			set_des_four(b,four_tablep);
			// do important something for FOR
		}
		else
			error();//missing ident	
	}
	else;
		// do nothing!
}

int expression();
{
	int a,f,t,c,mflag = 0;
	if (symtype == PLUS)
		getsym();
	else if (symtype == MINUS)
	{
		getsym();
		// minus flag
		mflag = 1;
	}
	a = term();
	while(symtype == PLUS || symtype == MINUS)
	{
		// do something for PLUS and MINUS
		f = symtype;
		getsym();
		b = term();
		t = new_temp_var_four();
		if (f == MINUS)
			insert_four(four_sub,a,b,t);
		else
			insert_four(four_add,a,b,t);
		a = t;
	}
	if (mflag)
	{
		t = new_temp_var_four();
		c = new_temp_const_four(0);
		insert_four(four_sub,c,a,t);
		a = t;
	}
	return a;
}

int term()
{
	int a,b,f,t;
	a = factor();
	while(symtype == MULT || symtype == DIV)
	{
		// do something for MULT and DIV
		f = symtype;
		getsym();
		b = factor();
		t = new_temp_var_four();
		if (f == MULT)
			insert_four(four_mul,a,b,t);
		else
			insert_four(four_div,a,b,t);
		a = t
	}
	return a;
}

int factor()
{
	int a,b,t;
	if (symtype == T_IDENT)
	{
		a = find_sym_table(sym);
		// do some for ident
		getsym();
		if (symtype == LBP)
		{
			getsym();
			b = expression();
			if (symtype == RBP)
				getsym();
			else
				error();//missing RBP
			// do some for array index
			t = new_temp_var_symtable();
			insert_four(four_getarr,a,b,t);
			a = t;
		}
		else if (symtype == LP)
		{
			getsym();
			real_arguements();
			t = new_temp_var_symtable();
			insert_four(four_call,a,symtables[a].x,t);
			// deal with real_arguements
			if (symtype == RP)
				getsym();
			else
				error();
			a = t;
		}
	}
	else if (symtype == T_CONST)
	{
		//do some for CONST
		a = new_temp_const_table(num);
		getsym();
	}
	else if (symtype == LP)
	{
		getsym();
		a = expression();
		// do something for expression
		if (symtype == RP)
			getsym();
		else
			error();// missing RP
	}
	else 
		error();// empty factor
	return a;
}

int condition()
{
	int a,t,f;
	a = expression();
	if (symtype != EQ &&
		symtype != NEQ &&
		symtype != SMO &&
		symtype != SMOE &&
		symtype != BIG &&
		symtype != BIGE)
		error();// unknown bj
	f = symtype;
	getsym();
	t = new_temp_var_sym_table();
 	b = expression();
	switch (f) 
	{
		case EQ: 
			insert_four(four_eq,a,b,t); break;
		case NEQ: 
			insert_four(four_neq,a,b,t); break;
		case SMO: 
			insert_four(four_smo,a,b,t); break;
		case SMOE: 
			insert_four(four_smoe,a,b,t); break;
		case BIG: 
			insert_four(four_big,a,b,t); break;
		case BIGE: 
			insert_four(four_bige,a,b,t); break;
		default :
			error();// what the fuck!
	}
	return t;
	// do something for CONDITION
}

void part_pro(int name);
{
	int i,f;
	if(name)
		for (i = sym_tables[name].x;i >= 1;i--)
			insert_four(four_pop,0,0,name + i);
	nowlevel += 1;
	i = insert_four(four_jmp,0,0,0);
	if (symtype == CONST)
	{
		getsym();
		const_declare();
		while(symtype == COMMA)
		{
			getsym();
			const_declare();
		}
		if(symtype == SEM)
			getsym();
		else
			error();//missing SEM
	}
	
	if (symtype == VAR)
	{
		getsym();
		while(symtype == T_IDENT)
		{
			var_declare();	
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
		}
	}
	f = nowfather;	
	while (symtype == PROC || symtype == FUNC)
	{
		if (symtype == PROC)
			proc_declare();	
		if (symtype == FUNC)
			func_declare();	
	}
	nowfather = f;
	set_four_des(i,four_tablep);
	if (symtype == BEGIN)
	{
		getsym();
		statement();	
		while(symtype == SEM)
		{
			getsym();
			statement();
		}
		if (symtype == END)
			getsym();
		else
			error();//missing END
	}
	else
		error();// no part_pro
	nowlevel -= 1;
}

int main(void)
{
	
	lexer_init();
	sym_tables[0].name = NULL;
	sym_tables[0].x = 0;
	sym_tables[0].type = 0;
	sym_tables[0].level = -1;
	getsym();
	part_pro(0);
	if (symtype == PERIOD)
		getsym();
	else
		error();//missing PERIOD
	return 0;
}
