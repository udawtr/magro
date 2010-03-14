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
#include "dunif.h"

#define LOWER(par) (par[0])
#define UPPER(par) (par[1])

double dunif_density(double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms)
{
    assert(par!=NULL && npar>=2);
    return dunif(ms, x, LOWER(par), UPPER(par), give_log);
}

char* dunif_toenvstring_density(char* x, char** par, unsigned int npar, int give_log)
{
    char *lower, *upper, *buff;
    assert(par!=NULL && npar>=2);
    lower = par[0];
    upper = par[1];
    buff = malloc(sizeof(char) * (strlen(x) + strlen(lower) + strlen(upper) + 40));
    sprintf(buff, "dunif(state, %s, %s, %s, %d)", x, lower, upper, give_log);
    return buff;
}

double dunif_random(double *par, unsigned int npar, NMATH_STATE *ms)
{
    assert(par!=NULL && npar>=2);
    return runif(ms, LOWER(par), UPPER(par));
}

