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
 *  SYNOPSIS
 *
 *    #include <Rmath.h>
 *    double rchisq(double df);
 *
 *  DESCRIPTION
 *
 *    Random variates from the chi-squared distribution.
 *
 *  NOTES
 *
 *    Calls rgamma to do the real work.
 */

#include "nmath.h"

double rchisq(NMATH_STATE *state, double df)
{
    if (!isfinite(df) || df <= 0.0) return NAN;

    return rgamma(state, df / 2.0, 2.0);
}
