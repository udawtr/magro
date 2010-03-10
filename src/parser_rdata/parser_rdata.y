%{
#include <stdio.h>
#include "rdata_node.h"

RDATA_NODE* g_rdatanode = NULL;

/* for avoid gcc-warning */
int rdataerror(const char*);
int rdatalex(void);

%}

%defines
%name-prefix="rdata"
%union
{
	int intval;
	double val;
	char* stringptr;
	struct _RDATA_NODE* pnode;
}

%token <intval> INT
%token <val>    DOUBLE
%token <stringptr> NAME
%token <stringptr> STRING

%token <intval> LIST 
%token <intval> STRUCTURE
%token <intval> DIM
%token <intval> NA
%token <intval> R_NULL
%token <intval> DIMNAMES
%token <intval> ITER
%token <intval> ARROW
%token <intval> ENDDATA
%token <intval> ASINTEGER

%type <pnode> r_assignment r_structure
%type <pnode> r_dim 
%type <pnode> r_attribute_list
%type <pnode> r_value 
%type <pnode>  r_value_list r_assignment_list 
%type <pnode> r_value_collection r_integer_collection r_collection
%type <stringptr> r_name;

%%

input: /*empty*/ 
| input line 
;

line: ENDDATA
| r_assignment_list { 
	g_rdatanode = $1;
};
;

/* Rules for scanning dumped R datasets */

r_assignment_list: r_assignment {
	$$ = rdata_node_create(RDN_ASSIGN);
	rdata_node_addparam($$, $1);
} 
| r_assignment_list r_assignment {
	$$ = $1;
	rdata_node_addparam($$, $2);
}
| r_assignment_list ';' r_assignment {
	$$ = $1;
	rdata_node_addparam($$, $3);
}
;

r_assignment: r_name ARROW r_structure { 
	$$ = $3;
	rdata_node_setname($$, $1);
}
| r_name ARROW r_collection {
	$$ = rdata_node_create(RDN_ARRAY);
	rdata_node_setname($$, $1);
	rdata_node_addparam($$, $3);
}
| r_name ARROW STRING {
	RDATA_NODE* p = rdata_node_create(RDN_VAR);
	$$ = rdata_node_create(RDN_VAR);
	rdata_node_setname($$, $1);
	rdata_node_setname(p, $3);;
	rdata_node_addparam($$, p);
}
;

r_name: STRING {
	$$ = $1;
}
| NAME
| '"' NAME '"' 
| '`' NAME '`' {
	$$ = $2;
}

r_structure: STRUCTURE '(' r_collection ',' r_attribute_list ')' {
	$$ = rdata_node_create(RDN_ARRAY);
	if( $5 )
	{
		rdata_node_addparam($$, $3);
		rdata_node_addparam($$, $5);
	}
	else
	{
		rdata_node_addparam($$, $3);
	} 
}
;

/* The only attribute we are interested in is .Dim. The rest are
   simply discarded - see below */
r_attribute_list: r_dim
| r_generic_attribute {$$=0;}
| r_attribute_list ',' r_generic_attribute
| r_attribute_list ',' r_dim { $$=$3;}
;

r_dim: DIM '=' r_collection {
	$$ = $3;
	rdata_node_setname($$, "dim");
}
;

r_collection: r_integer_collection
| r_value_collection
;

r_integer_collection: ASINTEGER '(' r_value_collection ')' {
	$$ = $3;
}
;

r_value_collection: r_value {
	$$ = rdata_node_create(RDN_VECTOR);
	rdata_node_addparam($$, $1);
}
| 'c' '(' r_value_list ')' { 
	$$ = rdata_node_create(RDN_VECTOR);
	rdata_node_addparam($$, $3);
}
;
 
r_value_list: r_value {
	$$ = $1; //RDN_VALUE 
}
| r_value_list ',' r_value {
	$$ = $1;
	rdata_node_addvalue($$, $3->values->items[0]);
	rdata_node_free($3);	
}
;

r_value: DOUBLE {
	$$ = rdata_node_create(RDN_VALUE);
	rdata_node_addvalue($$, $1);
} 
| NA {
	$$ = rdata_node_create(RDN_VALUE);
#ifdef __VC
	volatile double zero = 0.0;
	$$ = rdata_node_create(RDN_VALUE);
	rdata_node_addvalue($$, zero / zero);
#else
	$$ = rdata_node_create(RDN_VALUE);
	rdata_node_addvalue($$, JAGS_NA);
#endif
}
;

/* Rules for parsing generic attributes.  We don't want to do anything
   with the results, just have the parser accept them */

r_generic_attribute: NAME '=' r_generic_vector {;}
;

r_generic_list: r_generic_list_element {;}
| r_generic_list ',' r_generic_list_element {;}
;

r_generic_list_element: r_generic_vector {;}
| NAME '=' r_generic_vector {;}
;

r_generic_vector: r_numeric_vector {;}
| ASINTEGER '(' r_numeric_vector ')' {;}
| r_character_vector {;}
| LIST '(' r_generic_list ')' {;}
| STRUCTURE '(' r_generic_list ')' {;}
| R_NULL {;}
;

r_numeric_vector: DOUBLE {;}
| 'c' '(' r_double_list ')' {;}
;

r_double_list: DOUBLE {;}
| r_double_list ',' DOUBLE {;} 
;

r_character_vector: STRING {;}
| 'c' '(' r_string_list ')' {;}
;

r_string_list: STRING {;}
| r_string_list ',' STRING {;} 
;


%%

/*
int main(int argc, char **argv)
{
#ifdef	YYDEBUG
	extern int rdatadebug;
	rdatadebug = 0;
#endif

	GC_INIT();

	g_rdatanode = NULL;
	if(rdataparse() != 0)
	{
		printf("Err\n");
		return -1;
	}

	printf("ok\n");
	rdata_node_dump(g_rdatanode);

	return 0;
}
*/

int yyerror (const char *s)
{
	printf ( "%s¥n", s);
	return 0;	
}

int rdatawrap()
{
	return 1;
}
