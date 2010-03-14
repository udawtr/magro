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
#include "dcat.h"

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
	unsigned int i,l, sumpl;
	unsigned int y=0;
	char *sump, *s;

	sscanf(x[0], "%u", &y);	
	assert(!(y < 1 || y > npar ));

	sumpl = 0;
	for( i = 0 ; i < npar ; i++ ) sumpl += strlen(par[i]);
	sump = malloc(sizeof(char)*(sumpl+npar*3));
	sump[0] = '\0';
	for( i = 0 ; i < npar-1 ; i++ ){ strcat(sump,"("); strcat(sump, par[i]); strcat(sump,")+");}
	strcat(sump,"(");
	strcat(sump,par[i]);
	strcat(sump,")");
	
	l = strlen(par[y-1]+sumpl+20);
	s = malloc(sizeof(char)*l);
	sprintf(s, "log(%s)-log(%s)", par[y-1], sump);	

	free(sump);
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

