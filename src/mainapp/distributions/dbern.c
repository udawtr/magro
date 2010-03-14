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
#include <math.h>
#include <assert.h>

#include "../../nmath/nmath.h"
#include "dbern.h"

#define PROB(par)	par[0]

double dbern_density(double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms)
{
    double d = 0;
    assert(par!=NULL && npar>=1);
    if (x == 1.0)
	{
		d = PROB(par);
    }
	else if (x == 0.0)
	{
    	d = 1.0 - PROB(par);
	}

    if (give_log) {
	    return d == 0.0 ? NEGINF : log(d);
    }
    else {
    	return d;
    }
}

char* dbern_toenvstring_density(char* x, char** par, unsigned int npar, int give_log)
{
    char *prob, *buff;
    assert(par!=NULL && npar>=1);
    prob = par[0];
    buff = malloc(sizeof(char) * (strlen(x)*2 + strlen(prob)*2 + 40));
    if( give_log )
		sprintf(buff, "log((%s) == 1.0 ? (%s) : ((%s) == 0.0 ? 1.0-(%s): 0.0))", x, prob, x, prob);
	else
		sprintf(buff, "((%s) == 1.0 ? (%s) : ((%s) == 0.0 ? 1.0-(%s): 0.0))", x, prob, x, prob);
    return buff;
}

double dbern_random(double *par, unsigned int npar, NMATH_STATE *ms)
{
    assert(par!=NULL && npar>=1);
	return uniform(ms) < PROB(par) ? 1.0 : 0.0;
}

