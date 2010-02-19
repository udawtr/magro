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

#define MIN_ADAPT 	50

SAMPLER* real_sampler_create(STOCHASTIC_NODE* snode)
{
    assert(snode!=NULL);
    REAL_SAMPLER* s;
    s = (REAL_SAMPLER*)malloc(sizeof(REAL_SAMPLER));
    sampler_init(&s->sampler);
    s->sampler.samplertype = S_REAL;
    s->sampler.snode = snode;
    s->sampler.stochasticdescendant = stochastic_node_findstochasticdescendant(snode);
	s->adapt = 1;
	s->sumdiff = 0.0;
	s->iter = 0;
	s->width = 1.0;
	s->max = 10;
    return (SAMPLER*)s;
}

int real_sampler_cansample(STOCHASTIC_NODE* snode)
{
    assert(snode != NULL);
    return 1;
}

void real_sampler_free(REAL_SAMPLER* s)
{
	assert(s!=NULL);
	sampler_destroy(&s->sampler);
	free(s);
}

void real_sampler_update(REAL_SAMPLER* s, NMATH_STATE* ms)
{
	assert(s!=NULL);

	STOCHASTIC_NODE *snode = s->sampler.snode;
	double g0 = sampler_logfullconditional((SAMPLER*)s, ms);
	double z = g0 - exponential(ms);

	double xold = stochastic_node_getvalue(snode); 
	double L = xold - uniform(ms) * s->width;
	double R = L + s->width;

	if( mode_verbose > 2 )
    {
		printf("*reals: g0=%f, z=%f, xold=%f, L=%f, R=%f\n", g0, z, xold, L, R);
	}

	int j = (int)(uniform(ms) * s->max);
	int k = s->max - 1 - j;

	stochastic_node_setvalue(snode, L);
	while(j-- > 0 && sampler_logfullconditional((SAMPLER*)s, ms) > z) {
		L -= s->width;
		stochastic_node_setvalue(snode, L);
	}

	stochastic_node_setvalue(snode, R);
	while( k-- > 0 && sampler_logfullconditional((SAMPLER*)s, ms) > z) {
		R += s->width;
		stochastic_node_setvalue(snode, R);
	}

	// Keep sampling from the interval until acceptance ( the loop is guaranteed to terminate.)
	double xnew;
	for(;;){
		xnew = L + uniform(ms) * (R-L);
		stochastic_node_setvalue(snode, xnew);
		double g = sampler_logfullconditional((SAMPLER*)s, ms);
		if( mode_verbose > 2 )
		{
        	printf("      L=%f, R=%f, g=%f, xnew=%f\n",  L, R, g, xnew);
		}
		if( g >= z - DBL_EPSILON ) {
			//accept point
			break;
		}else{
			// shrink the interval
			if( xnew < xold ) {
				L = xnew;
			}else{
				R = xnew;
			}
		}
	}

/*
	if( s->adapt ) 
	{
		s->sumdiff += s->iter * fabs(xnew - xold);
		s->iter++;
		if( s->iter > MIN_ADAPT ) {
			s->width = 2.0 * s->sumdiff / s->iter / (s->iter-1);
		}
	}
*/
}
