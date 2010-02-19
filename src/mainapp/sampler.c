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
#include <string.h>
#include <assert.h>
#include <float.h>
#include "node.h"
#include "sampler.h"
#include <math.h>
#include "../nmath/nmath.h"
#include "conf.h"

#define NSAMPLER 3

SAMPLER* sampler_create(STOCHASTIC_NODE* snode)
{
	enum DISTTYPE name;
	int i;
	
	assert(snode != NULL);
	name = snode->name;

	SAMPLER* (*create[NSAMPLER]) (STOCHASTIC_NODE* snode) = {
		norm_sampler_create,
		gamma_sampler_create,
		real_sampler_create
	};
	int (*cansample[NSAMPLER]) (STOCHASTIC_NODE* snode) = {
		norm_sampler_cansample,
		gamma_sampler_cansample,
		real_sampler_cansample
	};

	for( i = 0 ; i < NSAMPLER ; i++ )
	{
		if( cansample[i](snode) ) {
			return create[i](snode);
		}
	}	
	printf("Error: cannot create a sampler for %s\n", node_tostring((NODE*)snode));
	exit(10);
}

void sampler_init(SAMPLER* s)
{
	assert(s != NULL);
	s->samplertype = S_UNKNOWN;
	s->snode = NULL;
	s->stochasticdescendant = NULL;	
}

void sampler_destroy(SAMPLER* s)
{
	assert(s != NULL);
	nodelist_free(s->stochasticdescendant);
	s->stochasticdescendant = NULL;
}

void sampler_free(SAMPLER* s)
{
	assert(s != NULL);
	sampler_destroy(s);
	switch(s->samplertype)
	{
	case S_NORM:
		norm_sampler_free((NORM_SAMPLER*)s);
		break;
	case S_GAMMA:
		gamma_sampler_free((GAMMA_SAMPLER*)s);
		break;
	case S_REAL:
		real_sampler_free((REAL_SAMPLER*)s);
		break;
	default:
		sampler_destroy(s);
		GC_FREE(s);
	}
}

const char* sampler_gettypestr(SAMPLER* s)
{
	assert(s != NULL);
	switch(s->samplertype)
	{
	case S_NORM:
		return "norm";
		break;
	case S_GAMMA:
		return "gamma";
		break;
	case S_REAL:
		return "real";
		break;
	case S_UNKNOWN:
		return "unknown";
	}
}

void sampler_update(SAMPLER* s, NMATH_STATE* ms)
{
	assert(s != NULL);
	switch(s->samplertype)
	{
	case S_NORM:
		norm_sampler_update((NORM_SAMPLER*)s, ms);
		break;
	case S_GAMMA:
		gamma_sampler_update((GAMMA_SAMPLER*)s, ms);
		break;
	case S_REAL:
		real_sampler_update((REAL_SAMPLER*)s, ms);
		break;
	case S_UNKNOWN:
		printf("sampler_update: can'n update [sampler type = S_UNKOWN]\n");
	}
}

double sampler_logfullconditional(SAMPLER* s, NMATH_STATE *ms)
{
	STOCHASTIC_NODE* snode = s->snode;
	NODE* node;
	double lprior, llike;
	int i,n;
	
	assert(s != NULL);
	snode = s->snode;

	assert(snode != NULL);
	lprior = stochastic_node_logdensity(snode, ms);

	llike = 0.0;
	n = s->stochasticdescendant->count;
	for( i = 0 ; i < n ; i++ )
	{
		node = s->stochasticdescendant->items[i];
		assert(node->nodetype == N_STOCHASTIC);
		llike += stochastic_node_logdensity((STOCHASTIC_NODE*)node, ms);
	}

	double lfc = lprior + llike;
	return lfc;
}

char* sampler_getvarname(SAMPLER* s, MODEL* m)
{
	assert(s != NULL);
	assert(s->snode != NULL);
//	MODEL* m = ((NODE*)s->snode)->model;
	NODE* sym;
	assert(m != NULL);
	sym =nodedic_findsymbol(m->relations, (NODE*)s->snode);
	return node_tostring(sym);
}

SAMPLERLIST* samplerlist_create()
{
	SAMPLERLIST* list;
	list = (SAMPLERLIST*)GC_MALLOC(sizeof(SAMPLERLIST));
	list->size = 10000;
	list->count = 0;
	list->items = (SAMPLER**)GC_MALLOC(sizeof(SAMPLER*)*list->size);
	return list;
}

void samplerlist_free(SAMPLERLIST* list)
{
	GC_FREE(list);
}

void samplerlist_add(SAMPLERLIST* list, SAMPLER* sampler)
{
	if( list->count < list->size )
	{
		list->items[list->count] = sampler;
		list->count = list->count + 1;
	}
	else
	{
		printf("samplerlist_add: buffer overflow\n");
	}
}

