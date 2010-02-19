/*
 *  AUTHOR
 *    Catherine Loader, catherine@research.bell-labs.com.
 *    October 23, 2000.
 *
 *  Merge in to R:
 *	Copyright (C) 2000 The R Core Development Team
 *	Copyright (C) 2004 The R Foundation
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
 *
 * DESCRIPTION
 *
 *   Computes the density of the gamma distribution,
 *
 *                   1/s (x/s)^{a-1} exp(-x/s)
 *        p(x;a,s) = -----------------------
 *                            (a-1)!
 *
 *   where `s' is the scale (= 1/lambda in other parametrizations)
 *     and `a' is the shape parameter ( = alpha in other contexts).
 *
 * The old (R 1.1.1) version of the code is available via `#define D_non_pois'
 */

#include <math.h>
#include "nmath.h"

#define give_log log_p
#define R_D__0	(log_p ? -INFINITY : 0.0)

double dgamma(NMATH_STATE *state, double x, double shape, double scale, int give_log)
{
    double pr;
    
	if (isnan(x) || isnan(shape) || isnan(scale))
        return x + shape + scale;
    
	if (shape <= 0 || scale <= 0) return NAN;
    if (x < 0)
	return R_D__0;
    if (x == 0) {
		if (shape < 1) return INFINITY;
		if (shape > 1) return R_D__0;
		/* else */
		return give_log ? -log(scale) : 1 / scale;
    }

    if (shape < 1) 
	{
		pr = dpois_raw(state, shape, x/scale, give_log);
		return give_log ?  pr + log(shape/x) : pr*shape/x;
    }
    /* else  shape >= 1 */
    pr = dpois_raw(state, shape-1, x/scale, give_log);
    return give_log ? pr - log(scale) : pr/scale;
}

