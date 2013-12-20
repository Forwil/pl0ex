/*
 * =====================================================================================
 *
 *       Filename:  lexer.c
 *
 *    Description:  lexer of PL0EX
 *
 *        Version:  1.0
 *        Created:  11/13/2013 02:47:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  forwil
 *        Company:  BUAA
 *
 * =====================================================================================
 */

#include<ctype.h>
#include<stdio.h>
#include "lexer.h"

FILE *fin,*fout;
int symnumber[256];
int linelen,linep,symtype,num,t;
char line[MAXLINE],sym[MAXSYM];
char ch;
char *alphabet[] = 
{
"array",
"begin",
"char",
"const",
"do",
"downto",
"else",
"end",
"for",
"function",
"if",
"integer",
"of",
"procedure",
"read",
"repeat",
"then",
"to",
"until",
"var",
"while",
"write"
};

void my_error(char a[],int b)
{
	char c;
	printf("\n\t\t\tError in file:\" %s \"line:%d\n",a,b);
	c = getchar();
}
void getch()
{
	if(!isspace(ch) && ch!='\"')
		{
		sym[t++] = ch;
		sym[t] = '\0';
		}
	if (linep == linelen)
	{
		if (fgets(line,MAXLINE,fin) == NULL)
		{
			ch = EOF;
			return ;
		}
	//	printf("%s",line);
		linep = 0;
		linelen = strlen(line);
	}
	ch = line[linep];
	putchar(ch);
	fflush(stdout);
	linep += 1;
}


void init_lexer()
{
	char inf[256];
	symnumber['+'] = PLUS;
	symnumber['-'] = MINUS;
	symnumber['*'] = MULT;
	symnumber['/'] = DIV;
	symnumber[','] = COMMA;
	symnumber['.'] = PERIOD;
	symnumber[';'] = SEM;
	symnumber[':'] = COLON;
	symnumber['['] = LBP;
	symnumber[']'] = RBP;
	symnumber['('] = LP;
	symnumber[')'] = RP;
	symnumber['='] = EQ;
	printf("Please input source file name:\n");
	scanf("%s",inf);
	fin = fopen(inf,"r");
	while (fin==NULL)
	{
		printf("source file <%s> name can\'t open,Please try again!\n",inf);
		scanf("%s",inf);
		fin = fopen(inf,"r");
	}
	//fout = fopen("11091222.txt","w");
	getch();
}

int searchident()
{
	int t,w,mid;
	t = 0;
	w = NALPHABET-1;
	while(t<=w)
	{
	mid = (t + w)/2;
	if (strcmp(sym,alphabet[mid])>=0)
		t = mid+1;
	if (strcmp(sym,alphabet[mid])<=0)
		w = mid-1;
	}
	if (t == w+2)
		return mid+1;
	else
		return 0;
}

int getsym()
{
	int  k;
	t = 0;
	while (isspace(ch)) 
		getch();	
	if(ch == EOF)
		return -1;	
	if (isalpha(ch))
	{
		getch();
		while (isalnum(ch))
		{
			getch();
		}
		k = searchident();
	//	printf("%s %d\n",sym,k);
		if (k == 0)
			symtype = T_IDENT;
		else
			symtype = k;
	}
	else if (isdigit(ch))	
	{
		num = c2i(ch);
		getch();
		while (isdigit(ch))	
		{
			num = num * 10 + c2i(ch);
			getch();
		}
		symtype = T_CONST;
	}
	else if (ch =='\'')
	{
		getch();
		num = ch;
		getch();
		if (ch !='\'')
			my_error(__FILE__,__LINE__);
		getch();
		symtype = T_CHAR;
	}
	else if (ch == '\"')
	{
		getch();
		while(iss(ch))
			getch();
		if(!(ch =='\"'))
			my_error(__FILE__,__LINE__);	
		symtype = T_STRING;
		getch();
	}
	else if(ch == ':')
	{
		getch();
		if(ch == '=')
		{
			symtype = BECOME;
			getch();
		}
		else
			symtype = COLON;
	}
	else if (ch == '<')
	{
		getch();
		if(ch == '>')
		{
			symtype = NEQ;
			getch(); 
		}
		else if( ch =='=')
		{
			symtype = SMOE;
			getch();
		}
		else
			symtype = SMO;
	}
	else if (ch == '>')
	{
		getch();
		if(ch == '=')
		{
			symtype = BIGE;
			getch();
		}
		else
			symtype = BIG;
	}
	else
	{
		symtype = symnumber[ch];
		getch();
	}
//	printf("%s\n",sym);
	return 0;
}

char *OUTPUT[]=
{
"",
"ARRAYTK",
"BEGINTK",
"CHARTK",
"CONSTTK",
"DOTK",
"DOWNTOTK",
"ELSETK",
"ENDTK",
"FORTK",
"FUNCTK",
"IFTK",
"INTTK",
"OFTK",
"PROCETK",
"READTK",
"REPTTK",
"THENTK",
"TOTK",
"UNLTK",
"VARTK",
"WHILETK",
"WRITETK",
"PLUS",
"MINU",
"MULT",
"DIV",
"COMMA",
"PERIOD",
"SEMICN",
"COLON",
"LBRACK",
"RBRACK",
"LPARENT",
"RPARENT",
"ASSIGN",
"EQL",
"NEQ",
"LSS",
"LEQ",
"GRE",
"GEQ",
"IDEN",
"INTCON",
"CHARCON",
"STRCON",
};
/*  
int main(void)
{
	int i = 1;
	char inf[255];
	init_lexer();
	printf("Please input source file name:\n");
	scanf("%s",inf);
	fin = fopen(inf,"r");
	while (fin==NULL)
	{
		printf("source file <%s> name can\'t open,Please try again!\n",inf);
		scanf("%s",inf);
		fin = fopen(inf,"r");
	}
	fout = fopen("11091222_token.txt","w");
	while(getsym() != -1)
	{
		fprintf(fout,"%d %s %s\n",i++,OUTPUT[symtype],sym);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
*/
