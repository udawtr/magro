﻿/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998 Ross Ihaka
 *  Copyright (C) 2000 The R Development Core Team
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
 *    #include "nmath.h"
 *    double rnorm(NMATH_STATE* state, double mu, double sigma);
 *
 *  DESCRIPTION
 *
 *    Random variates from the normal distribution.
 * 
 *    To support multi threading environment, add NMATH_STATE argument.
 *    (2010, Wataru Uda)
 */

#include "nmath.h"

double rnorm(NMATH_STATE *state, double mu, double sigma)
{
	if (!isfinite(mu) || !isfinite(sigma) || sigma < 0.0)
        return NaN;

    if (sigma == 0.0)
        return mu;
    else
        return mu + sigma * norm_rand(state);
}

