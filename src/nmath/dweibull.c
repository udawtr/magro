/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998 Ross Ihaka
 *  Copyright (C) 2000 The R Development Core Team
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
 *  DESCRIPTION
 *
 *    The density function of the Weibull distribution.
 */

#include "nmath.h"
#include "dpq.h"

double dweibull(NMATH_STATE *state, double x, double shape, double scale, int give_log)
{
    double tmp1, tmp2;
    if (isnan(x) || isnan(shape) || isnan(scale)) return x + shape + scale;
    if (shape <= 0 || scale <= 0) return NAN;

    if (x < 0) return R_D__0;
    if (!isfinite(x)) return R_D__0;
    tmp1 = pow(x / scale, shape - 1);
    tmp2 = tmp1 * (x / scale);
    return  give_log ?
	-tmp2 + log(shape * tmp1 / scale) :
	shape * tmp1 * exp(-tmp2) / scale;
}
