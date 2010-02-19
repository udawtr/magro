/*
 *  NMathlib : A C Library of Special Functions
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
 */

#include <stdio.h>
#include <sys/resource.h>
#include <time.h>
#include <assert.h>
#include "nmath.h"

void NMath_Init(NMATH_STATE *state)
{
	assert(state!=NULL);

	RNG_Init(state, (int)time(NULL));
   // RNG_Init(state, 12345);

	state->rgamma.aa  = 0.0;
	state->rgamma.aaa = 0.0;

	state->rbinom.psave = -1.0;
	state->rbinom.nsave = -1;

	state->gammafn.ngam = 0;
	state->gammafn.xmin = 0;
	state->gammafn.xmax = 0.0;
	state->gammafn.xsml = 0.0;
	state->gammafn.dxrel = 0.0;
}


