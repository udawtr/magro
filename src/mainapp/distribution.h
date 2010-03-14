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

#ifndef __DISTRIBUTION_H__
#define __DISTRIBUTION_H__

#include "../nmath/nmath.h"

#define DIST_SCALAR_MIN  2
#define DIST_SCALAR_MAX 20 
#define DIST_MAX  20 

enum DISTTYPE {
	DCAT = 1,
	DSUM = DIST_SCALAR_MIN,
	DBERN,
	DBETA,
	DBIN,

	DCHISQR,
	DDEXP,
	DEXP,
	DGAMMA,
	DGENGAMMA,

	DHYPER,
	DLNORM,
	DLOGIS,
	DNEGIN,
	DNORM,

	DPAR,
	DPOIS,
	DT,
	DUNIF,
	DWEIB
};

/* main functions */
enum DISTTYPE distribution_find(const char* name);
const char* distribution_tostring(enum DISTTYPE name);
double distribution_loglikelihood(enum DISTTYPE name, double *x, unsigned int length, double *par, unsigned int npar, NMATH_STATE *ms);
void distribution_randomsample(enum DISTTYPE name, double *x, unsigned int length, double *par, unsigned int npar, NMATH_STATE *ms);

/* for scalar distribution (private use) */
double distribution_scalarloglikelihood(enum DISTTYPE name, double x, double *par, unsigned int npar, NMATH_STATE *ms);
double distribution_scalarrandomsample(enum DISTTYPE name, double *par, unsigned int npar, NMATH_STATE *ms);
int distribution_isscalar(enum DISTTYPE name);

/* for density and random with nmath library */
double distribution_density(enum DISTTYPE name, double x, double* par, unsigned int npar, int give_log, NMATH_STATE *ms);
double distribution_random(enum DISTTYPE name, double* par, unsigned int npar, NMATH_STATE *ms);

/* density specific density or random function (private use) */
#include "distributions/dbin.h"
#include "distributions/dgamma.h"
#include "distributions/dnorm.h"
#include "distributions/dcat.h"

char* distribution_toenvstring_loglikelihood(enum DISTTYPE name, char** x, unsigned int length, char** par, unsigned int npar);
char* distribution_toenvstring_scalarloglikelihood(enum DISTTYPE name, char* x, char** par, unsigned int npar);
char* distribution_toenvstring_density(enum DISTTYPE name, char*  x, char** par, unsigned int npar, int give_log);
#endif

