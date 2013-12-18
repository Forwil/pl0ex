# 1 "syntax.c"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "syntax.c"
# 19 "syntax.c"
# 1 "lexer.h" 1
# 83 "lexer.h"
char sym[255];

void init_lexer();
int getsym();
int num,symtype;
# 20 "syntax.c" 2
# 1 "four.h" 1
# 30 "four.h"
struct four_expression
{
 int type;
 int src1;
 int src2;
 int des;
};

struct four_expression four_codes[2000];
int four_tablep,four_labelp;

int insert_four(int,int,int,int);
int new_label_four();
int new_enter_four();
void set_des_four(int,int);
void init_four();
void out_all_four();
void out_one(struct four_expression);
# 21 "syntax.c" 2
# 1 "symtable.h" 1
# 14 "symtable.h"
struct sym_table
{
 char *name;
 int kind;
 int type;
 int x;
 int level;
 int last;
 int reg;
 int mem;
};

char sym_name[1000];
int sym_namep,sym_tablep;
struct sym_table sym_tables[200];

void init_sym_table();
char *new_sym_name(char *);
int insert_sym_table(char *,int);
void settype_sym_table(int,int,int);
int new_temp_var_sym_table();
int new_temp_const_sym_table(int);
void uplevel_sym_table(int);
int find_sym_table(char *);
# 22 "syntax.c" 2



int nowlevel;
void part_pro(int);

void init_syntax()
{
 nowlevel = -1;
 sym_tables[0].name = 0;
 sym_tables[0].x = 0;
 sym_tables[0].type = 0;
 sym_tables[0].level = -1;
}




void real_arguments()
{
 int t;
 t = expression();
 insert_four(17,0,0,t);
 while(symtype == 27)
 {
  getsym();
  t = expression();
  insert_four(17,0,0,t);
 }
}





int form_arguments()
{
 int kind = 0,count = 0,t,i,nvdec;
 int p[100];
 while(symtype == 20 || symtype == 42)
 {
  nvdec = 0;
  kind = 1;
  if (symtype == 20)
  {
   getsym();
   kind = 5;

  }
  if (symtype == 42)
   error();
  while (symtype == 42)
  {

   t = find_sym_table(sym);
   if (t == 0|| sym_tables[t].level!=nowlevel)
    p[nvdec]= insert_sym_table(sym,kind);
   else
    error();
   nvdec += 1;
   count += 1;
   getsym();
   if (symtype == 27)
    getsym();
  }
  if (symtype == 30)
   getsym();
  else
   error();
  if (symtype == 12)
   t = 2;
  else if(symtype == 3)
   t = 1;
  else
   error();
  getsym();
  for(i = 0;i < nvdec;i++)
  {
   settype_sym_table(p[i],0,t);
   uplevel_sym_table(p[i]);
  }
  if (symtype ==29)
   getsym();
  else
   break;
 }
 return count;
}







int const_value()
{
 int t;
 if (symtype == 23)
 {
  getsym();
  t = num;
 }
 else if (symtype == 24)
 {
  getsym();
  t = -num;
 }
 else if (symtype == 44 || symtype == 43)
 {
  t = num;
 }
 else
  error();
 return t;
}





void const_declare()
{
 char ident[255];
 int t,p;
 if (symtype == 42)
 {
  strcpy(ident,sym);
  getsym();
  if(symtype == 36)
  {
   getsym();
   t = const_value();
   p = find_sym_table(ident);
   if (p == 0 || sym_tables[p].level!=nowlevel)
    p = insert_sym_table(ident,2);
   else
    error();
   if(symtype == 44)
    settype_sym_table(p,t,1);
   else
    settype_sym_table(p,t,2);
   getsym();
  }
  else
   error();
 }
}





void get_type(struct sym_table *t)
{
 if (symtype == 12)
 {
  t->type = 2;
  t->x = 0;
 }
 else if (symtype == 3)
 {
  t->type= 1;
  t->x = 0;
 }
 else if (symtype == 1)
 {
  getsym();
  if (symtype == 32)
  {
   getsym();
   if(symtype == 43)
   {
    t->x = num;
    getsym();
    if (symtype == 31)
     getsym();
    else
     error();
    if (symtype == 13)
     getsym();
    else
     error();
    if (symtype == 12)
     t->type = 2;
    else if (symtype == 3)
     t->type = 1;
    else
     error();
   }
   else
    error();
  }
  else
   error();
 }
 else
  error();
 getsym();
}



void var_declare()
{
 int p[100];
 int nvdec = 0,i;
 struct sym_table t;
 while (symtype == 42)
 {

  i = find_sym_table(sym);
  if (i == 0 || sym_tables[i].level!=nowlevel)
   p[nvdec] = insert_sym_table(sym,1);
  else
   error();
  nvdec += 1;
  getsym();
  if (symtype == 27)
   getsym();
  else if (symtype == 30)
  {
   getsym();
   get_type(&t);
   for(i = 0;i < nvdec;i++)
    settype_sym_table(p[i],t.x,t.type);
   break;
  }
  else
   error();
 }
}





void proc_declare()
{
 char ident[255];
 int p,x;
 getsym();
 if (symtype == 42)
 {
  strcmp(ident,sym);
  p = find_sym_table(sym);
  if (p == 0 || sym_tables[p].level!=nowlevel)
   p = insert_sym_table(sym,4);
  else
   error();
  getsym();
  nowlevel += 1;
  if (symtype == 34)
  {
   getsym();
   if (symtype == 20 || symtype == 42)
   {
    x = form_arguments();
    settype_sym_table(p,x,0);
   }
   if (symtype == 33)
    getsym();
   else
    error();
   if (symtype == 29)
    getsym();
   else
    error();
   part_pro(p);
   if (symtype == 29)
    getsym();
   else
    error();
  }
  else
   error();
 }
 else
  error();
}





void func_declare()
{
 int p;
 struct sym_table t;
 getsym();
 if (symtype == 42)
 {
  p = find_sym_table(sym);
  if (p == 0 || sym_tables[p].level != nowlevel)
   p = insert_sym_table(sym,3);
  else
   error();
  getsym();
  nowlevel += 1;
  if (symtype == 34)
  {
   getsym();
   t.x = form_arguments();
   if (symtype == 33)
    getsym();
   else
    error();
   if (symtype == 30)
    getsym();
   else
    error();
   if (symtype == 12)
    t.type = 2;
   else if (symtype == 3)
    t.type = 1;
   else
    error();
   settype_sym_table(p,t.x,t.type);
   getsym();
   if (symtype == 29)
    getsym();
   else
    error();
   part_pro(p);
   if (symtype == 29)
    getsym();
   else
    error();
  }
  else
   error();
 }
 else
  error();
}



void statement()
{
 int a,b,c,d,i,t1,t2,t3;
 if (symtype == 42)
 {
  a = find_sym_table(sym);
  if (a == 0)
   error();
  if (sym_tables[a].kind == 2)
   error();
  getsym();
  if (symtype == 35)
  {
   getsym();
   b = expression();
   if (sym_tables[a].type != 4)
    insert_four(13,b,0,a);
   else
    error();
  }
  else if (symtype == 32)
  {
   getsym();
   b = expression();
   if (symtype == 31)
    getsym();
   else
    error();
   if (symtype == 35)
    getsym();
   else
    error();
   c = expression();
   if (sym_tables[a].kind == 1 &&
    sym_tables[a].x > 0)
    insert_four(13,c,b,a);
   else
    error();

  }
  else if (symtype == 34)
  {
   getsym();
   if (symtype == 33)
    getsym();
   else
   {
    real_arguments();
    if (symtype == 33)
     getsym();
    else
     error();
   }

   if (sym_tables[a].kind == 4)
   {
    b = sym_tables[a].x;
    insert_four(14,a,b,0);
   }
   else
    error();
  }
  else
   error();
 }
 else if (symtype == 11)
 {
  getsym();
  a = condition();
  t1 = insert_four(12,a,0,0);

  if (symtype == 17)
   getsym();
  else
   error();
  statement();
  if (symtype == 7)
  {
   t2 = insert_four(11,0,0,0);
   a = new_label_four();
   set_des_four(t1,a);
   getsym();
   statement();
   a = new_label_four();
   set_des_four(t2,a);

  }
  else
  {
   a = new_label_four();
   set_des_four(t1,a);

  }
 }
 else if (symtype == 16)
 {
  getsym();
  t1 = new_label_four();
  statement();
  if (symtype == 19)
   getsym();
  else
   error();
  a = condition();
  insert_four(12,a,0,t1);

 }
 else if (symtype == 2)
 {
  getsym();
  statement();
  while(symtype == 29)
  {
   getsym();
   statement();
  }
  if (symtype == 8)
   getsym();
  else
   error();
 }
 else if (symtype == 15)
 {
  getsym();
  if (symtype == 34)
   getsym();
  else
   error();
  if (symtype !=42)
   error();
  while(symtype == 42)
  {

   a = find_sym_table(sym);
   if (a != 0 && (sym_tables[a].kind == 1
      || sym_tables[a].kind == 3
      || sym_tables[a].kind == 5))
    insert_four(15,0,0,a);
   else
    error();
   getsym();
   if (symtype == 27)
    getsym();
   else if (symtype == 33)
   {
    getsym();
    break;
   }
   else
    error();
  }
 }
 else if (symtype == 22)
 {
  getsym();
  if (symtype == 34)
   getsym();
  else
   error();
  if (symtype == 45)
  {
   a = insert_sym_table(sym,2);
   settype_sym_table(a,0,3);
   insert_four(16,0,0,a);

   getsym();
   if (symtype == 27)
   {
    getsym();
    a = expression();
    insert_four(16,0,0,a);

   }
   if (symtype == 33)
    getsym();
   else
    error();
  }
  else
  {
   a = expression();
   insert_four(16,0,0,a);
   if (symtype == 33)
    getsym();
   else
    error();

  }
 }
 else if (symtype == 9)
 {
  getsym();
  if (symtype == 42)
  {

   a = find_sym_table(sym);
   if (a == 0 || sym_tables[a].kind !=1)
    error();
   getsym();
   if (symtype == 35)
    getsym();
   else
    error();
   b = expression();
   insert_four(13,b,0,a);
   if (symtype == 18)
   {

    t3 = 1;
   }
   else if (symtype == 6)
   {

    t3 = -1;
   }
   else
    error();
   getsym();
   c = new_temp_const_sym_table(t1);
   t1 = new_label_four();
   d = expression();
   t2 = new_temp_var_sym_table();
   if (t3 == 1)
    insert_four(8,a,d,t2);
   else
    insert_four(7,a,d,t2);
   b = insert_four(12,t2,0,0);
   if (symtype == 5)
    getsym();
   else
    error();
   statement();
   insert_four(1,a,c,a);
   insert_four(11,0,0,t1);
   t2 = new_label_four();
   set_des_four(b,t2);

  }
  else
   error();
 }
 else;

}



int expression()
{
 int a,b,f,t,c,mflag = 0;
 if (symtype == 23)
  getsym();
 else if (symtype == 24)
 {
  getsym();

  mflag = 1;
 }
 a = term();
 while(symtype == 23 || symtype == 24)
 {

  f = symtype;
  getsym();
  b = term();
  t = new_temp_var_sym_table();
  if (f == 24)
   insert_four(2,a,b,t);
  else
   insert_four(1,a,b,t);
  a = t;
 }
 if (mflag)
 {
  t = new_temp_var_sym_table();
  c = new_temp_const_sym_table(0);
  insert_four(2,c,a,t);
  a = t;
 }
 return a;
}

int term()
{
 int a,b,f,t;
 a = factor();
 while(symtype == 25 || symtype == 26)
 {

  f = symtype;
  getsym();
  b = factor();
  t = new_temp_var_sym_table();
  if (f == 25)
   insert_four(3,a,b,t);
  else
   insert_four(4,a,b,t);
  a = t;
 }
 return a;
}

int factor()
{
 int a,b,t;
 if (symtype == 42)
 {
  a = find_sym_table(sym);
  if (a == 0 || (sym_tables[a].kind!=1
        sym_tables[a].kind!=3
        sym_tables[a].kind!=5 ))
   error();

  getsym();
  if (symtype == 32)
  {
   getsym();
   b = expression();
   if (symtype == 31)
    getsym();
   else
    error();

   if ( sym_tables[a].kind == 1 &&
     sym_tables[a].x >0)
   {
    t = new_temp_var_sym_table();
    insert_four(20,a,b,t);
    a = t;
   }
   else
    error();
  }
  else if (symtype == 34)
  {
   getsym();
   if (sym_tables[a].kind != 3)
    error();
   real_arguments();
   t = new_temp_var_sym_table();
   insert_four(14,a,sym_tables[a].x,t);

   if (symtype == 33)
    getsym();
   else
    error();
   a = t;
  }
 }
 else if (symtype == 43)
 {

  a = new_temp_const_sym_table(num);
  getsym();
 }
 else if (symtype == 34)
 {
  getsym();
  a = expression();

  if (symtype == 33)
   getsym();
  else
   error();
 }
 else
  error();
 return a;
}

int condition()
{
 int a,b,t,f;
 a = expression();
 if (symtype != 36 &&
  symtype != 37 &&
  symtype != 38 &&
  symtype != 39 &&
  symtype != 40 &&
  symtype != 41)
  error();
 f = symtype;
 getsym();
 t = new_temp_var_sym_table();
  b = expression();
 switch (f)
 {
  case 36:
   insert_four(9,a,b,t); break;
  case 37:
   insert_four(10,a,b,t); break;
  case 38:
   insert_four(6,a,b,t); break;
  case 39:
   insert_four(8,a,b,t); break;
  case 40:
   insert_four(5,a,b,t); break;
  case 41:
   insert_four(7,a,b,t); break;
  default :
   error();
 }
 return t;

}

void part_pro(int name)
{
 int i,f,t;
 t = new_enter_four(name);
 if(name)
  sym_tables[name].mem = t;
 if(name)
  for (i = sym_tables[name].x;i >= 1;i--)
   insert_four(18,0,0,name + i);
 i = insert_four(11,0,0,0);
 if (symtype == 4)
 {
  getsym();
  const_declare();
  while(symtype == 27)
  {
   getsym();
   const_declare();
  }
  if(symtype == 29)
   getsym();
  else
   error();
 }

 if (symtype == 20)
 {
  getsym();
  while(symtype == 42)
  {
   var_declare();
   if (symtype == 29)
    getsym();
   else
    error();
  }
 }
 while (symtype == 14 || symtype == 10)
 {
  if (symtype == 14)
   proc_declare();
  if (symtype == 10)
   func_declare();
 }
 t = new_label_four();
 set_des_four(i,t);
 if (symtype == 2)
 {
  getsym();
  statement();
  while(symtype == 29)
  {
   getsym();
   statement();
  }
  if (symtype == 8)
   getsym();
  else
   error();
 }
 else
  error();
 nowlevel -= 1;
 if (name)
  if(sym_tables[name].kind == 4)
   insert_four(19,0,0,0);
  else
   insert_four(19,0,0,name);
}


int main(void)
{
 init_lexer();
 printf("init lexer success!\n");
 init_sym_table();
 printf("init symtable success!\n");
 init_syntax();
 printf("init syntax success!\n");
 getsym();
 part_pro(0);
 if (symtype == 28)
  getsym();
 else
  error();

 printf("---------------------\n");
 printf("-  Complie Success! -\n");
 printf("---------------------\n");

 out_all_four();
 return 0;
}
