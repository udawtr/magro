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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include "node.h"
#include "model.h"
#include "../nmath/nmath.h"
#include "conf.h"

FUNCTION_NODE* function_node_create(MODEL* m)
{
	FUNCTION_NODE* fnode;
	fnode = (FUNCTION_NODE*)GC_MALLOC(sizeof(FUNCTION_NODE));
	node_init(&fnode->node, m);
	fnode->node.nodetype = N_FUNCTION;
	return fnode;
}

double function_node_getaddvalue(FUNCTION_NODE* fnode)
{
	double left, right;
	left = node_getvalue(fnode->node.parents->items[0]);
	right = node_getvalue(fnode->node.parents->items[1]);
	return left+right;
}

double function_node_getsubvalue(FUNCTION_NODE* fnode)
{
	double left, right;
	left = node_getvalue(fnode->node.parents->items[0]);
	right = node_getvalue(fnode->node.parents->items[1]);
	return left-right;
}

double function_node_getmulvalue(FUNCTION_NODE* fnode)
{
	double left, right;
	left = node_getvalue(fnode->node.parents->items[0]);
	right = node_getvalue(fnode->node.parents->items[1]);
	return left*right;
}

double function_node_getdivvalue(FUNCTION_NODE* fnode)
{
	double left, right;
	left = node_getvalue(fnode->node.parents->items[0]);
	right = node_getvalue(fnode->node.parents->items[1]);
	return left/right;
}

double function_node_getnegvalue(FUNCTION_NODE* fnode)
{
	double left;
	left = node_getvalue(fnode->node.parents->items[0]);
	return -left;
}

double function_node_getmeanvalue(FUNCTION_NODE* fnode)
{
	int i, n;
	double value, total;
	ARRAY_NODE* array;	

	total = 0.0;
	array = (ARRAY_NODE*)fnode->node.parents->items[0];
	n = array->node.parents->count;
	for( i = 0 ; i < n ; i++ )
	{
		value = node_getvalue(array->node.parents->items[i]);
		total += value;
	}

	return total/n;
}

double function_node_getsqrtvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return sqrt(value);
}

double function_node_getpowvalue(FUNCTION_NODE* fnode)
{
	double value, mul;
	value = node_getvalue(fnode->node.parents->items[0]);
	mul   = node_getvalue(fnode->node.parents->items[1]);
	return pow(value, mul);
}

double function_node_getexpvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return exp(value);
}

double function_node_getlogvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return log(value);
}

double function_node_getabsvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return fabs(value);
}

double function_node_getcosvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return cos(value);
}

double function_node_getsinvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return sin(value);
}

double function_node_getcloglogvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return log(-log(1-value));
}

double function_node_geticloglogvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return 1.0 - exp(-exp(value));
}

double function_node_getilogitvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return 1.0 / (1.0+exp(-value));
}

double function_node_getlogitvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return log(value) - log(1.0-value); 
}

double function_node_getroundvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return floor(value+0.5);
}
double function_node_getstepvalue(FUNCTION_NODE* fnode)
{
	double value;
	value = node_getvalue(fnode->node.parents->items[0]);
	return value >= 0 ? 1.0 : 0.0; 
}

double function_node_getsumvalue(FUNCTION_NODE* fnode)
{
	int i, n;
	double value, total;
	ARRAY_NODE* array;	

	total = 0.0;
	array = (ARRAY_NODE*)fnode->node.parents->items[0];
	n = array->node.parents->count;
	for( i = 0 ; i < n ; i++ )
	{
		value = node_getvalue(array->node.parents->items[i]);
		total += value;
	}

	return total;
}

double (*func_table[FUNC_MAX])(FUNCTION_NODE* fnode) = {
	NULL,
	NULL,
	function_node_getaddvalue,
	function_node_getsubvalue,
	function_node_getmulvalue,
	function_node_getdivvalue,
	function_node_getnegvalue,
	function_node_getmeanvalue,
	function_node_getsqrtvalue,
	function_node_getabsvalue,
	function_node_getsinvalue,
	function_node_getcosvalue,
	function_node_getcloglogvalue,
	function_node_getilogitvalue,
	function_node_getlogitvalue,
	function_node_getroundvalue,
	function_node_getpowvalue,
	function_node_getexpvalue,
	function_node_getstepvalue,
	function_node_getlogvalue,
	function_node_getsumvalue
};
	
double function_node_getvalue(FUNCTION_NODE* fnode)
{
	assert( fnode != NULL );

	return func_table[fnode->name](fnode);
}

int function_node_isbinary(FUNCTION_NODE* fnode)
{
	enum FUNCTYPE name;
	assert( fnode != NULL );
	name = fnode->name;
	if( FUNC_BINARY_MIN <= name && name <= FUNC_BINARY_MAX ) return 1;
	return 0;
}

void function_node_free(FUNCTION_NODE* fnode)
{
	assert( fnode != NULL );
	fnode->node.refcount--;
	if( fnode->node.refcount <= 0 )
	{
		node_destroy((NODE*)fnode);
		GC_FREE(fnode);
	}
}

char* function_node_tostring(FUNCTION_NODE* fnode)
{
	const char* namestr = function_node_getnamestr(fnode);
	int nparam = fnode->node.parents->count;
	char** sparams = GC_MALLOC(sizeof(char*) * nparam);
	int i, l, sz, off, isbin;
	char* buf;
	NODE* param;	

	isbin = function_node_isbinary(fnode);

	sz = strlen(namestr);
	for( i = 0 ; i < nparam ; i++ )
	{
		param = fnode->node.parents->items[i];
		sparams[i] = node_tostring(param);
		sz += strlen(sparams[i]);
	}
	sz += nparam + 100;
	
	buf = GC_MALLOC_ATOMIC(sizeof(char) * sz);
	off = 0;
	
	if( isbin )
	{
		sprintf(buf, "(%s)", sparams[0]);
		strcat(buf, namestr);
		if( nparam > 1 )
		{
			strcat(buf, "(");
			strcat(buf, sparams[1]);
			strcat(buf, ")");
		}else{
			strcat(buf, "?");
		}
	}
	else
	{
		sprintf(buf, "%s(", namestr);
		for( i = 0 ; i < nparam ; i++ )
		{
			strcat(buf, sparams[i]);
			if(i < nparam-1) strcat(buf, ",");
		}
		strcat(buf, ")");
	}

	assert(strlen(buf) < sz);
	
	return buf;
}

char* function_node_toenvstring(FUNCTION_NODE* fnode)
{
	int i, nparam;
	int off;
	int isbin;
	char* buf, *tmp;
	const char *namestr;

	namestr = function_node_getnamestr(fnode);
	buf = (char*)GC_MALLOC_ATOMIC(sizeof(char*) * 255);
	nparam = fnode->node.parents->count;
    isbin = function_node_isbinary(fnode);
	if( isbin )
	{
		if( nparam > 1 )
		{
			sprintf(buf, "(%s %s %s)", 
				node_toenvstring(fnode->node.parents->items[0]),
				namestr,
				node_toenvstring(fnode->node.parents->items[1])
			);
		}
		else
		{
			sprintf(buf, "%s %s", 
				node_toenvstring(fnode->node.parents->items[0]),
				namestr
			);
		}
	}
	else if( fnode->name == F_MEAN  )
	{
		ARRAY_NODE* array = (ARRAY_NODE*)fnode->node.parents->items[0];
		sprintf(buf, "env_mean(%s, %d)", 
			array_node_toenvstring(array),
			array_node_getsize(array)
		);
		return buf;
	}
	else if( fnode->name == F_ILOGIT  )
	{
		char* tpl = "1.0 / (1.0+exp(-(%s)))";
		char* val = node_toenvstring(fnode->node.parents->items[0]);
		sprintf(buf, tpl, val);
		return buf;
	}
	else
	{
		sprintf(buf, "env_%s(", namestr);
        for( i = 0 ; i < nparam ; i++ )
        {
			tmp = node_toenvstring(fnode->node.parents->items[i]);
            strcat(buf, tmp);
            if( i < nparam-1 )
				strcat(buf, ",");
        }
	}

	return buf;	
}

void function_node_setname(FUNCTION_NODE* fnode, char* name)
{
	assert( fnode != NULL );

    if( strcmp(name, "+") == 0 ) fnode->name = F_ADD; 
    if( strcmp(name, "-") == 0 ) fnode->name = F_SUB; 
    if( strcmp(name, "*") == 0 ) fnode->name = F_MUL; 
    if( strcmp(name, "/") == 0 ) fnode->name = F_DIV; 
    if( strcmp(name, "NEG") == 0 ) fnode->name = F_NEG; 
    if( strcmp(name, "mean") == 0 ) fnode->name = F_MEAN; 
    if( strcmp(name, "sqrt") == 0 ) fnode->name = F_SQRT; 
    if( strcmp(name, "abs") == 0 ) fnode->name = F_ABS; 
    if( strcmp(name, "sin") == 0 ) fnode->name = F_SIN; 
    if( strcmp(name, "cos") == 0 ) fnode->name = F_COS; 
    if( strcmp(name, "cloglog") == 0 ) fnode->name = F_CLOGLOG; 
    if( strcmp(name, "ilogit") == 0 ) fnode->name = F_ILOGIT; 
    if( strcmp(name, "logit") == 0 ) fnode->name = F_LOGIT; 
    if( strcmp(name, "round") == 0 )fnode->name = F_ROUND; 
    if( strcmp(name, "pow") == 0 ) fnode->name = F_POW; 
    if( strcmp(name, "exp") == 0 ) fnode->name = F_EXP; 
    if( strcmp(name, "step") == 0 ) fnode->name = F_STEP; 
    if( strcmp(name, "log") == 0 ) fnode->name = F_LOG; 
    if( strcmp(name, "sum") == 0 ) fnode->name = F_SUM; 

}

const char* function_node_getnamestr(FUNCTION_NODE* fnode)
{
	assert(fnode != NULL);
	switch(fnode->name) {
	case F_ADD: return "+";
	case F_SUB: return "-";
	case F_MUL: return "*";
	case F_DIV: return "/";
	case F_NEG: return "NEG";
	case F_MEAN: return "mean";
	case F_SQRT: return "sqrt";
	case F_ABS: return "abs";
	case F_SIN: return "sin";
	case F_COS: return "cos";
	case F_CLOGLOG: return "cloglog";
	case F_ILOGIT: return "ilogit";
	case F_LOGIT: return "logit";
	case F_ROUND: return "round";
	case F_POW: return "pow";
	case F_EXP: return "exp";
	case F_STEP: return "step";
	case F_LOG: return "log";
	case F_SUM: return "sum";
	case F_LET: return "=";
	}
	assert(0);
}

/*
SYMBOL_NODE* function_node_getsymbol(FUNCTION_NODE* fnode)
{
	NODE* s;
	assert( fnode != NULL);
	if( strcmp(fnode->name, "<=" ) == 0 )
	{
		s =  fnode->node.parents->items[0];
		assert( s->nodetype == N_SYMBOL );
		return (SYMBOL_NODE*)s;
	}
	return NULL;
}

NODE* function_node_gettarget(FUNCTION_NODE* fnode)
{
	NODE* s;
	assert( fnode != NULL);
	if( strcmp(fnode->name, "<=" ) == 0 )
	{
		s =  fnode->node.parents->items[1];
		return s;
	}
	return NULL;
}
*/
