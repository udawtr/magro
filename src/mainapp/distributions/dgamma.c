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
#include "dgamma.h"

double dgamma_density(double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms)
{
	double shape, scale;
	assert(par!=NULL && npar>=2);
	shape = par[0];
	scale = 1.0 / par[1];
	return dgamma(ms, x, shape, scale, give_log);
}

char* dgamma_toenvstring_density(char* x, char** par, unsigned int npar, int give_log)
{
	char *shape, *_scale, *buff;
	assert(par!=NULL && npar>=2);
	shape= par[0];
	_scale = par[1];
	buff = malloc(sizeof(char) * (strlen(x) + strlen(shape) + strlen(_scale) + 30));
	sprintf(buff, "dgamma(state, %s, %s, 1.0/(%s), %d)", x, shape, _scale, give_log);
	return buff;
}

double dgamma_random(double *par, unsigned int npar, NMATH_STATE *ms)
{
	double shape, scale;
	assert(par!=NULL && npar>=2);
	shape = par[0];
	scale = 1.0 / par[1];
	return rgamma(ms, shape, scale);
}

