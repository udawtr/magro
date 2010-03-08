/*
 *  MAGRO: MCMC Another Gibbs Sampler
 *
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
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include "node.h"
#include "model.h"
#include "../nmath/nmath.h"
#include "distribution.h"
#include "conf.h"

RANGE_NODE* range_node_create(MODEL* m, double begin, double end)
{
	RANGE_NODE* range;
	range = (RANGE_NODE*)GC_MALLOC(sizeof(RANGE_NODE));
	node_init(&range->node, m);
	range->node.nodetype = N_RANGE;
	range->begin = begin;
	range->end = end;
	return range;
}

void range_node_free(RANGE_NODE* range)
{
	assert(range != NULL);
	range->node.refcount--;
	if( range->node.refcount <= 0 )
	{
		node_destroy((NODE*)range);
		GC_FREE(range);
	}
}

char* range_node_tostring(RANGE_NODE* range)
{
	assert(range != NULL);
	return "range";
}

