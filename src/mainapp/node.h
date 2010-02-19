/*
 *  MAGRO: MCMC Another Gibbs Sampler
 *
 *  Copyright (C) 2010 Wataru Uda
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef __NODE_H__
#define __NODE_H__

#define ARRAY_MAX_DIMS	3
#define MAX_PARAM 10

#include "model.h"
#include "distribution.h"
#include "../nmath/nmath.h"

enum NODETYPE {
	N_NODE=1, N_ARRAY, N_CONSTANT, N_RANGE, N_SYMBOL, N_STOCHASTIC, N_FUNCTION
};

#define NP(node)    ((NODE*)node)
#define NODE_RANGE_CHECK(node)	assert(NP(node)->nodetype >= N_NODE && NP(node)->nodetype <= N_FUNCTION) 
//#define NODE_CHECK(node)	NODE_RANGE_CHECK(node);node_check(node);
#define	NODE_CHECK(node)

#define FUNC_MAX 21
#define FUNC_BINARY_MIN 1
#define FUNC_BINARY_MAX 5
enum FUNCTYPE {
	F_LET = FUNC_BINARY_MIN,
	F_ADD, F_SUB, F_MUL, F_DIV = FUNC_BINARY_MAX, 
	F_NEG, F_MEAN, F_SQRT, F_ABS, F_SIN, F_COS,
	F_CLOGLOG, F_ILOGIT, F_LOGIT, F_ROUND, F_POW, F_EXP, F_STEP, F_LOG,
	F_SUM
};

typedef struct _NODE NODE;
typedef struct _NODELIST NODELIST;
typedef struct _NODEDIC NODEDIC;
typedef struct _ARRAY_NODE ARRAY_NODE;
typedef struct _CONSTANT_NODE CONSTANT_NODE;
typedef struct _RANGE_NODE RANGE_NODE;
typedef struct _SYMBOL_NODE SYMBOL_NODE;
typedef struct _MODEL __MODEL;
typedef struct _STOCHASTIC_NODE STOCHASTIC_NODE;
typedef struct _FUNCTION_NODE FUNCTION_NODE;

struct _NODE {
	enum NODETYPE nodetype;
	NODELIST* parents;
	NODELIST* children;
	__MODEL* model;
	int _isobserved;
	int _isinitialized;
	int order;
};

struct _NODELIST {
	int count;
	NODE** items;
};

struct _NODEDIC {
	int count;
	NODE** symbols;
	NODE** items;
};

struct _ARRAY_NODE {
	NODE node;
	char* name;
	int dims[ARRAY_MAX_DIMS];
};

struct _CONSTANT_NODE {
	NODE node;
	char* name;
	double value;
};

struct _RANGE_NODE {
	NODE node;
	double begin;
	double end;
};

struct _SYMBOL_NODE {
	NODE node;
	char* name;
};

struct _STOCHASTIC_NODE {
	NODE node;
	enum DISTTYPE name;
	double value;
	double params[MAX_PARAM];
};

struct _FUNCTION_NODE {
	NODE node;
	//char* name;
	enum FUNCTYPE name;
};

NODE* node_create(__MODEL *m);
void node_init(NODE* node, __MODEL* m);
void node_initialize(NODE* node, NMATH_STATE *ms);
void node_destroy(NODE* node);
void node_free(NODE* node);
char* node_tostring(NODE* node);
char* node_toenvstring(NODE* node);
void node_addparent(NODE* node, NODE* parent);
void node_addchild(NODE* node, NODE* child);
int node_isobserved(NODE* node);
void node_setobserved(NODE* node);
int node_isinitialized(NODE* node);
void node_setinitialized(NODE* node);
double node_getvalue(NODE* node);
void node_setvalue(NODE* node, double value);
int node_checkancestorsobserved(NODE* node);
int node_isdescendant(NODE* node, NODE* target);
int node_isancestor(NODE* node, NODE* target);
int node_getdesendantcount(NODE* node);
int node_getancestorcount(NODE* node);
void node_check(NODE* node);

void nodelist_check(NODELIST* nodelist);
NODELIST* nodelist_create();
void nodelist_add(NODELIST* nodelist, NODE* node);
int nodelist_contains(NODELIST* nodelist, NODE* node);
void nodelist_free(NODELIST* list);

NODEDIC* nodedic_create();
void nodedic_add(NODEDIC* nodelist, NODE* symbol, NODE* node);
int nodedic_contains(NODEDIC* nodelist, NODE* node);
NODE* nodedic_findsymbol(NODEDIC* dic, NODE* node);
NODE* nodedic_findnode_byliteral(NODEDIC* dic, char* literal);
void nodedic_free(NODEDIC* list);

ARRAY_NODE* array_node_create(__MODEL* m);
void array_node_setdimension(ARRAY_NODE* array, int dims[], int ndims);
void array_node_free(ARRAY_NODE* array);
int array_node_getsize(ARRAY_NODE* array);
void array_node_setname(ARRAY_NODE* array, char* name);
void array_node_add(ARRAY_NODE* array, NODE* item);
NODE* array_node_getnode(ARRAY_NODE* array, int* index);
char* array_node_tostring(ARRAY_NODE* array);
char* array_node_toenvstring(ARRAY_NODE* array);

CONSTANT_NODE* constant_node_create(__MODEL* m);
void constant_node_free(CONSTANT_NODE* constant);
void constant_node_setname(CONSTANT_NODE* constant, char* name);
void constant_node_setvalue(CONSTANT_NODE* constant, double value);
double constant_node_getvalue(CONSTANT_NODE* constant);
char* constant_node_tostring(CONSTANT_NODE* constant);
char* constant_node_toenvstring(CONSTANT_NODE* constant);

RANGE_NODE* range_node_create(__MODEL* m, double begin, double end);
void range_node_free(RANGE_NODE* range);
char* range_node_tostring(RANGE_NODE* range);

SYMBOL_NODE* symbol_node_create(__MODEL* m);
void symbol_node_free(SYMBOL_NODE* symbol);
void symbol_node_setname(SYMBOL_NODE* symbol, char* name);
char* symbol_node_tostring(SYMBOL_NODE* symbol);
char* symbol_node_toenvstring(SYMBOL_NODE* symbol);
int symbol_node_compare(SYMBOL_NODE* s1, SYMBOL_NODE* s2);

STOCHASTIC_NODE* stochastic_node_create(__MODEL* m);
void stochastic_node_randomsample(STOCHASTIC_NODE* snode, NMATH_STATE *ms);
void stochastic_node_free(STOCHASTIC_NODE* snode);
void stochastic_node_setname(STOCHASTIC_NODE* snode, enum DISTTYPE name);
double stochastic_node_getvalue(STOCHASTIC_NODE* snode);
void stochastic_node_setvalue(STOCHASTIC_NODE* snode, double value);
char* stochastic_node_tostring(STOCHASTIC_NODE* snode);
char* stochastic_node_toenvstring(STOCHASTIC_NODE* snode);
//SYMBOL_NODE* stochastic_node_getsymbol(STOCHASTIC_NODE* snode);
//NODE* stochastic_node_gettarget(STOCHASTIC_NODE* snode);
NODELIST* stochastic_node_findstochasticdescendant(STOCHASTIC_NODE* snode);
double stochastic_node_logdensity(STOCHASTIC_NODE* snode, NMATH_STATE *ms);
char* stochastic_node_toenvstring_logdensity(STOCHASTIC_NODE* snode);

FUNCTION_NODE* function_node_create(__MODEL* m);
void function_node_free(FUNCTION_NODE* fnode);
double function_node_getvalue(FUNCTION_NODE* fnode);
void function_node_setname(FUNCTION_NODE* fnode, char* name);
const char* function_node_getnamestr(FUNCTION_NODE* fnode);
char* function_node_tostring(FUNCTION_NODE* fnode);
char* function_node_toenvstring(FUNCTION_NODE* fnode);
//SYMBOL_NODE* function_node_getsymbol(FUNCTION_NODE* fnode);
//NODE* function_node_gettarget(FUNCTION_NODE* fnode);

#endif
