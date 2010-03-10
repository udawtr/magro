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

ARRAY_NODE* array_node_create(MODEL* m)
{
	ARRAY_NODE* array;
	int i;

	array = (ARRAY_NODE*)GC_MALLOC(sizeof(ARRAY_NODE));
	node_init(&array->node, m);
	array->node.nodetype = N_ARRAY;
	
	for( i = 0 ; i < ARRAY_MAX_DIMS ; i++ )
		array->dims[i] = 1;
	
	return array;
}

void array_node_setdimension(ARRAY_NODE* array, int* dims, int ndims)
{
	int i;
	assert(ARRAY_MAX_DIMS >= ndims);
	for( i = 0 ; i < ndims ; i++ )
	{
		array->dims[i] = dims[i];
	}
	for( ; i < ARRAY_MAX_DIMS ; i++ )
	{
		array->dims[i] = 1;
	}
}

void array_node_free(ARRAY_NODE* array)
{
	assert(array != NULL);
	array->node.refcount--;
	if( array->node.refcount <= 0 )
	{
		node_destroy((NODE*)array);
		GC_FREE(array);
	}
}

char* array_node_tostring(ARRAY_NODE* array)
{
	int n;
	char** sparams;
	int i, i1, i2, i3, index[3], c, off, sz;
	char* buf;

	assert(array!=NULL);
	
	n = array_node_getsize(array);
	sparams = (char**)GC_MALLOC(sizeof(char*) * n);
	sz = strlen(array->name);
	
	c = 0;
	for( i1 = 0 ; i1 < array->dims[0] ; i1++ )
	for( i2 = 0 ; i2 < array->dims[1] ; i2++ )
	for( i3 = 0 ; i3 < array->dims[2] ; i3++, c++ )
	{
		index[0] = i1; index[1] = i2; index[2] = i3;
		sparams[c] = node_tostring(array_node_getnode(array, index));
		sz += strlen(sparams[c]);
	}
	sz += 4 + n-1 + 2 + 100;

	buf = (char*)GC_MALLOC(sizeof(char)*sz);
	off = 0;

	sprintf(buf, "structure(<%s>c(", array->name);
	for( i = 0 ; i < c ; i++ )
	{
		strcat(buf, sparams[i]);
		if( i < c-1 ) strcat(buf, ",");
		GC_FREE(sparams[i]);
	}
	strcat(buf, "),Dim=c(");
	for( i = 0 ; i < ARRAY_MAX_DIMS ; i ++ )
	{
		char tmp[100];
		sprintf(tmp, "%d", array->dims[i]);
		strcat(buf, tmp);
		if( i < ARRAY_MAX_DIMS-1 ) 
			strcat(buf, ",");
	}
	strcat(buf, "))");

	assert(strlen(buf) < sz);

	return buf;
}

char* array_node_toenvstring(ARRAY_NODE* array)
{
	char* buf;
	int i, l;

	assert(array!=NULL);
	assert(array->node.model != NULL);

	buf = (char*)GC_MALLOC_ATOMIC(sizeof(char) * (strlen(array->name)+50));
	sprintf(buf, "env[index].%s", array->name);

	l = strlen(array->name);
	for( i = 4 ; i < l+4 ; i++ )
	{
		if( buf[i] == '.' ) buf[i] = '_';
	}

	return buf; 
}

void array_node_setname(ARRAY_NODE* symbol, char* name)
{
   	int len,sz;
	assert(symbol != NULL);
    len = strlen(name);
	sz = sizeof(char) * (len+1);
	symbol->name = (char*)GC_MALLOC(sz);
    memcpy(symbol->name, name, sz);
	symbol->name[len] = '\0';
}

int array_node_getsize(ARRAY_NODE* array)
{
	assert(array != NULL);
	return array->node.parents->count;
}

void array_node_add(ARRAY_NODE* array, NODE* item)
{
	assert(array != NULL);
	node_addparent(&array->node, item);
	array->dims[0] = NP(array)->parents->count;
}

NODE* array_node_getnode(ARRAY_NODE* array, int* index)
{
	assert(array != NULL);
	if( index != NULL )
	{
		//int i;
		//for( i = 0 ; i < ARRAY_MAX_DIMS ; i++ ) printf("array_node_getnode: dims[%d] = %d\n", i, array->dims[i]);
		//for( i = 0 ; i < ARRAY_MAX_DIMS ; i++ ) printf("array_node_getnode: index[%d] = %d\n", i, index[i]);
		int off = index[0] + index[1]*(array->dims[0]) + index[2]*(array->dims[0]*array->dims[1]);
		//printf("array_node_getnode: off = %d\n", off);
		return array->node.parents->items[off];
	}
	else
	{
		return array->node.parents->items[0];
	}
}

