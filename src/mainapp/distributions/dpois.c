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
#include "dpois.h"

#define LAMBDA(par)	(par[0])

double dpois_density(double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms)
{
    assert(par!=NULL && npar>=1);
    return dpois(ms, x, LAMBDA(par), give_log);
}

char* dpois_toenvstring_density(char* x, char** par, unsigned int npar, int give_log)
{
    char *lambda, *buff;
    assert(par!=NULL && npar>=1);
    lambda = LAMBDA(par);
    buff = malloc(sizeof(char) * (strlen(x) + strlen(lambda) + 40));
    sprintf(buff, "dpois(state, %s, %s, %d)", x, lambda, give_log);
    return buff;
}

double dpois_random(double *par, unsigned int npar, NMATH_STATE *ms)
{
    assert(par!=NULL && npar>=1);
    return rpois(ms, LAMBDA(par));
}

