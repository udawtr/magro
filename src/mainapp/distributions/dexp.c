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
#include "dexp.h"

#define SCALE(par)	(1.0/par[0])

double dexp_density(double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms)
{
    assert(par!=NULL && npar>=1);
    return dexp(ms, x, SCALE(par), give_log);
}

char* dexp_toenvstring_density(char* x, char** par, unsigned int npar, int give_log)
{
    char *scale, *buff;
    assert(par!=NULL && npar>=1);
    scale = par[0];
    buff = malloc(sizeof(char) * (strlen(x) + strlen(scale) + 40));
    sprintf(buff, "dexp(state, %s, 1.0/%s, %d)", x, scale, give_log);
    return buff;
}

double dexp_random(double *par, unsigned int npar, NMATH_STATE *ms)
{
    assert(par!=NULL && npar>=1);
    return rexp(ms, SCALE(par));
}

