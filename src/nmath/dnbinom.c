/*
 *  AUTHOR
 *    Catherine Loader, catherine@research.bell-labs.com.
 *    October 23, 2000 and Feb, 2001.
 *
 *  Merge in to R:
 *	Copyright (C) 2000--2001, The R Core Development Team
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
 *   Computes the negative binomial distribution. For integer n,
 *   this is probability of x failures before the nth success in a
 *   sequence of Bernoulli trials. We do not enforce integer n, since
 *   the distribution is well defined for non-integers,
 *   and this can be useful for e.g. overdispersed discrete survival times.
 */

#include <stdio.h>
#include "nmath.h"
#include "dpq.h"

double dnbinom(NMATH_STATE *state, double x, double n, double p, int give_log)
{ 
    double prob;

    if (isnan(x) || isnan(n) || isnan(p))
        return x + n + p;

    if (p <= 0 || p > 1 || n <= 0) return NAN;
    R_D_nonint_check(x);
    if (x < 0 || !isfinite(x)) return R_D__0;
    x = R_D_forceint(x);

    prob = dbinom_raw(state, n, x+n, p, 1-p, give_log);
    p = ((double)n)/(n+x);
    return((give_log) ? log(p) + prob : p * prob);
}
