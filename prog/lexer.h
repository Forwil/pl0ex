/*
 * =====================================================================================
 *
 *       Filename:  lexer.h
 *
 *    Description:  The include file of lexer.c
 *
 *        Version:  1.0
 *        Created:  11/12/2013 02:38:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Forwil 
 *        Company:  BUAA
 *
 * =====================================================================================
 */

#define NALPHABET 21
#define MAXLINE 255
#define MAXSYM 255
#define MAXVARDEC 100
#define c2i(c) (c-'0') 
#define iss(c) ((c)>=32 && (c)<=126 && (c)!=34)

/*
 * reserve words  1-22
 * */
#define ARRAY	1	
#define BEGIN	2
#define CHAR	3
#define CONST	4
#define DO		5
#define DOWNTO	6
#define ELSE	7
#define END		8
#define FOR		9
#define FUNC	10
#define IF		11
#define INT		12
#define OF		13
#define PROC	14
#define READ	15
#define REPEAT	16
#define THEN	17
#define TO		18
#define UNTIL	19
#define VAR		20
#define WHILE	21
#define WRITE	22

/* 
 * other symbol 23-41
 * */
#define	PLUS	23
#define	MINUS	24
#define	MULT	25
#define	DIV		26
#define	COMMA	27
#define	PERIOD	28
#define	SEM		29
#define	COLON	30
#define	RBP		31
#define	LBP		32
#define	RP		33
#define	LP		34
#define	BECOME	35
#define	EQ		36
#define NEQ		37
#define SMO		38
#define SMOE	39
#define	BIG		40
#define BIGE	41

/* 
 * type symbol 42-45
 * */
#define T_IDENT	42
#define T_CONST	43
#define T_CHAR	44
#define T_STRING	45

char sym[MAXSYM];

void init_lexer();
int getsym();
int num,symtype;

