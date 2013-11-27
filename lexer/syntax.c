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
#define t_char 1
#define t_integer 2

struct s_type
{
	int base_type;
	int len;  // zero represent one-value var
}

extern void getsym();
extern void lexer_init();
extern int symtype;
extern char sym[MAXSYM];
extern int num;


void constvalue()
{
	if (symtype == PLUS)
		getsym();
	else if (symtype == MINUS)
	{
		getsym();
		num = -num;
	}
	else if (symtype == T_CHAR)
		getsym();
	else
		error(); // unknown const value
}

void constdeclare()
{
	char ident[MAXSYM];
	
	getsym();
	if (symtype != T_IDENT)
		error();	// missing ident
	while (symtype == T_IDENT)
	{
		strcpy(ident,sym);
		getsym();
		if(symtype == BECOME)
		{
			getsym();
			constvalue();
			insert_symtable(sym,CONST); //insert const to symtable
		}
		else
			error();	// missing BECOME
		if(symtype == COMMA)
			getsym();
		else if (symtype == SEM)
		{	
			getsym();
			return ;
		}
		else
			error(); //missing COLON
	}
}

struct s_type gettype()
{
	struct s_type t;
	if (symtype == INT)
	{
		t.base_type = t_integer;
		t.len = 0;
	}
	else if (symtype == CHAR)
	{
		t.base_type = t_char;
		t.len = 0;
	}
	else if (symtype == ARRAY)
	{
		getsym();
		if (symtype == LBP)
		{
			getsym();
			if(symtype == T_CONST)
			{
				t.len = num;
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
					t.base_type = t_integer;
				else if (symtype == CHAR)
					t.base_type = t_char;
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
	return t;
}

void vardeclare()
{
	struct symtable	* p[MAXVARDEC];
	struct s_type t;
	int nvdec = 0,i;
	getsym();
	if (symtype != T_IDENT)
		error() 	//missing ident
	while (symtype == T_IDENT)
	{
		//insert var indent to symtable
		p[nvdec] = insert_symtable(sym,VAR); // no demain yet!
		nvdec += 1;
		getsym();
		if (symtype == COMMA)
			getsym();
		else if (symtype == COLON)
		{
			getsym();
			t = gettype();
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
			for(i = 0;i < nvdec;i++)
				settype_symtable(p[i],t);// set all var to t_type
			break;			
		}
	}
}

void procdeclare()
{
	getsym();
	if (symtype == T_IDENT)
	{
		insert_symtable(sym,PROC);
		getsym();
		if (symtype == LP)
		{
			getsym();
			form_arguments();//deal with form_arguments
			if (symtype == RP)
				getsym();
			else
				error();//missing RP
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
			partpro();
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

void funcdeclare()
{
	struct symtable	* p;
	struct s_type t;
	getsym();
	if (symtype == T_IDENT)
	{
		p = insert_symtable(sym,FUNC);
		getsym();
		if (symtype == LP)
		{
			getsym();
			form_arguments();//deal with form_arguments
			if (symtype == RP)
				getsym();
			else
				error();// missing RP
			if (symtype == COLON)
				getsym();
			else
				error();//missing COLON
			if (symtype == INT)
				t.base_type = t_integer;
			else if (symtype == CHAR)
				t.base_type = t_char;
			else
				error();//unknown type
			settype_symtable(p,t);
			if (symtype == SEM)
				getsym();
			else
				error();//missing SEM
			partpro();
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

var form_arguments()
{
	struct s_type t;
	if (symtype != VAR && symtype != T_IDENT)
		error();//error type of form arguments
	while(symtype == VAR || symtype == T_IDENT)
	{
		if (symtype == VAR)
		{
			getsym();
			//mark some...
		}
		while (symtype == T_IDENT)
		{
			// do something about the ident
			getsym();
			if (symtype == COMMA)
				getsym();
		}
		if (symtype == COLON)
			getsym();
		else
			error();//missing COLON
		if (symtype == INT)
			t.base_type = t_integer;
		else if(symtype == CHAR)
			t.base_type = t_char;
		else
			error();// unknow type
		if (symtype ==SEM)
			getsym();
		else
			break;
	}			
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

void partpro();
{
	if (symtype == CONST)
	{
		constdeclare();
	}
	
	if (symtype == VAR)
	{
		vardeclare();	
	}
	
	while (symtype == PROC || symtype == FUNC)
	{
		if (symtype == PROC)
			procdeclare();	
		if (symtype == FUNC)
			funcdeclare();	
	}
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
}

int main(void)
{
	
	lexer_init();
	getsym();
	partpro();
	if (symtype == PERIOD)
		getsym();
	else
		error();//missing PERIOD
	return 0;
}
