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
#include "model.h"
#include "node.h"
#include "conf.h"

MODEL* model_create()
{
	MODEL* m;
	m = (MODEL*)GC_MALLOC(sizeof(MODEL));
	m->relations = nodedic_create();
	m->samplers = samplerlist_create();
	return m;
}

void model_free(MODEL* m)
{
	assert(m != NULL && m->relations != NULL && m->samplers != NULL);
	if( m->samplers != NULL )
	{
		samplerlist_free(m->samplers);
		m->samplers = NULL;
	}
	if( m->relations != NULL )
	{
		nodedic_free(m->relations);
		m->relations = NULL;
	}
	GC_FREE(m);
}

void model_addrelation(MODEL* m, NODE* symbol, NODE* rel)
{
	assert( m != NULL );
	assert( rel != NULL );
	assert( rel->nodetype == N_STOCHASTIC || rel->nodetype == N_FUNCTION || rel->nodetype == N_CONSTANT);

	if( symbol->nodetype == N_CONSTANT )
	{
		SYMBOL_NODE* _s = symbol_node_create(m);
		symbol_node_setname(_s, ((CONSTANT_NODE*)symbol)->name);
		symbol = (NODE*)_s;
	}

	assert(symbol->nodetype == N_SYMBOL);
	nodedic_add(m->relations, (SYMBOL_NODE*)symbol, rel);
}

void model_addsampler(MODEL* m, SAMPLER* sampler)
{
	samplerlist_add(m->samplers, sampler);
}


