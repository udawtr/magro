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
	if( name == DBIN ) return dbin_density(x, par, npar, give_log, ms);
	if( name == DGAMMA ) return dgamma_density(x, par, npar, give_log, ms);
	if( name == DNORM ) return dnorm_density(x, par, npar, give_log, ms);
	fprintf(stderr, "distribution_density: unknown distribution name %s\n", distribution_tostring(name));
	exit(99);
}

char* distribution_toenvstring_density(enum DISTTYPE name, char*  x, char** par, unsigned int npar, int give_log)
{
	if( name == DBIN ) return dbin_toenvstring_density(x, par, npar, give_log);
	if( name == DGAMMA ) return dgamma_toenvstring_density(x, par, npar, give_log);
	if( name == DNORM ) return dnorm_toenvstring_density(x, par, npar, give_log);
	fprintf(stderr, "distribution_density: unknown distribution name %s\n", distribution_tostring(name));
	exit(99);
}

double distribution_random(enum DISTTYPE name, double* par, unsigned int npar, NMATH_STATE *ms)
{
	if( name == DBIN ) return dbin_random(par, npar, ms);
	if( name == DGAMMA ) return dgamma_random(par, npar, ms);
	if( name == DNORM ) return dnorm_random(par, npar, ms);
	fprintf(stderr, "distribution_random: unknown distribution name %s\n", distribution_tostring(name));
	exit(99);
}

double dbin_density(double x, double* par, unsigned int npar, int give_log, NMATH_STATE* ms)
{
	assert(par!=NULL && npar>=2);
	double size = par[1];
	double prob = par[0];
	return dbinom(ms, x, size, prob, give_log);
}

char* dbin_toenvstring_density(char* x, char** par, unsigned int npar, int give_log)
{
	assert(par!=NULL && npar>=2);
	char* size = par[1];
	char* prob = par[0];
	char* buff = GC_MALLOC_ATOMIC(sizeof(char) * (strlen(x) + strlen(size) + strlen(prob) + 20));
	sprintf(buff, "dbinom(%s, %s, %s, %d)", x, size, prob, give_log);
	return buff;
}

double dbin_random(double* par, unsigned int npar, NMATH_STATE *ms)
{
	assert(par!=NULL && npar>=2);
	double size = par[1];
	double prob = par[0];
	return rbinom(ms, size, prob);
}

double dgamma_density(double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms)
{
	assert(par!=NULL && npar>=2);
	double shape = par[0];
	double scale = 1.0 / par[1];
	return dgamma(ms, x, shape, scale, give_log);
}

char* dgamma_toenvstring_density(char* x, char** par, unsigned int npar, int give_log)
{
	assert(par!=NULL && npar>=2);
	char* shape= par[0];
	char* _scale = par[1];
	char* buff = GC_MALLOC_ATOMIC(sizeof(char) * (strlen(x) + strlen(shape) + strlen(_scale) + 30));
	sprintf(buff, "dgamma(%s, %s, 1.0/(%s), %d)", x, shape, _scale, give_log);
	return buff;
}

double dgamma_random(double *par, unsigned int npar, NMATH_STATE *ms)
{
	assert(par!=NULL && npar>=2);
	double shape = par[0];
	double scale = 1.0 / par[1];
	return rgamma(ms, shape, scale);
}

double dnorm_density(double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms)
{
	assert(par!=NULL && npar>=2);
	double mu = par[0];
	double sigma  = 1.0 / sqrt(par[1]);
	return dnorm(ms, x, mu, sigma, give_log);
}

char* dnorm_toenvstring_density(char* x, char** par, unsigned int npar, int give_log)
{
	assert(par!=NULL && npar>=2);
	char* mu= par[0];
	char* _sigma= par[1];
	char* buff = GC_MALLOC_ATOMIC(sizeof(char) * (strlen(x) + strlen(mu) + strlen(_sigma) + 40));
	sprintf(buff, "dnorm(%s, %s, 1.0/sqrt(%s), %d)", x, mu, _sigma, give_log);
	return buff;
}

double dnorm_random(double *par, unsigned int npar, NMATH_STATE *ms)
{
	assert(par!=NULL && npar>=2);
	double mu = par[0];
	double sigma = 1.0 / sqrt(par[1]);
	return rnorm(ms, mu, sigma);
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

double dcat_loglikelihood(double *x, unsigned int length, double* par, unsigned int npar)
{
	unsigned int i;
	unsigned int y;
	double sump = 0.0;
	
	assert(length==1);

	y = (unsigned int)x[0];
	assert(!( y < 1 || y > npar ));

	for( i = 0 ; i < npar ; i++ ) {
		double prob = par[i];
		sump += prob;
	}
	return log(par[y-1]) - log(sump);
}

char* dcat_toenvstring_loglikelihood(char** x, unsigned int length, char** par, unsigned int npar)
{
	unsigned int i;
	unsigned int y=0;

	sscanf(x[0], "%u", &y);	
	assert(!(y < 1 || y > npar ));

	int sumpl = 0;
	for( i = 0 ; i < npar ; i++ ) sumpl += strlen(par[i]);
	char* sump = GC_MALLOC_ATOMIC(sizeof(char)*(sumpl+npar*3));
	sump[0] = '\0';
	for( i = 0 ; i < npar-1 ; i++ ){ strcat(sump,"("); strcat(sump, par[i]); strcat(sump,")+");}
	strcat(sump,"(");
	strcat(sump,par[i]);
	strcat(sump,")");
	
	int l = strlen(par[y-1]+sumpl+20);
	char* s = GC_MALLOC_ATOMIC(sizeof(char)*l);
	sprintf(s, "log(%s)-log(%s)", par[y-1], sump);	

	GC_FREE(sump);
	return s;
}

void dcat_randomsample(double *x, unsigned int length, double* par, unsigned int npar, NMATH_STATE *ms)
{
	double sump = 0.0;
	unsigned int i = 0;
	double p, prob;

	for( i = 0 ; i < npar ; i++ ) {
		prob = par[i];
		sump += prob;
	}
	p = sump * norm_rand(ms);
	
	for( i = npar-1 ; i > 0 ; i-- ) {
		prob = par[i];
		sump -= prob;
		if( sump <= p ) break;
	}
	x[0] = (double)i;
}

