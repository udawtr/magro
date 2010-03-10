/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998 Ross Ihaka
 *  Copyright (C) 2000-2002 The R Development Core Team
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
 *	#include <Rmath.h>
 *	double rbinom(double nin, double pp)
 *
 *  DESCRIPTION
 *
 *	Random variates from the binomial distribution.
 *
 *  REFERENCE
 *
 *	Kachitvichyanukul, V. and Schmeiser, B. W. (1988).
 *	Binomial random variate generation.
 *	Communications of the ACM 31, 216-222.
 *	(Algorithm BTPEC).
 */

#include "nmath.h"
#include <stdlib.h>

#define MT(p)	state->rbinom.p

static double fmin2(double x, double y) {
        return (x < y) ? x : y;
}

double rbinom(NMATH_STATE *state, double nin, double pp)
{
    /* FIXME: These should become THREAD_specific globals : */

    double f, f1, f2, u, v, w, w2, x, x1, x2, z, z2;
    double p, q, np, g, r, al, alv, amaxp, ffm, ynorm;
    int i,ix,k, n;

    if (!isfinite(nin)) return NAN;
    n = floor(nin + 0.5);
    if (n != nin) return NAN;

    if (!isfinite(pp) ||
	/* n=0, p=0, p=1 are not errors <TSL>*/
	n < 0 || pp < 0. || pp > 1.)	return NAN;;

    if (n == 0 || pp == 0.) return 0;
    if (pp == 1.) return n;

    p = fmin2(pp, 1. - pp);
    q = 1. - p;
    np = n * p;
    r = p / q;
    g = r * (n + 1);

    /* Setup, perform only when parameters change [using static (globals): */

    /* FIXING: Want this thread safe
       -- use as little (thread globals) as possible
    */
    if (pp != MT(psave) || n != MT(nsave))
	{
		MT(psave) = pp;
		MT(nsave) = n;
	
		if (np < 30.0)
		{
	    	/* inverse cdf logic for mean less than 30 */
	    	MT(qn) = pow(q, (double) n);
	    	goto L_np_small;
		}
		else
		{
	    	ffm = np + p;
	    	MT(m) = ffm;
	    	MT(fm) = MT(m);
	    	MT(npq) = np * q;
	    	MT(p1) = (int)(2.195 * sqrt(MT(npq)) - 4.6 * q) + 0.5;
	    	MT(xm) = MT(fm) + 0.5;
	    	MT(xl) = MT(xm) - MT(p1);
	    	MT(xr) = MT(xm) + MT(p1);
	    	MT(c) = 0.134 + 20.5 / (15.3 + MT(fm));
	    	al = (ffm - MT(xl)) / (ffm - MT(xl) * p);
	    	MT(xll) = al * (1.0 + 0.5 * al);
	    	al = (MT(xr) - ffm) / (MT(xr) * q);
	    	MT(xlr) = al * (1.0 + 0.5 * al);
	    	MT(p2) = MT(p1) * (1.0 + MT(c) + MT(c));
	    	MT(p3) = MT(p2) + MT(c) / MT(xll);
	    	MT(p4) = MT(p3) + MT(c) / MT(xlr);
		}
    }
	else if (n == MT(nsave))
	{
		if (np < 30.0)
	    	goto L_np_small;
    }

    /*-------------------------- np = n*p >= 30 : ------------------- */
    while(1)
	{
		u = unif_rand(state) * MT(p4);
    	v = unif_rand(state);
      	
		/* triangular region */
      	if (u <= MT(p1))
		{
	  		ix = MT(xm) - MT(p1) * v + u;
	  		goto finis;
      	}
	
      	/* parallelogram region */
      	if (u <= MT(p2))
		{
	  		x = MT(xl) + (u - MT(p1)) / MT(c);
	  		v = v * MT(c) + 1.0 - fabs(MT(xm) - x) / MT(p1);
	  		if (v > 1.0 || v <= 0.)
	    		continue;
	  		ix = x;
      	}
		else
		{
	  		if (u > MT(p3))
			{
				/* right tail */
	      		ix = MT(xr) - log(v) / MT(xlr);
	      		if (ix > n)
		  			continue;
	      		v = v * (u - MT(p3)) * MT(xlr);
	  		}
			else
			{
				/* left tail */
	      		ix = MT(xl) + log(v) / MT(xll);
	      		if (ix < 0)
		  			continue;
	      		v = v * (u - MT(p2)) * MT(xll);
	  		}
      	}
	
      	/* determine appropriate way to perform accept/reject test */
      	k = abs(ix - MT(m));
      	if (k <= 20 || k >= MT(npq) / 2 - 1)
		{
	 		/* explicit evaluation */
	  		f = 1.0;
	  		if (MT(m) < ix)
			{
	      		for (i = MT(m) + 1; i <= ix; i++)
		 			f *= (g / i - r);
	  		}
			else if (MT(m) != ix)
			{
	      		for (i = ix + 1; i <= MT(m); i++)
		  			f /= (g / i - r);
	  		}
	  		if (v <= f)
	    		goto finis;
      	}
		else
		{
	  		/* squeezing using upper and lower bounds on log(f(x)) */
	  		amaxp = (k / MT(npq)) * ((k * (k / 3. + 0.625) + 0.1666666666666) / MT(npq) + 0.5);
	  		ynorm = -k * k / (2.0 * MT(npq));
	  		alv = log(v);
	  		if (alv < ynorm - amaxp)
	      		goto finis;
	  		if (alv <= ynorm + amaxp)
			{
	      		/* stirling's formula to machine accuracy */
	      		/* for the final acceptance/rejection test */
	      		x1 = ix + 1;
	      		f1 = MT(fm) + 1.0;
	      		z = n + 1 - MT(fm);
	      		w = n - ix + 1.0;
	      		z2 = z * z;
	      		x2 = x1 * x1;
	      		f2 = f1 * f1;
	      		w2 = w * w;
	      		if (alv <= MT(xm) * log(f1 / x1) + (n - MT(m) + 0.5) * log(z / w) + (ix - MT(m)) * log(w * p / (x1 * q)) + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / f2) / f2) / f2) / f2) / f1 / 166320.0 + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / z2) / z2) / z2) / z2) / z / 166320.0 + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / x2) / x2) / x2) / x2) / x1 / 166320.0 + (13860.0 - (462.0 - (132.0 - (99.0 - 140.0 / w2) / w2) / w2) / w2) / w / 166320.)
		  			goto finis;
	  		}
		}
	}

	L_np_small:
    /*---------------------- np = n*p < 30 : ------------------------- */

	while(1)
	{
    	ix = 0;
    	f = MT(qn);
    	u = unif_rand(state);
    	while(1)
		{
	 		if (u < f)
	    		 goto finis;
	 		if (ix > 110)
	     		break;
	 		u -= f;
	 		ix++;
	 		f *= (g / ix - r);
     	}
  	}
 	finis:
    if (MT(psave) > 0.5)
		ix = n - ix;
  
	return (double)ix;
}

