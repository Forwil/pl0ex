/*
 * =====================================================================================
 *
 *       Filename:  name.h
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
 * reserve words  1-21
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
#define INT		11
#define OF		12
#define PROC	13
#define READ	14
#define REPEAT	15
#define THEN	16
#define TO		17
#define UNTIL	18
#define VAR		19
#define WHILE	20
#define WRITE	21

/* 
 * other symbol 21-39
 * */
#define	PLUS	22
#define	MINUS	23
#define	MULT	24
#define	DIV		25
#define	COMMA	26
#define	PERIOD	27
#define	SEM		28
#define	COLON	29
#define	RBP		30
#define	LBP		31
#define	RP		32
#define	LP		33
#define	BECOME	34
#define	EQ		35
#define NEQ		36
#define SMO		37
#define SMOE	38
#define	BIG		39
#define BIGE	40

/* 
 * type symbol 41-44
 * */
#define T_IDENT	41
#define T_CONST	42
#define T_CHAR	43
#define T_STRING	44
