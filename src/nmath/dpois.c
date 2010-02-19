/*
 *  AUTHOR
 *    Catherine Loader, catherine@research.bell-labs.com.
 *    October 23, 2000.
 *
 *  Merge in to R:
 *	Copyright (C) 2000, The R Core Development Team
 *
 *  Merge in to MAGRO:
 *  Copyright (C) 2010, Wataru Uda
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
 *
 * DESCRIPTION
 *
 *    dpois() checks argument validity and calls dpois_raw().
 *
 *    dpois_raw() computes the Poisson probability  lb^x exp(-lb) / x!.
 *      This does not check that x is an integer, since dgamma() may
 *      call this with a fractional x argument. Any necessary argument
 *      checks should be done in the calling function.
 *
 */

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "nmath.h"

#define give_log log_p
#define R_D__0  (log_p ? -INFINITY : 0.0)
#define R_D__1	(log_p ? 0. : 1.)			/* 1 */
#define R_D_exp(x)	(log_p	?  (x)	 : exp(x))	/* exp(x) */
#define R_D_fexp(f,x)     (give_log ? -0.5*log(f)+(x) : exp(x)/sqrt(f))
#define R_D_forceint(x)   floor((x) + 0.5)
#define R_D_nonint(x) 	  (fabs((x) - floor((x)+0.5)) > 1e-7)
#define R_D_nonint_check(x) 				\
   if(R_D_nonint(x)) {					\
	printf("non-integer x = %f", x);	\
	return R_D__0;					\
   }

double dpois_raw(NMATH_STATE *state, double x, double lambda, int give_log)
{
    /*       x >= 0 ; integer for dpois(), but not e.g. for pgamma()!
        lambda >= 0
    */
    if (lambda == 0) return( (x == 0) ? R_D__1 : R_D__0 );
    if (!isfinite(lambda)) return R_D__0;
    if (x < 0) return( R_D__0 );
    if (x <= lambda * DBL_MIN) return(R_D_exp(-lambda) );
    if (lambda < x * DBL_MIN) return(R_D_exp(-lambda + x*log(lambda) -lgammafn(state, x+1)));
    return(R_D_fexp( M_PI*2.0*x, -stirlerr(state,x)-bd0(x,lambda) ));
}

double dpois(NMATH_STATE *state, double x, double lambda, int give_log)
{
    if(isnan(x) || isnan(lambda))
        return x + lambda;

    if (lambda < 0) return NAN;
    R_D_nonint_check(x);
    if (x < 0 || !isfinite(x))
	return R_D__0;

    x = R_D_forceint(x);

    return( dpois_raw(state, x,lambda,give_log) );
}

