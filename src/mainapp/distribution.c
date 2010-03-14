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

#include "../nmath/nmath.h"
#include "distribution.h"
#include "conf.h"

enum DISTTYPE distribution_find(const char* name)
{
	enum DISTTYPE i;
	
	for( i = 0 ; i < DIST_MAX ; i++ )
	{
		if( strcmp(name, distribution_tostring(i)) == 0 ) return (enum DISTTYPE)i;
	}

	return 0;
}

const char* distribution_tostring(enum DISTTYPE name)
{
	switch(name)
	{
	case DCAT: return "dcat";
	case DSUM: return "dsum";
	case DBERN: return "dbern";
	case DBETA: return "dbeta";
	case DBIN: return "dbin";
	case DCHISQR: return "dchisqr";
	case DDEXP: return "ddexp";
	case DEXP: return "dexp";
	case DGAMMA: return "dgamma";
	case DGENGAMMA: return "dgengamma";
	case DHYPER: return "dhyper";
	case DLNORM: return "dlnorm";
	case DLOGIS: return "dlogis";
	case DNEGIN: return "dnegin";
	case DNORM: return "dnorm";
	case DPAR: return "dpar";
    case DPOIS: return "dpois";
	case DT: return "dt";
	case DUNIF: return "dunif";
	case DWEIB: return "dweib";
	}
	return "[?]";
}

double distribution_density(enum DISTTYPE name, double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms)
{
	switch(name)
	{
	case DBIN:  return dbin_density(x, par, npar, give_log, ms);
	case DGAMMA: return dgamma_density(x, par, npar, give_log, ms);
	case DNORM: return dnorm_density(x, par, npar, give_log, ms);
	case DBERN: return dbern_density(x, par, npar, give_log, ms);
	case DPOIS: return dpois_density(x, par, npar, give_log, ms);
	case DBETA: return dbeta_density(x, par, npar, give_log, ms);
	case DUNIF: return dunif_density(x, par, npar, give_log, ms);
	case DWEIB: return dweib_density(x, par, npar, give_log, ms);
	}
	fprintf(stderr, "distribution_density: unknown distribution name %s\n", distribution_tostring(name));
	exit(99);
}

char* distribution_toenvstring_density(enum DISTTYPE name, char*  x, char** par, unsigned int npar, int give_log)
{
	switch(name)
	{
	case DBIN: return dbin_toenvstring_density(x, par, npar, give_log);
	case DGAMMA: return dgamma_toenvstring_density(x, par, npar, give_log);
	case DNORM: return dnorm_toenvstring_density(x, par, npar, give_log);
	case DBERN: return dbern_toenvstring_density(x, par, npar, give_log);
	case DPOIS: return dpois_toenvstring_density(x, par, npar, give_log);
	case DBETA: return dbeta_toenvstring_density(x, par, npar, give_log);
	case DUNIF: return dunif_toenvstring_density(x, par, npar, give_log);
	case DWEIB: return dweib_toenvstring_density(x, par, npar, give_log);
	}
	fprintf(stderr, "distribution_density: unknown distribution name %s\n", distribution_tostring(name));
	exit(99);
}

double distribution_random(enum DISTTYPE name, double* par, unsigned int npar, NMATH_STATE *ms)
{
	switch(name)
	{
	case DBIN: return dbin_random(par, npar, ms);
	case DGAMMA: return dgamma_random(par, npar, ms);
	case DNORM: return dnorm_random(par, npar, ms);
	case DBERN: return dbern_random(par, npar, ms);
	case DPOIS: return dpois_random(par, npar, ms);
	case DBETA: return dbeta_random(par, npar, ms);
	case DUNIF: return dunif_random(par, npar, ms);
	case DWEIB: return dweib_random(par, npar, ms);
	}
	fprintf(stderr, "distribution_random: unknown distribution name %s\n", distribution_tostring(name));
	exit(99);
}

/**
 * given named distribution is scalar or not ?
 * if the distribution is scalar, distribution_** function calls distribution_scalar*** function
 * \param name is a distribution name in lower case (OK:dsum, NG:DSum)
*/
int distribution_isscalar(enum DISTTYPE name)
{
	if( DIST_SCALAR_MIN <= name && name <= DIST_SCALAR_MAX ) return 1; 
	return 0;	
}

double distribution_loglikelihood(enum DISTTYPE name, double *x, unsigned int length, double *par, unsigned int npar, NMATH_STATE *ms)
{
	if( name == DCAT ) return dcat_loglikelihood(x, length, par, npar);
	if( distribution_isscalar(name) ) return distribution_scalarloglikelihood(name, *x, par, npar, ms);
	else{
		fprintf(stderr, "distribution_loglikelihood: unknown distribution name %s\n", distribution_tostring(name));
		exit(99);
	}
}

char* distribution_toenvstring_loglikelihood(enum DISTTYPE name, char** x, unsigned int length, char** par, unsigned int npar)
{
	if( name == DCAT ) return dcat_toenvstring_loglikelihood(x, length, par, npar);
	if( distribution_isscalar(name) ) return distribution_toenvstring_scalarloglikelihood(name, x[0], par, npar);
	else {
		return "BBBB";
		fprintf(stderr, "distribution_loglikelihood: unknown distribution name %s\n", distribution_tostring(name));
		exit(99);
	}
}

double distribution_scalarloglikelihood(enum DISTTYPE name, double x, double *par, unsigned int npar, NMATH_STATE* ms)
{
	return distribution_density(name, x, par, npar, 1, ms);
}

char* distribution_toenvstring_scalarloglikelihood(enum DISTTYPE name, char* x, char** par, unsigned int npar)
{
	return distribution_toenvstring_density(name, x, par, npar, 1);
}

void distribution_randomsample(enum DISTTYPE name, double *x, unsigned int length, double *par, unsigned int npar, NMATH_STATE *ms)
{
	if( name == DCAT ) 
	{
		dcat_randomsample(x, length, par, npar, ms);
	}
	else if( distribution_isscalar(name) )
	{
		distribution_scalarrandomsample(name, par, npar, ms);
	}
	else
	{
		fprintf(stderr, "distribution_loglikelihood: unknown distribution name %s\n", distribution_tostring(name));
		exit(99);
	}
}

double distribution_scalarrandomsample(enum DISTTYPE name, double *par, unsigned int npar, NMATH_STATE *ms)
{ 
	return distribution_random(name, par, npar, ms);
}

