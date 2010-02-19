/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998 Ross Ihaka
 *  Copyright (C) 2000	    The R Development Core Team
 *  Copyright (C) 2003	    The R Foundation
 *
 *  Merge in to MAGRO: 
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
 *  SYNOPSIS
 *
 *	double dnorm4(double x, double mu, double sigma, int give_log)
 *	      {dnorm (..) is synonymous and preferred inside R}
 *
 *  DESCRIPTION
 *
 *	Compute the density of the normal distribution.
 */

#include <math.h>
#include "nmath.h"

#define give_log log_p
#define R_D__0	(log_p ? -INFINITY: 0.0)		/* 0 */
#define R_D__1	(log_p ? 0.0 : 1.0)			/* 1 */

#ifndef M_LN_SQRT_PI
#define M_LN_SQRT_PI	0.572364942924700087071713675677	/* log(sqrt(pi)) */
#endif

#ifndef M_LN_SQRT_2PI
#define M_LN_SQRT_2PI	0.918938533204672741780329736406	/* log(sqrt(2*pi)) */
#endif

#ifndef M_LN_SQRT_PId2
#define M_LN_SQRT_PId2	0.225791352644727432363097614947	/* log(sqrt(pi/2)) */
#endif

#ifndef M_1_SQRT_2PI
#define M_1_SQRT_2PI	0.398942280401432677939946059934	/* 1/sqrt(2pi) */
#endif

double dnorm4(NMATH_STATE *state, double x, double mu, double sigma, int give_log)
{
    if (isnan(x) || isnan(mu) || isnan(sigma))
	return x + mu + sigma;
    
	if(!isfinite(sigma)) return R_D__0;
    if(!isfinite(x) && mu == x) return NAN;/* x-mu is NaN */
    if (sigma <= 0) {
	if (sigma < 0) return NAN;;
	/* sigma == 0 */
	return (x == mu) ? INFINITY: R_D__0;
    }
    x = (x - mu) / sigma;

    if(!isfinite(x)) return R_D__0;
    return (give_log ?
	    -(M_LN_SQRT_2PI  +	0.5 * x * x + log(sigma)) :
	    M_1_SQRT_2PI * exp(-0.5 * x * x)  /	  sigma);
    /* M_1_SQRT_2PI = 1 / sqrt(2 * pi) */
}


