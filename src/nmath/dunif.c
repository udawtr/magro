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
 *    The density of the uniform distribution.
 */

#include "nmath.h"
#include "dpq.h"

double dunif(NMATH_STATE *state, double x, double a, double b, int give_log)
{
    if (isnan(x) || isnan(a) || isnan(b)) return x + a + b;
    if (b <= a) return NAN;;

    if (a <= x && x <= b)
	return give_log ? -log(b - a) : 1. / (b - a);
    return R_D__0;
}
