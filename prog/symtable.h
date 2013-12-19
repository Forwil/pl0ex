#define MAXSYMTABLE 200
#define MAXSYMNAME	1000

#define k_var		1
#define k_const		2
#define k_func		3
#define k_proc		4
#define k_point		5

#define t_char 		1
#define t_integer 	2
#define t_string 	3

struct sym_table
{
	char *name;
	int kind; 	// 
	int type;	//
	int x;		// 
	int level;
	int last;
	int reg;	// when we want to generate machine code... we will use that.
	int mem;	
};

char sym_name[MAXSYMNAME];
int sym_namep,sym_tablep,sym_stringp;
struct sym_table sym_tables[MAXSYMTABLE];

void init_sym_table();
char *new_sym_name(char *);
int insert_sym_table(char *,int);
void settype_sym_table(int,int,int);
int new_temp_var_sym_table();
int new_temp_const_sym_table(int);
//void uplevel_sym_table(int);
int find_sym_table(char *);
