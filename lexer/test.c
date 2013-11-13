/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  test
 *
 *        Version:  1.0
 *        Created:  11/12/2013 06:14:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Forwil 
 *        Company:  BUAA
 *
 * =====================================================================================
 */

#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
		int
main ( int argc, char *argv[] )
{
		int i;
		for (i = 32;i<127;++i)
			{
			printf("%d:%c\t",i,i);
			}
		return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

