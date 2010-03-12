/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998 Ross Ihaka
 *  Copyright (C) 2000--2006  The R Development Core Team
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
 *    #include <Rmath.h>
 *    double rnbinom(double n, double p)
 *
 *  DESCRIPTION
 *
 *    Random variates from the negative binomial distribution.
 *
 *  NOTES
 *
 *    x = the number of failures before the n-th success
 *
 *  REFERENCE
 *
 *    Devroye, L. (1986).
 *    Non-Uniform Random Variate Generation.
 *    New York:Springer-Verlag. Page 480.
 *
 *  METHOD
 *
 *    Generate lambda as gamma with shape parameter n and scale
 *    parameter p/(1-p).  Return a Poisson deviate with mean lambda.
 */

#include "nmath.h"

double rnbinom(NMATH_STATE *state, double n /* size */, double p /* prob */)
{
    if(!isfinite(n) || !isfinite(p) || n <= 0 || p <= 0 || p > 1)
	/* p = 1 is ok, PR#1218 */
	return NAN;
    
	return (p == 1) ? 0 : rpois(state, rgamma(state, n, (1 - p) / p));
}
