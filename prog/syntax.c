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

#include<stdio.h>
#include"error.h"
#include"lexer.h"
#include"four.h"
#include"symtable.h"

#define MAXVARDEC 100

int nowlevel;
void part_pro(int);

void init_syntax()
{
	nowlevel = 0;
	sym_tables[0].name = "_main";
	sym_tables[0].x = 0;
	sym_tables[0].type = 0;
	sym_tables[0].level = -1;
	sym_tables[0].size = 0;
}

/*
 * deal with real arguments, with every argument ,insert a PUSH four_yuan_code
 */

int deal_var()
{
	int a,b = 0,t;
	if (symtype == T_IDENT)
	{
		a = find_sym_table(sym);
		if (sym_tables[a].kind == k_const || sym_tables[a].kind == k_proc)
			my_error(3);// my_error type
		getsym();
		if(symtype == LBP)
		{
			getsym();
			b = expression();
			if(symtype == RBP)
			{
				getsym();
				if (sym_tables[a].x == 0)
					my_error(3);// it is not array
				t = new_temp_var_sym_table(); 
				insert_four(four_getadd,a,b,t);
			}
			else
				my_error(4);// missing RBP
		}
		else if(symtype == COMMA || symtype == RP)
		{
			t = new_temp_var_sym_table();
			insert_four(four_getadd,a,0,t);
		}
		else
			my_error(5);// my_error type 
	}
	else
		my_error(6);// must be ident	
	return t;
} 

void real_arguments(int f)
{
	int t,i;
	for( i = 1;i <= sym_tables[f].x;i++)
	{	
		if(sym_tables[f+i].kind == k_point)
		{
			t = deal_var();
			insert_four(four_push,i,0,t);
		}
		else
		{
			t = expression();
			insert_four(four_push,i,0,t);
		}
		if (symtype == COMMA)
			getsym();
		else if(((symtype == RP) ^ (i == sym_tables[f].x))!=0)
			my_error(7);// arguments my_error
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
		if (symtype != T_IDENT)
			my_error(6);// no ident
		while (symtype == T_IDENT)
		{
			// do something about the ident
			//printf("\n \n shsb\n");
			t = find_sym_table(sym);
			//printf("\n %d  %d\n",sym_tables[t].level,nowlevel);
			if (t == 0|| sym_tables[t].level!=nowlevel)
				p[nvdec]= insert_sym_table(sym,kind);
			else
				my_error(8);// redefined 
			nvdec += 1;
			count += 1;
			getsym();
			if (symtype == COMMA)
				getsym();
		}
		if (symtype == COLON)
			getsym();
		else
			my_error(9);//missing COLON
		if (symtype == INT)
			t = t_integer;
		else if(symtype == CHAR)
			t = t_char;
		else
			my_error(5);// unknow type
		getsym();
		for(i = 0;i < nvdec;i++)
		{
			settype_sym_table(p[i],0,t);
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
		my_error(10); // unknown const value
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
				my_error(8);// redefined
			if(symtype == T_CHAR)
				settype_sym_table(p,t,t_char);
			else
				settype_sym_table(p,t,t_integer);
			getsym();
		}
		else
			my_error(11);	// missing BECOME
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
					my_error(12); // missing LBP
				if (symtype == OF)
					getsym();
				else
					my_error(30); // missing OF
				if (symtype == INT)
					t->type = t_integer;
				else if (symtype == CHAR)
					t->type = t_char;
				else 
					my_error(5); // unknown type	
			}
			else
				my_error(10); //must be a unsigned integer
		}
		else
			my_error(12); //missing L big p	
	}
	else
		my_error(13); // missing ARRAY
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
			my_error(8);// redefined
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
			my_error(5);// missing COMMA or COLON
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
			my_error(8);// redefine
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
				my_error(14);//missing RP
			if (symtype == SEM)
				getsym();
			else
				my_error(15);//missing SEM
			part_pro(p);
			if (symtype == SEM)
				getsym();
			else
				my_error(15);//missing SEM
		}	
		else
			my_error(16);//missing LP
	}
	else
		my_error(6);//missing ident	
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
			my_error(8);// redefine
		getsym();
		nowlevel += 1;
		if (symtype == LP)
		{
			getsym();
			t.x = form_arguments();//deal with form_arguments
			if (symtype == RP)
				getsym();
			else
				my_error(14);// missing RP
			if (symtype == COLON)
				getsym();
			else
				my_error(9);//missing COLON
			if (symtype == INT)
				t.type = t_integer;
			else if (symtype == CHAR)
				t.type = t_char;
			else
				my_error(5);//unknown type
			settype_sym_table(p,t.x,t.type);
			getsym();
			if (symtype == SEM)
				getsym();
			else
				my_error(15);//missing SEM
			part_pro(p);
			if (symtype == SEM)
				getsym();
			else
				my_error(15);//missing SEM
		}	
		else
			my_error(16);//missing LP
	}
	else
		my_error(6);//missing ident	
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
			my_error(17);// can't find ident
		if (sym_tables[a].kind == k_const)
			my_error(18);// it can't be const
		getsym();
		if (symtype == BECOME)
		{
			getsym();// do something for BECOME
			b = expression();
			if (sym_tables[a].type != k_proc)
				insert_four(four_bec,b,0,a);	
			else
				my_error(19);// cann't assign value to a proc
		}
		else if (symtype == LBP)
		{
			getsym();// do something for ARRAY-INDEX
			b = expression();
			if (symtype == RBP)
				getsym();
			else
				my_error(4);//missing RBP
			if (symtype == BECOME)
				getsym();
			else
				my_error(11);//missing BECOME
			c = expression();
			if (sym_tables[a].kind == k_var &&
				sym_tables[a].x > 0)
				insert_four(four_bec,c,b,a);
			else
				my_error(20);// the var is not array type
			// do something for BECOME 
		}
		else if (symtype == LP)
		{
			getsym();
			if (symtype == RP)
				getsym();
			else
			{
				real_arguments(a);
				if (symtype == RP)
					getsym();
				else
					my_error(14);// missing RP
			}
			// do something for CALL
			if (sym_tables[a].kind == k_proc)
			{
				b = sym_tables[a].x;
				insert_four(four_call,a,b,0);
			}
			else
				my_error(21);// it is not proc
		}
		else
			my_error(5);//my_error BECOME statement
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
			my_error(22);// missing THEN
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
			my_error(23);//missing UNTIL
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
			my_error(24);//missing END
	}
	else if (symtype == READ)
	{
		getsym();
		if (symtype == LP)
			getsym();
		else
			my_error(16);//missing LP
		if (symtype !=T_IDENT)
			my_error(6);//missing ident
		while(symtype == T_IDENT)
		{
			// do something for T_IDENT
			a = find_sym_table(sym);
			if (a != 0 && (sym_tables[a].kind == k_var
						|| sym_tables[a].kind == k_func
						|| sym_tables[a].kind == k_point)) 
				insert_four(four_read,0,0,a);
			else
				my_error(25);// cann't read to a static ident
			getsym();
			if (symtype == COMMA)
				getsym();
			else if (symtype == RP)
			{
				getsym();
				break;
			}
			else
				my_error(5);// missing COMMA or RP
		}
	}
	else if (symtype == WRITE)
	{
		getsym();
		if (symtype == LP)
			getsym();
		else
			my_error(16);//missing LP
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
				my_error(5);// missing type
		}
		else
		{
			a = expression(); 
			insert_four(four_write,0,0,a);
			if (symtype == RP)
				getsym();
			else
				my_error(5);// missing type
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
				my_error(6);// FOR value isn't var or unfind
			getsym();
			if (symtype == BECOME)
				getsym();
			else
				my_error(11);// missing BECOME
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
				my_error(5);// unknow type for FOR
			getsym();
			c = new_temp_const_sym_table(t3);
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
				my_error(26);// missing DO
			statement();
			insert_four(four_add,a,c,a);
			insert_four(four_jmp,0,0,t1);
			t2 = new_label_four();	
			set_des_four(b,t2);
			// do important something for FOR
		}
		else
			my_error(6);//missing ident	
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
		//printf("\n\t %s %d %d\n",sym_tables[a].name,sym_tables[a].kind,a);
		if (a == 0 ||   (sym_tables[a].kind!=k_var
					 &&  sym_tables[a].kind!=k_func
					 &&  sym_tables[a].kind!=k_point
					 &&  sym_tables[a].kind!=k_const))
			my_error(5);// can get value from it
		// do some for ident
		getsym();
		if (symtype == LBP)
		{
			getsym();
			b = expression();
			if (symtype == RBP)
				getsym();
			else
				my_error(4);//missing RBP
			// do some for array index
			if ( sym_tables[a].kind == k_var &&
				 sym_tables[a].x >0)
			{
				t = new_temp_var_sym_table();
				insert_four(four_getarr,a,b,t);
				a = t;
			}
			else
				my_error(20);// it is not array
		}
		else if (symtype == LP)
		{
			getsym();
			if (sym_tables[a].kind != k_func)
				my_error(27);// it is not func
			real_arguments(a);
			t = new_temp_var_sym_table();
			insert_four(four_call,a,sym_tables[a].x,t);
			// deal with real_arguements
			if (symtype == RP)
				getsym();
			else
				my_error(14);
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
			my_error(14);// missing RP
	}
	else 
		my_error(28);// empty factor
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
		my_error(5);// unknown bj
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
			my_error(5);// what the fuck!
	}
	return t;
	// do something for CONDITION
}

void part_pro(int name)
{
	int i,f,t;
/*  
	if(name)
		for (i = sym_tables[name].x;i >= 1;i--)
			insert_four(four_pop,0,0,name + i);
*/
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
			my_error(15);//missing SEM
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
				my_error(15);//missing SEM
		}
	}
	while (symtype == PROC || symtype == FUNC)
	{
		if (symtype == PROC)
			proc_declare();	
		if (symtype == FUNC)
			func_declare();	
	}
	
	t = new_enter_four(name);

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
			my_error(24);//missing END
	}
	else
		my_error(29);// no part_pro
	nowlevel -= 1;
	insert_four(four_end,0,0,name);
}

int main(int argc,char *argv[])
{
	init_lexer(argc,argv);
	printf("init lexer success!\n");
	init_sym_table();
	printf("init symtable success!\n");
	init_syntax();
	printf("init syntax success!\n");
	init_four();
	printf("init four success!\n");

	printf("------------------------------------------\n");
	printf("-             Starting Complier          -\n");
	printf("------------------------------------------\n");
	getsym();
	part_pro(0);
	if (symtype == PERIOD)
		getsym();
	else
		my_error(31);//missing PERIOD

	printf("------------------------------------------\n");
	printf("-           Complie Success!             -\n");
	printf("------------------------------------------\n");
	gen_block();
	printf("\nStart generate mips code\n");	
	init_mips();
	gen_mips();
	printf("\nSuccess!\nGenerate into \"test/result.asm\"\n");	
	return 0;
}
