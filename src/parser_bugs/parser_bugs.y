%{
#include <stdio.h>
#include <string.h>
#include <gc.h>
#include "bugs_node.h"

BUGS_NODE* g_bugsnode = NULL;

%}


%defines
%name-prefix="bugs"
%union
{
  double val;
  char *stringptr;
  struct _BUGS_NODE *pnode;
}

%token VAR
%token DATA
%token MODEL
%token <stringptr> NAME
%token <stringptr> FUNC
%token <stringptr> SPECIAL
%token IN
%token ARROW
%token FOR
%token GT GE LT LE EQ NE 
%token AND OR
%token LENGTH 
%token DIM

%token <val> DOUBLE
%token <val> INT

%left OR
%left AND
%left NOT
%nonassoc GT GE LT LE EQ NE
%left '+' '-'
%left '*' '/'
%left SPECIAL
%left NEG
%right '^'


%token ENDL

%type <pnode> node_dec
%type <pnode> expression var
%type <pnode> relation for_loop counter
%type <pnode> determ_relation stoch_relation  
%type <pnode> range_element range_element_list
%type <pnode> distribution relations
%type <pnode> dec_list relation_list expression_list dim_list 
%type <pnode> product sum
%type <pnode> data_stmt var_stmt model_stmt input
%expect 2

%%
input:   /* empty */
| model_stmt { 
	g_bugsnode = bugs_node_create(BN_LIST);
	bugs_node_addparam(g_bugsnode, $1); 
}
| var_stmt model_stmt {
	g_bugsnode = bugs_node_create(BN_LIST);
	bugs_node_addparam(g_bugsnode, $2); 
}
| data_stmt model_stmt {
	g_bugsnode = bugs_node_create(BN_LIST);
	bugs_node_addparam(g_bugsnode, $2); 
}
| var_stmt data_stmt model_stmt {
	g_bugsnode = bugs_node_create(BN_LIST);
	bugs_node_addparam(g_bugsnode, $3); 
}
;

var_stmt: VAR dec_list
| VAR dec_list ';'
;

dec_list: node_dec
| dec_list ',' node_dec
;

node_dec: NAME
| NAME '[' dim_list ']' 
;

dim_list: expression
| dim_list ',' expression
;

data_stmt: DATA relations {
	$$ = $2;
}
;

model_stmt: MODEL relations {
	$$ = $2;
}
;
 
relations: '{' relation_list '}' {
	$$ = bugs_node_create(BN_RELATIONS);
	$$->params = $2->params;
}
| '{' '}' {
	$$ = bugs_node_create(BN_RELATIONS);
}
;

relation_list: relation {
	$$ = bugs_node_create(BN_LIST);
	bugs_node_addparam($$, $1);
}
| relation_list relation {
	$$ = $1;
	bugs_node_addparam($$, $2);
}
;

relation: stoch_relation
| determ_relation 
| for_loop
| stoch_relation ';'
| determ_relation ';'
| error ';' { yyerrok; }
;

for_loop: counter relations {
	$$ = bugs_node_create(BN_FOR);
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $2);
}
;

counter: FOR '(' NAME IN range_element ')' {
	$$ = bugs_node_create(BN_COUNTER);
	bugs_node_setname($$, $3);
	bugs_node_addparam($$, $5);
}
;

determ_relation: var ARROW expression {
	$$ = bugs_node_create(BN_DETERMREL);
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| var error {
	char buf[1024];
	sprintf(buf, "variable '%s' has no right expression", $1->name);
	yyerror(buf);
	YYERROR;
	yyerrok;
}
| FUNC '(' var ')' ARROW expression {
	BUGS_NODE *p = bugs_node_create(BN_LINK);
	bugs_node_setname(p, $1);
	bugs_node_addparam(p, $6);
	$$ = bugs_node_create(BN_DETERMREL);
	bugs_node_addparam($$, $3);
	bugs_node_addparam($$, p);
}
| FUNC '(' var ')' error {
	char buf[1024];
	sprintf(buf, "variable '%s(%s)' has no right expression", $1, $3->name);
	yyerror(buf);
	YYERROR;
	yyerrok;
}
;

stoch_relation:	var '~' distribution {
	$$ = bugs_node_create(BN_STOCHREL);
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
} 
;

product: expression '*' expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "*");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| product '*' expression {
	$$ = $1;
	bugs_node_addparam($$, $3);
}
;

sum: expression '+' expression{
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "+");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| sum '+' expression {
	$$ = $1;
	bugs_node_addparam($$, $3);
}
;

expression: var {
	$$ = $1;
}
| DOUBLE {
	$$ = bugs_node_create(BN_VALUE);
	bugs_node_addvalue($$, $1);
}
| LENGTH '(' var ')' {
	$$ = bugs_node_create(BN_LENGTH);
	bugs_node_addparam($$, $3);
}
| DIM '(' var ')' {
	$$ = bugs_node_create(BN_DIM);
	bugs_node_addparam($$, $3);
}
| FUNC '(' expression_list ')' {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, $1);
	$$->params = $3->params;
}
| product {
	$$ = $1;
//	bugs_node_create(BN_FUNC);
//	bugs_node_setname($$, "*");
//	bugs_node_addparam($$, $1);
}
| expression '/' expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "/");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| sum {
	$$ = $1;
}
| expression '-' expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "-");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| '-' expression %prec NEG {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "NEG");
	bugs_node_addparam($$, $2);
}
| expression GT expression { 
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, ">");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression GE expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, ">=");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression LT expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "<");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression LE expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "<=");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression EQ expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "==");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression NE expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "!=");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression AND expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "^");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression OR expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "^");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression '^' expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, "^");
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| expression SPECIAL expression {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, $2);
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
| '(' expression ')' {
	$$ = $2;
}
;

expression_list: expression {
	$$ = bugs_node_create(BN_LIST);
	bugs_node_addparam($$, $1);
}
| expression_list ',' expression {
	$$ = $1;
	bugs_node_addparam($$, $3);
}
;

range_element_list: range_element {
	$$ = bugs_node_create(BN_LIST);
	bugs_node_addparam($$, $1);
}
| range_element_list ',' range_element {
	$$ = $1;
	bugs_node_addparam($$, $1);
};

range_element: /*empty*/
| expression {
	$$ = bugs_node_create(BN_RANGE);
	bugs_node_addparam($$, $1);
}
| expression ':' expression {
	$$ = bugs_node_create(BN_RANGE);
	bugs_node_addparam($$, $1);
	bugs_node_addparam($$, $3);
}
;

distribution: FUNC '(' expression_list ')' {
	$$ = bugs_node_create(BN_FUNC);
	bugs_node_setname($$, $1);
	$$->params = $3->params;
	//bugs_node_addparam($$, $3);
}
;

var: NAME {
	$$ = bugs_node_create(BN_VAR);
	bugs_node_setname($$, $1);
}
| NAME '[' expression_list ']' {
	$$ = bugs_node_create(BN_VAR);
	bugs_node_setname($$, $1);
	$$->params = $3->params;
	//bugs_node_addparam($$, $3);
}
| NAME '[' ']' {
	$$ = bugs_node_create(BN_VAR);
	bugs_node_setname($$, $1);
}
| NAME '[' error ']' {
	char buf[1024];
	sprintf(buf, "index expression of an array '%s' has error", $1);
	yyerror(buf);
	YYERROR;
	yyerrok;
}
;

%%

/*
int main(int argc, char **argv)
{
#ifdef  YYDEBUG
        extern int bugsdebug;
        bugsdebug = 0;
#endif

        GC_INIT();

        g_bugsnode = NULL;
        if(bugsparse() != 0)
        {
                printf("Err\n");
                return -1;
        }

        printf("ok\n");
        bugs_node_dump(g_bugsnode);

        return 0;
}
*/

extern int bugslineno;
extern char* bugstext;
extern int bugsnerrs;
int yyerror (const char *s)
{
	fprintf(stderr, "Error#%d: %s (near '%s' [line:%d])\n", bugsnerrs, s, bugstext, bugslineno);
}

int bugswrap()
{
        return 1;
}

