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

#include"lexer.h"
#include"four.h"
#include"symtable.h"

#define MAXVARDEC 100

int nowlevel;
void part_pro(int);

void init_syntax()
{
	nowlevel = -1;
	sym_tables[0].name = 0;
	sym_tables[0].x = 0;
	sym_tables[0].type = 0;
	sym_tables[0].level = -1;
}

/*
 * deal with real arguments, with every argument ,insert a PUSH four_yuan_code
 */
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
/*
 *	deal with form arguments,with every arguments,if it has VAR flag, insert it with point type
 *	then finally return the number of arguments
 */

int form_arguments()
{
	int kind = 0,count = 0,t,i,nvdec;
	int p[MAXVARDEC];
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
		if (symtype == T_IDENT)
			error();// no ident
		while (symtype == T_IDENT)
		{
			// do something about the ident
			t = find_sym_table(sym);
			if (t == 0|| sym_tables[t].level!=nowlevel)
				p[nvdec]= insert_sym_table(sym,kind);
			else
				error();// redefined 
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
		getsym();
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

/*
 * deal with const_declare's const value
 * it can be a integer with minus,or a char type ,
 * a char type also be a 0-255 integer
 */

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
	else if (symtype == T_CHAR || symtype == T_CONST)
	{
		t = num;
	}
	else
		error(); // unknown const value
	return t;
}

/*
 * const declare 
 */

void const_declare()
{
	char ident[MAXSYM];
	int t,p;
	if (symtype == T_IDENT)
	{
		strcpy(ident,sym);
		getsym();
		if(symtype == EQ)
		{
			getsym();
			t = const_value();
			p = find_sym_table(ident);
			if (p == 0 || sym_tables[p].level!=nowlevel)
				p = insert_sym_table(ident,k_const);
			else
				error();// redefined
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
/* 
 * in call a struct sym_table t,
 * get a single type or array type
 * store it in struct sym_table t
 */
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
/*
 * variables declare
 */
void var_declare()
{
	int p[MAXVARDEC];
	int nvdec = 0,i;
	struct sym_table t;
	while (symtype == T_IDENT)
	{
		//insert var indent to sym_table
		i = find_sym_table(sym);
		if (i == 0 || sym_tables[i].level!=nowlevel)
			p[nvdec] = insert_sym_table(sym,k_var); // no demain yet!
		else
			error();// redefined
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
			error();// missing COMMA or COLON
	}
}
/*
 *	procedure declare 
 *	after procedure name insert
 *	we let nowlevel plus one
 */
void proc_declare()
{
	char ident[MAXSYM];
	int p,x;
	getsym();
	if (symtype == T_IDENT)
	{
		strcmp(ident,sym);
		p = find_sym_table(sym);
		if (p == 0 || sym_tables[p].level!=nowlevel)
			p = insert_sym_table(sym,k_proc);
		else
			error();// redefine
		getsym();
		nowlevel += 1;
		if (symtype == LP)
		{
			getsym();
			if (symtype == VAR || symtype == T_IDENT)
			{
				x = form_arguments();//deal with form_arguments
				settype_sym_table(p,x,0);
			}
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
/*
 * function declare
 * after function name insert
 * we let nowlevel plus one
 */
void func_declare()
{
	int p;
	struct sym_table t;
	getsym();
	if (symtype == T_IDENT)
	{
		p = find_sym_table(sym);
		if (p == 0 || sym_tables[p].level != nowlevel)
			p = insert_sym_table(sym,k_func); // unknown x
		else
			error();// redefine
		getsym();
		nowlevel += 1;
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
			getsym();
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
/*
 *
 */
void statement()
{
	int a,b,c,d,i,t1,t2,t3;
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
			if (sym_tables[a].type != k_proc)
				insert_four(four_bec,b,0,a);	
			else
				error();// cann't assign value to a proc
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
			c = expression();
			if (sym_tables[a].kind == k_var &&
				sym_tables[a].x > 0)
				insert_four(four_bec,c,b,a);
			else
				error();// the var is not array type
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
			if (sym_tables[a].kind == k_proc)
			{
				b = sym_tables[a].x;
				insert_four(four_call,a,b,0);
			}
			else
				error();// it is not proc
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
			a = new_label_four();
			set_des_four(t1,a);
			getsym();
			statement();
			a = new_label_four();
			set_des_four(t2,a);
			// do something for IF-THEN-ELSE
		}
		else
		{
			a = new_label_four();
			set_des_four(t1,a);
			// do something for IF-THEN
		}
	}
	else if (symtype == REPEAT)
	{
		getsym();
		t1 = new_label_four();
		statement();
		if (symtype == UNTIL)
			getsym();
		else
			error();//missing UNTIL
		a = condition();	
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
			if (a != 0 && (sym_tables[a].kind == k_var
						|| sym_tables[a].kind == k_func
						|| sym_tables[a].kind == k_point)) 
				insert_four(four_read,0,0,a);
			else
				error();// cann't read to a static ident
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
				// do some for expression
			}
			if (symtype == RP)
				getsym();
			else
				error();// missing type
		}
		else
		{
			a = expression(); 
			insert_four(four_write,0,0,a);
			if (symtype == RP)
				getsym();
			else
				error();// missing type
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
			if (a == 0 || sym_tables[a].kind !=k_var)
				error();// FOR value isn't var or unfind
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
				t3 = 1;
			}
			else if (symtype == DOWNTO)
			{
				// mark for DOWNTO
				t3 = -1;
			}
			else
				error();// unknow type for FOR
			getsym();
			c = new_temp_const_sym_table(t1);
			t1 = new_label_four();	
			d = expression();
			t2 = new_temp_var_sym_table();
			if (t3 == 1)
				insert_four(four_smoe,a,d,t2);
			else
				insert_four(four_bige,a,d,t2);
			b = insert_four(four_jz,t2,0,0);
			if (symtype == DO)
				getsym();
			else
				error();// missing DO
			statement();
			insert_four(four_add,a,c,a);
			insert_four(four_jmp,0,0,t1);
			t2 = new_label_four();	
			set_des_four(b,t2);
			// do important something for FOR
		}
		else
			error();//missing ident	
	}
	else;
		// do nothing!
}
/*
 * expression
 */
int expression()
{
	int a,b,f,t,c,mflag = 0;
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
		t = new_temp_var_sym_table();
		if (f == MINUS)
			insert_four(four_sub,a,b,t);
		else
			insert_four(four_add,a,b,t);
		a = t;
	}
	if (mflag)
	{
		t = new_temp_var_sym_table();
		c = new_temp_const_sym_table(0);
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
		t = new_temp_var_sym_table();
		if (f == MULT)
			insert_four(four_mul,a,b,t);
		else
			insert_four(four_div,a,b,t);
		a = t;
	}
	return a;
}

int factor()
{
	int a,b,t;
	if (symtype == T_IDENT)
	{
		a = find_sym_table(sym);
		if (a == 0 || (sym_tables[a].kind!=k_var
					   sym_tables[a].kind!=k_func
					   sym_tables[a].kind!=k_point))
			error();// can get value from it
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
			if ( sym_tables[a].kind == k_var &&
				 sym_tables[a].x >0)
			{
				t = new_temp_var_sym_table();
				insert_four(four_getarr,a,b,t);
				a = t;
			}
			else
				error();// it is not array
		}
		else if (symtype == LP)
		{
			getsym();
			if (sym_tables[a].kind != k_func)
				error();// it is not func
			real_arguments();
			t = new_temp_var_sym_table();
			insert_four(four_call,a,sym_tables[a].x,t);
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
		a = new_temp_const_sym_table(num);
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
	int a,b,t,f;
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

void part_pro(int name)
{
	int i,f,t;
	t = new_enter_four(name);
	if(name)
		sym_tables[name].mem = t;
	if(name)
		for (i = sym_tables[name].x;i >= 1;i--)
			insert_four(four_pop,0,0,name + i);
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
	while (symtype == PROC || symtype == FUNC)
	{
		if (symtype == PROC)
			proc_declare();	
		if (symtype == FUNC)
			func_declare();	
	}
	t = new_label_four();	
	set_des_four(i,t);
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
	if (name)
		if(sym_tables[name].kind == k_proc)
			insert_four(four_end,0,0,0);
		else
			insert_four(four_end,0,0,name);
}


int main(void)
{
	init_lexer();
	printf("init lexer success!\n");
	init_sym_table();
	printf("init symtable success!\n");
	init_syntax();
	printf("init syntax success!\n");
	getsym();
	part_pro(0);
	if (symtype == PERIOD)
		getsym();
	else
		error();//missing PERIOD
	out_all_four();
	return 0;
}
