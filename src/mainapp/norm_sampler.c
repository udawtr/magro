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

SAMPLER* norm_sampler_create(STOCHASTIC_NODE* snode)
{
	NORM_SAMPLER* s;

	assert(snode!=NULL);

	s = (NORM_SAMPLER*)malloc(sizeof(NORM_SAMPLER));
	sampler_init(&s->sampler);
	s->sampler.samplertype = S_NORM;
	s->sampler.snode = snode;
	s->sampler.stochasticdescendant = nodelist_create();
	stochastic_node_findstochasticdescendant(snode, s->sampler.stochasticdescendant);
	s->beta = NULL;

	return (SAMPLER*)s;
}

int norm_sampler_cansample(STOCHASTIC_NODE* snode)
{
	NODELIST* schildren;
	int i, n;

	assert(snode != NULL);
	if( snode->name != DNORM ) return 0;

	schildren = nodelist_create();
	stochastic_node_findstochasticdescendant(snode, schildren);
	n = schildren->count;
	for( i = 0 ; i < n ; i++ )
	{
		STOCHASTIC_NODE* tmp;
		tmp = (STOCHASTIC_NODE*)schildren->items[i];
		if( tmp->name != DNORM )
		{
			nodelist_free(schildren);
			return 0;
		}
	}
	nodelist_free(schildren);
	return 1;
}

void norm_sampler_free(NORM_SAMPLER* s)
{
	assert(s!=NULL);
	nodelist_free(s->sampler.stochasticdescendant);
	sampler_destroy(&s->sampler);
	if( s->beta != NULL) free(s->beta);
	free(s);
}

void norm_sampler_calbeta(NORM_SAMPLER* s, double* beta)
{
	STOCHASTIC_NODE* snode = s->sampler.snode;
	NODE* target;
	NODELIST* schildren = s->sampler.stochasticdescendant;
	double xold,xnew,mu;
	int i, n;

	assert(s != NULL && beta != NULL);

	xold  = stochastic_node_getvalue(snode);
	xnew = xold + 1.0;
	stochastic_node_setvalue(snode, xnew);

	n = schildren->count;
	for( i = 0 ; i < n ; i++ )
	{
		target = schildren->items[i];
		mu = node_getvalue(target->parents->items[0]);
//		if( mode_verbose>2) printf("  mu: [%s] => %f\n", node_tostring(target->parents->items[0]), mu);
		beta[i] = mu;
	}

	stochastic_node_setvalue(snode, xold);
	for( i = 0 ; i < n ; i++ )
	{
		target = schildren->items[i];
		mu = node_getvalue(target->parents->items[0]);
		if( mode_verbose>2) printf("  mu: [%s] => %f\n", node_tostring(target->parents->items[0]), mu);
		beta[i] -= mu;
	}
}

void norm_sampler_update(NORM_SAMPLER* s, NMATH_STATE* ms)
{
	STOCHASTIC_NODE* snode;
	STOCHASTIC_NODE* schild;
	NODE* target;
	NODELIST* schildren;
	int i, n;
	double xold, xnew, priormean, priorprec, postmean, postsd;
	double A, B, Y, tau, alpha;
	double* beta;
	double tau_beta;

	assert(s != NULL);

	if(mode_verbose>2)printf("*dnorm:  ");
		
	snode = s->sampler.snode;
	schildren = s->sampler.stochasticdescendant;
	n = schildren->count;
	target = (NODE*)snode;
	
	xold = stochastic_node_getvalue(snode);
	priormean = node_getvalue(target->parents->items[0]) - xold;
	priorprec = node_getvalue(target->parents->items[1]);

	A = priormean * priorprec;
	B = priorprec;
	if(mode_verbose>2)printf(" xold=%f, priormean=%f, priorprec=%f", xold, priormean, priorprec);
	
	beta = s->beta;
	if( beta == NULL )
	{
		beta = (double*)malloc(sizeof(double) * n);
		norm_sampler_calbeta(s, beta);
	}
	if(mode_verbose>2)printf(" nchildren=%d\n", n);
	for( i = 0 ; i < n ; i++ )
	{
		schild = (STOCHASTIC_NODE*)schildren->items[i];
		target = (NODE*)schild;
		Y = stochastic_node_getvalue(schild);
		tau = node_getvalue(target->parents->items[1]);
		alpha = node_getvalue(target->parents->items[0]);
		if(mode_verbose>2)printf("  Y=%f, tau=%f, alpha=%f, beta=%f\n", Y, tau, alpha, beta[i]);
		tau_beta = tau * beta[i];
		A += (Y - alpha) * tau_beta;
		B +=  beta[i] * tau_beta;
	}
	if( s->beta == NULL ) s->beta = beta;

	postmean = xold + A/B;
	postsd = sqrt(1/B);
	xnew = rnorm(ms, postmean, postsd);

	stochastic_node_setvalue(snode, xnew);

	if(mode_verbose>2)printf(" postmean=%f, postsd=%f, xnew=%f\n", postmean, postsd, xnew);
}

