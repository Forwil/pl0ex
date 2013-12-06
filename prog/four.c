#include "four.h"

int insert_four(int type,int src1,int src2,int des)
{
	four_codes[four_codep].type = type;
	four_codes[four_codep].src1 = src1;
	four_codes[four_codep].src2 = src2;
	four_codes[four_codep].des = des;
	four_codep += 1;
	return four_codep - 1;
}

int new_label_four()
{
	int t;
	t = insert_four(four_label,0,0,four_labelp);
	four_labelp += 1;
	return t; 	
}

void set_des_four(int ind,int des)
{
	four_codes[ind].des = des;
}

void init_four()
{
	four_codep = 1;
	four_labelp = 0;
}
