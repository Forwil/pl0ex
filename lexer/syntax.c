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

#define t_char 		1
#define t_integer 	2
#define t_charc		3
#define t_intc		4
#define t_string 	5
#define t_proc 		6
#define t_func 		7

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

struct symtable
{
	char *name;
	int type;	//undefined 0, char 1,int 2, charc 3, intc 4, string 5, proc 6,func 7
	int x;	// when type is char int ,0 is single,>0 is array,
			// when type is charc intc,it is the value
			// when type is proc func,it is the number of arguements
			// when type is string ,it is not used
	int level;
	int first_son;
	int father;
	int next;
	int reg;// when we want to generate machine code... we will use that.
	int mem;	
}

struct four_expression
{
	int type;
	int src1;
	int f1;// flag1= 0, it means src1 is a normal var,when flag1 <> 0,it means we get src1[f1]
	int src2;
	int f2;
	int des;
	int f3;
}

char symname[MAXSYMNAME];
int symnamep = 1,symtablep = 1,nowlevel = -1,nowfather;
struct symtable symtables[MAXSYMTABLE];

extern void getsym();
extern void lexer_init();
extern int symtype;
extern char sym[MAXSYM];
extern int num;

char *new_sym_name(char a[])
{
	int i = 0;
	char * r;
	r = &symname[symnamep];
	while(a[i])
	{
		symname[symnamep] = a[i];
		symnamep += 1;
		i += 1;
	}
	symname[symnamep] = '\0';
	symnamep += 1;
	return r;
}

int insert_symtable(char a[], int val,int type);
{
	int i;
	symtables[symtablep].type = type;
	symtables[symtablep].x = val;
	symtables[symtablep].level = nowlevel;
	symtables[symtablep].name = new_sym_name(a);
	symtables[symtablep].father = nowfather;
	symtables[symtablep].next = 0;
	symtables[symtablep].first_son = 0;
	i = symtables[nowfather].first_son;
	//set linker
	if (i == 0)
	{	
		symtables[nowfather].first_son = symtablep;
	}
	else
	{	
		while(symtables[i].next)
			i = symtables[i].next;
		symtables[i].next = symtablep;
	}
	symtablep += 1;	
	return symtablep - 1;
}

void settype_symtable(int ind,struct symtable t)
{
	symtables[ind].type = t.type;
	symtables[ind].x  = t.x;
}

void uplevel_symtable(int ind)
{
	symtables[ind].level += 1;
}

void find_symtable(char a[],int type)
{
	int i,f;
	f = nowfather;
	while(f);
	{
		i = symtable[f].first_son;
		while(i)
		{
			if(strcmp(a,symtable[i].name)==0 && symtable[i].type == type)
				return i;
			i = symtable[i].next;
		}
		f = symtable[f].father;	
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
	int t;
	if (symtype == T_IDENT)
	{
		strcpy(ident,sym);
		getsym();
		if(symtype == BECOME)
		{
			getsym();
			t = const_value();
			if(symtype == T_CHAR)
				insert_symtable(ident,t,t_charc); //insert const to symtable
			else
				insert_symtable(ident,t,t_intc);
			getsym();
		}
		else
			error();	// missing BECOME
	}	
}

void get_type(struct symtable *t)
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
	struct symtable t;
	while (symtype == T_IDENT)
	{
		//insert var indent to symtable
		p[nvdec] = insert_symtable(sym,0,0); // no demain yet!
		nvdec += 1;
		getsym();
		if (symtype == COMMA)
			getsym();
		else if (symtype == COLON)
		{
			getsym();
			get_type(&t);
			for(i = 0;i < nvdec;i++)
				settype_symtable(p[i],t);// set all var to t_type
			break;			
		}
		else
			error();// missing COMMA
	}
}

void proc_declare()
{
	char ident[MAXSYM];
	struct symtable t;
	int p;
	getsym();
	if (symtype == T_IDENT)
	{
		strcmp(ident,sym);
		p = insert_symtable(sym,0,0);
		nowfather = p;
		getsym();
		if (symtype == LP)
		{
			getsym();
			t.x = form_arguments();//deal with form_arguments
			t.type = t_proc;
			settype_symtable(p,t);
			if (symtype == RP)
				getsym();
			else
				error();//missing RP
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
			part_pro();
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
	char ident[MAXSYM];
	struct symtable t;
	getsym();
	if (symtype == T_IDENT)
	{
		strcmp(ident,sym);
		p = insert_symtable(sym,0,0); // unknown x
		nowfather = p;
		getsym();
		if (symtype == LP)
		{
			getsym();
			t.x = form_arguments();//deal with form_arguments
			t.type = t_func;
			settype_symtable(p,t);
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
			insert_symtable(ident,0,t.type);
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
			part_pro();
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

void form_arguments()
{
	struct symtable t;
	int isvar = 0,p,count;
	if (symtype != VAR && symtype != T_IDENT)
		error();//error type of form arguments
	while(symtype == VAR || symtype == T_IDENT)
	{
		if (symtype == VAR)
		{
			getsym();
			isvar = -1;
			//mark some...
		}
		while (symtype == T_IDENT)
		{
			// do something about the ident
			p = insert_symtable(sym,0,0);
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
			t.type = t_integer;
		else if(symtype == CHAR)
			t.type = t_char;
		else
			error();// unknow type
		t.x = isvar;	//set VAR type is -1
		settype_symtable(p,t);
		uplevel_symtable(p);	// the arguements must be level up
		if (symtype ==SEM)
			getsym();
		else
			break;
	}
	return count;
}

void statement()
{
	if (symtype == T_IDENT)
	{
		getsym();
		if (symtype == BECOME)
		{
			getsym();// do something for BECOME
			 = expression();
		}
		else if (symtype == LBP)
		{
			getsym();// do something for ARRAY-INDEX
			 = expression();
			if (symtype == RBP)
				getsym();
			else
				error();//missing RBP
			if (symtype == BECOME)
				getsym();
			else
				error();//missing BECOME
			 = expressiong();
			// do something for BECOME 
		}
		else if (symtype == LP)
		{
			getsym();
			real_arguments();
			if (symtype == RP)
				getsym();
			else
				error();// missing RP
			// do something for CALL
		}
		else
			error();//error BECOME statement
	}
	else if (symtype == FUNC)
	{
		getsym();
		if (symtype == BECOME)
		{
			getsym();
			 = expression();
				// do something for FUNC-BECOME
		}
		else
			error();//missing  BECOME
	}
	else if (symtype == IF)
	{
		getsym();
		 = condition();
		// do something for condition
		if (symtype == THEN)
			getsym();
		else
			error();// missing THEN
		statement();
		if (symtype == ELSE)
		{
			getsym();
			statement();
			// do something for IF-THEN-ELSE
		}
		else
		{
			// do something for IF-THEN
		}
	}
	else if (symtype == REPEAT)
	{
		getsym();
		statement();
		if (symtype == UNTIL)
			getsym();
		else
			error();//missing UNTIL
		 = condiction();	
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
			//do something for string
			getsym();
			if (symtype == COMMA)
			{
				getsym();
				 = expression();
				// do some for expressiong
			}
			else if (symtype == RP)
				getsym();
			else
				error();// missing type
		}
		else
		{
			= expression; 
			// do something for expression
		}	
	}
	else if (symtype == FOR)
	{
		getsym();
		if (symtype == T_IDENT)
		{
			// save ident
			getsym();
			if (symtype == BECOME)
				getsym();
			else
				error();// missing BECOME
			 = expression();
			if (symtype == TO)
			{
				// mark for TO
			}
			else if (symtype == DOWNTO)
			{
				// mark for DOWNTO
			}
			else
				error();// unknow type for FOR
			 = expression();
			if (symtype == DO)
				getsym();
			else
				error();// missing DO
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
	if (symtype == PLUS)
		getsym();
	else if (symtype == MINUS)
	{
		getsym();
		// minus flag
	}
	 = term();
	while(symtype == PLUS || symtype == MINUS)
	{
		// do something for PLUS and MINUS
		getsym();
		 = term();
		// do something for term()
	}
	// do something for others
}

int term()
{
	 = factor();
	while(symtype == MULT || symtype == DIV)
	{
		// do something for MULT and DIV
		getsym();
		 = factor();
		// do something for factor();
	}
	// do something for others
}

int factor()
{
	if (symtype == T_IDENT)
	{
		// do some for ident
		getsym();
		if (symtype == LBP)
		{
			getsym();
			 = expression();
			if (symtype == RBP)
				getsym();
			else
				error();//missing RBP
			// do some for array index
		}
		else if (symtype == LP)
		{
			getsym();
			 = real_arguements();
			// deal with real_arguements
			if (symtype == RP)
				getsym();
			else
				error();
		}
	}
	else if (symtype == T_CONST)
	{
		//do some for CONST
		getsym();
	}
	else if (symtype == LP)
	{
		getsym();
		 = expression();
		// do something for expression
		if (symtype == RP)
			getsym();
		else
			error();// missing RP
	}
	else 
		error();// empty factor
}

int condition()
{
	= expression();
	if (symtype == EQ)
	{
	}
	else if (symtype == NEQ)
	{
	}
	else if (symtype == SMO)
	{
	}
	else if (symtype == SMOE)
	{
	}
	else if (symtype == SMOE)
	{
	}
	else if (symtype == BIG)
	{
	}
	else if (symtype == BIGE)
	{
	}
	else
		error();// unknown BJ
 	= expression();
	// do something for CONDITION
}

void part_pro();
{
	int f;
	nowlevel += 1;

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
	symtables[0].name = NULL;
	symtables[0].x = 0;
	symtables[0].type = 0;
	symtables[0].level = -1;
	getsym();
	part_pro();
	if (symtype == PERIOD)
		getsym();
	else
		error();//missing PERIOD
	return 0;
}
