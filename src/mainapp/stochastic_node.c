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
#include "distribution.h"

#ifndef GC_MALLOC
#define GC_MALLOC malloc
#define GC_MALLOC_ATOMIC malloc
#define GC_FREE free
#endif

STOCHASTIC_NODE* stochastic_node_create(MODEL* m)
{
	STOCHASTIC_NODE* snode;

	assert(m!=NULL);
	if( m != NULL )
	{
		snode = (STOCHASTIC_NODE*)GC_MALLOC(sizeof(STOCHASTIC_NODE));
		if( snode != NULL )
		{
			node_init(&snode->node, m);
			snode->node.nodetype = N_STOCHASTIC;
			snode->value = 0.0;
			return snode;
		}
	}
	return NULL;
}

void stochastic_node_free(STOCHASTIC_NODE* snode)
{
	assert( snode != NULL);
	if( snode != NULL )
	{
		snode->node.refcount--;
		if( snode->node.refcount <= 0 )
		{
			node_destroy((NODE*)snode);
			free(snode);
		}
	}
}

double stochastic_node_getvalue(STOCHASTIC_NODE* snode)
{
	return snode->value;
}

void stochastic_node_setvalue(STOCHASTIC_NODE* snode, double value)
{
	snode->value = value;
}

char* stochastic_node_toenvstring_logdensity(STOCHASTIC_NODE* snode)
{
    char **p, **param, *s;
    int i,n;

    assert(snode != NULL);
    n = snode->node.parents->count;
	p = GC_MALLOC(sizeof(char*)*n);
    p[0] = node_toenvstring((NODE*)snode);
    assert(n < MAX_PARAM);


	param = GC_MALLOC(sizeof(char*)*n);
    for( i = 0 ; i < n ; i++ )
    {
        param[i] = node_toenvstring(snode->node.parents->items[i]);
    }
   
    s =distribution_toenvstring_loglikelihood(snode->name, p, 1, param, (unsigned int)n);

	return s;
}

double stochastic_node_logdensity(STOCHASTIC_NODE* snode, NMATH_STATE *ms)
{
	double *p, *param;
	int i,n;

	assert(snode != NULL);
	p = &(snode->value);
	n = snode->node.parents->count;
	assert(n < MAX_PARAM);

	param = snode->params;	
	for( i = 0 ; i < n ; i++ )
	{
		*(param++) = node_getvalue(snode->node.parents->items[i]);
	}
	
	return distribution_loglikelihood(snode->name, p, 1, snode->params, (unsigned int)n, ms);
}

void stochastic_node_randomsample(STOCHASTIC_NODE* snode, NMATH_STATE *ms)
{
/*
	double mu, sigma, r;
	char* namestr;

	assert(snode != NULL);

	if( snode->name == DNORM )
	{
		mu = node_getvalue(snode->node.parents->items[0]);
		sigma =1.0 / sqrt(node_getvalue(snode->node.parents->items[1]));
		r = rnorm(mu, sigma);
		printf("stochastic_node_randomsample: mu=%f, sigma=%f => %f\n", mu, sigma, r);
		snode->value = r;
	}
	else 
	{
		namestr = distribution_tostring(snode->name);
		printf("stochastic_node_randomsample: unkonown distribution name = %s\n", namestr);
		exit(22);
	}
*/

	double par[10];
	int i, n;
	n = snode->node.parents->count;
	for( i = 0 ; i < n ; i++ )
	{
		par[i] = node_getvalue(snode->node.parents->items[i]);
	}
	distribution_randomsample(snode->name, &snode->value, 1, par, n, ms); 
}


char* stochastic_node_tostring(STOCHASTIC_NODE* snode)
{
    int nparam = snode->node.parents->count;
    char** sparams = GC_MALLOC(sizeof(char*) * nparam);
	int i;
	unsigned int sz, off;
    char* buf;
    NODE* param;
	const char* namestr;

	assert(snode != NULL);
	namestr = distribution_tostring(snode->name);

    sz = strlen(namestr); 
    for( i = 0 ; i < nparam ; i++ )
    {
        param = snode->node.parents->items[i];
        sparams[i] = node_tostring(param);
        sz += strlen(sparams[i]);
    }
    sz += nparam + 100;

    buf = GC_MALLOC_ATOMIC(sizeof(char) * sz);
    off = 0;

	sprintf(buf, "%s(", namestr);
	for( i = 0 ; i < nparam ; i++ )
	{	
		strcat(buf, sparams[i]);
		if( i < nparam-1 )
			strcat(buf, ",");
	}
	strcat(buf, ")");

	assert(strlen(buf) < sz);

    return buf;
}

char* stochastic_node_toenvstring(STOCHASTIC_NODE* snode)
{
	NODE* sym;
	
	assert(snode!=NULL);
    assert(snode->node.model != NULL);
    
    sym = nodedic_findsymbol(snode->node.model->relations, (NODE*)snode);
    assert(sym != NULL);
    
    return node_toenvstring(sym);
}

void stochastic_node_setname(STOCHASTIC_NODE* snode, enum DISTTYPE name)
{
	assert( snode != NULL );
	snode->name = name;
}

/*
SYMBOL_NODE* stochastic_node_getsymbol(STOCHASTIC_NODE* snode)
{
	assert( snode != NULL);
	NODE* s = snode->node.parents->items[0];
	if( s->nodetype == N_SYMBOL )
	{
		return (SYMBOL_NODE*)s;
	}
	return NULL;
}

NODE* stochastic_node_gettarget(STOCHASTIC_NODE* snode)
{
	assert( snode != NULL);
	NODE* s = snode->node.parents->items[1];
	return s;
}
*/

void __stochastic_node_findstochasticdescendant(NODE* node, NODELIST* list, NODE* ignore)
{
	int i, n;
	NODE* snode;

	assert( node != NULL && list != NULL );

	n = node->children->count;

	for( i = 0 ; i < n ; i ++ )
	{
		snode = node->children->items[i];
		if( snode->nodetype == N_STOCHASTIC && snode != ignore)
		{
			nodelist_add(list, snode);
		}
		else
		{
			__stochastic_node_findstochasticdescendant(snode, list, ignore);
		}
	}
}

void stochastic_node_findstochasticdescendant(STOCHASTIC_NODE* snode, NODELIST* list)
{
	assert( snode != NULL && list != NULL );
	__stochastic_node_findstochasticdescendant((NODE*)snode, list, (NODE*)snode);
}

