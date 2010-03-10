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

CONSTANT_NODE* constant_node_create(MODEL* m)
{
	CONSTANT_NODE* constant;
	constant = (CONSTANT_NODE*)GC_MALLOC(sizeof(CONSTANT_NODE));
	node_init(&constant->node, m);
	constant->node.nodetype = N_CONSTANT;
	constant->value = 0;
	constant->name = NULL; 
	constant_node_setvalue(constant, 1.0);
	return constant;
}

void constant_node_free(CONSTANT_NODE* constant)
{
	assert(constant != NULL);
	constant->node.refcount--;
	if( constant->node.refcount <= 0 )
	{
		node_destroy((NODE*)constant);
	
		if( constant->name != NULL )
		{
			GC_FREE(constant->name);
			constant->name = NULL;
		}

		GC_FREE(constant);
	}
}

char* constant_node_tostring(CONSTANT_NODE* constant)
{
	char* buf;

	assert(constant != NULL);

	buf  = (char*)GC_MALLOC(sizeof(char) * 255);
	if( constant->name != NULL )
		sprintf(buf, "%s", constant->name);
	else
		sprintf(buf, "%f", constant_node_getvalue(constant));
	return buf;
}

char* constant_node_toenvstring(CONSTANT_NODE* constant)
{
	char* buf;
	assert(constant != NULL);
	buf = (char*)GC_MALLOC_ATOMIC(sizeof(char) * 255);
	if( constant->name != NULL )
	{
		int i,l;
		sprintf(buf, "env[index].%s", constant->name);
		l = strlen(constant->name);
		for ( i = 11 ; i < l+11 ; i++ )
		{
			if( buf[i] == '.' ) buf[i] = '_';
		}
	}
	else
		sprintf(buf, "%f", constant->value);
	return buf;
}

void constant_node_setname(CONSTANT_NODE* symbol, char* name)
{
   	int len, sz;
	assert(symbol != NULL);
    len = strlen(name);
	sz = sizeof(char) * (len+1);
	symbol->name = (char*)GC_MALLOC(sz);
    memcpy(symbol->name, name, sz);
	symbol->name[sz-1] = '\0';
}

void constant_node_setvalue(CONSTANT_NODE* constant, double value)
{
	assert(constant != NULL && constant->node.nodetype == N_CONSTANT);
	constant->value = value;
}

double constant_node_getvalue(CONSTANT_NODE* constant)
{
	assert(constant != NULL);
	return constant->value;
}

