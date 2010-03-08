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

SAMPLER* gamma_sampler_create(STOCHASTIC_NODE* snode)
{
    assert(snode!=NULL);
    GAMMA_SAMPLER* s;
    s = (GAMMA_SAMPLER*)malloc(sizeof(GAMMA_SAMPLER));
    sampler_init(&s->sampler);
    s->sampler.samplertype = S_GAMMA;
    s->sampler.snode = snode;
    s->sampler.stochasticdescendant = nodelist_create();
	stochastic_node_findstochasticdescendant(snode, s->sampler.stochasticdescendant);
	s->coef = NULL;
    return (SAMPLER*)s;
}

int gamma_sampler_cansample(STOCHASTIC_NODE* snode)
{
	assert(snode!=NULL);
	if( snode->name != DGAMMA ) return 0;

    NODELIST* schildren = nodelist_create();
	stochastic_node_findstochasticdescendant(snode, schildren);
    int i, n = schildren->count;
    for( i = 0 ; i < n ; i++ )
    {
        STOCHASTIC_NODE* tmp;
        tmp = (STOCHASTIC_NODE*)schildren->items[i];
        if( tmp->name == DEXP ) continue;
        if( tmp->name == DPOIS ) continue;
        if( tmp->name == DNORM) continue;
        if( tmp->name == DGAMMA ) continue;
		nodelist_free(schildren);
		return 0; 
    }
	nodelist_free(schildren);
    return 1;
}

double gamma_sampler_getscale(STOCHASTIC_NODE* snode)
{
	NODE* target;
	target = (NODE*)snode;
	return node_getvalue(target->parents->items[1]);
}

void gamma_sampler_free(GAMMA_SAMPLER* s)
{
	assert(s!=NULL);
    nodelist_free(s->sampler.stochasticdescendant);
	sampler_destroy(&s->sampler);
	if( s->coef != NULL) free(s->coef);
	free(s);
}

void gamma_sampler_calcoef(GAMMA_SAMPLER* s, double* coef)
{
	assert( s != NULL && coef != NULL );

	STOCHASTIC_NODE* snode;
	NODELIST* schildren;
	int i, n;
	double xold, val;

	snode = s->sampler.snode;
	schildren = s->sampler.stochasticdescendant;
	n = schildren->count;
	xold = stochastic_node_getvalue(snode);

	for( i = 0 ; i < n ; i++ )
	{
		coef[i] = -gamma_sampler_getscale((STOCHASTIC_NODE*)schildren->items[i]);
	}

	val = xold + 1.0;
	stochastic_node_setvalue(snode, val);
	
	for( i = 0 ; i < n ; i++ )
	{
		coef[i] += gamma_sampler_getscale((STOCHASTIC_NODE*)schildren->items[i]);
	}

	stochastic_node_setvalue(snode, xold);
}

void gamma_sampler_update(GAMMA_SAMPLER* s, NMATH_STATE* ms)
{
	STOCHASTIC_NODE *snode, *schild;
	NODELIST* schildren;
	int i, n;
	NODE* target;
	double r, mu;
	double* coef;
	double Y;
	double xnew;
	double ymean;

	assert(s!=NULL);

	if(mode_verbose > 2 ) printf("*dgamma:  ");
	
	snode = s->sampler.snode;
	schildren = s->sampler.stochasticdescendant;
	n = schildren->count;
	target = (NODE*)snode;
	
	r = node_getvalue(target->parents->items[0]);
	mu = node_getvalue(target->parents->items[1]);
	if(mode_verbose>2) printf(" r=%f, mu=%f", r, mu);

	coef = s->coef;
	if( coef == NULL )
	{
		coef = (double*)malloc(sizeof(double)*n);
		gamma_sampler_calcoef(s, coef);
		s->coef = coef;
	}
	if( mode_verbose>2)printf(" nchildren=%d\n", n);

	for( i = 0 ; i < n ; i++ )
	{
		if(mode_verbose>2) printf("  coef[%d] = %f, ", i, coef[i]);
		if( coef[i] > 0.0 )
		{
			schild = (STOCHASTIC_NODE*)schildren->items[i];
			assert( schild != NULL && schild->node.nodetype == N_STOCHASTIC);
			target = (NODE*)schild;
		
			Y = stochastic_node_getvalue(schild);
	
			if( schild->name == DNORM )
			{
				r += 0.5;
				ymean = node_getvalue(target->parents->items[0]);
				mu += coef[i] * (Y - ymean) * (Y - ymean) / 2;
				if(mode_verbose>2) printf(" Y = %f, r = %f, ymean = %f, mu = %f\n", Y, r, ymean, mu);
			}
			else if( schild->name == DGAMMA )
			{	
				r += node_getvalue(target->parents->items[0]);
				mu += coef[i] * Y;
				if(mode_verbose>2) printf(" r = %f, mu = %f\n", r, mu);
			}
			else
			{
				if(mode_verbose)printf("gamma_sampler_update: unknown child dstribution name = %s\n", distribution_tostring(schild->name));
			}
		}
	}

	xnew = rgamma(ms, r, 1/mu);
	stochastic_node_setvalue(snode, xnew);

	if(mode_verbose>2)printf(" post_r=%f, post_mu=%f, xnew=%f\n", r, 1/mu, xnew);
}
