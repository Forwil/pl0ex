
#define MAXFOURCODE	2000

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
#define four_label	21

struct four_expression
{
	int type;
	int src1;
	int src2;
	int des;
};

struct four_expression four_codes[MAXFOURCODE];
int four_tablep;

int insert_four(int,int,int,int);
void set_des_four(int,int);
void init_four();