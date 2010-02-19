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

SYMBOL_NODE* symbol_node_create(MODEL* m)
{
	SYMBOL_NODE* symbol;
	symbol = (SYMBOL_NODE*)GC_MALLOC(sizeof(SYMBOL_NODE));
	node_init(&symbol->node, m);
	symbol->node.nodetype = N_SYMBOL;
	return symbol;
}

void symbol_node_free(SYMBOL_NODE* symbol)
{
	assert(symbol != NULL);
	node_destroy((NODE*)symbol);
	if( symbol->name != NULL )
	{
		free(symbol->name);
		symbol->name = NULL;
	}
	free(symbol);
}

int symbol_node_compare(SYMBOL_NODE* s1, SYMBOL_NODE* s2)
{
	int i, n;
	if( s1 != NULL && s2 != NULL && s1->node.nodetype == N_SYMBOL && s2->node.nodetype == N_SYMBOL)
	{
//		printf("symbol_node_comare: %s vs %s => ", symbol_node_tostring(s1), symbol_node_tostring(s2));
		if( strcmp(s1->name, s2->name) != 0 )
		{
//			printf("different name\n");
			return 1;
		}
		else if( s1->node.parents->count == s2->node.parents->count )
		{
			n = s1->node.parents->count;
			for( i = 0 ; i < n ; i++ )
			{
				if( node_getvalue(s1->node.parents->items[i]) != node_getvalue(s2->node.parents->items[i]) )
				{
//					printf("different index\n");
					return -2;
				}
			}
//			printf("equal\n");
			return 0;
		}
//		printf("different dimension\n");
	}
	return -1;
}

char *_symbol_node_tostring(SYMBOL_NODE* symbol, int index_base)
{
	assert(symbol != NULL);
	assert(symbol->node.nodetype == N_SYMBOL);
	int i, off, sz;
	int nindex = symbol->node.parents->count;
	char** sparams = (char**)GC_MALLOC_ATOMIC(sizeof(char*)*nindex);
	char* buf;
	
	sz = strlen(symbol->name);
	for( i = 0 ; i < nindex ; i++ )
	{
		CONSTANT_NODE* cnode = (CONSTANT_NODE*)symbol->node.parents->items[i];
		sparams[i] = (char*)malloc(25);
		sprintf(sparams[i], "%d", (int)cnode->value-1+index_base);
		sz += strlen(sparams[i]);
	}
	sz += (nindex-1)+2+1;

	buf = (char*)malloc(sizeof(char)*sz);
	off = 0;	

	sprintf(buf, "%s", symbol->name);	
	if( nindex > 0 )
	{
		strcat(buf, "[");
		for( i = 0 ; i < nindex ; i++ )
		{
			strcat(buf, sparams[i]);
			if( i < nindex-1 )
				strcat(buf, ",");
		}	
		strcat(buf, "]");
	}

	assert(strlen(buf) < sz);

	return buf;
}

char* symbol_node_tostring(SYMBOL_NODE* symbol)
{
	return _symbol_node_tostring(symbol, 0);
}

char* symbol_node_toenvstring(SYMBOL_NODE* symbol)
{
	char* str = _symbol_node_tostring(symbol, 0);
	char* buf = GC_MALLOC_ATOMIC(sizeof(char)*(strlen(str)+255));
	sprintf(buf, "env[index].%s", str);
        int i,l;
        l = strlen(buf);
        for ( i = 11 ; i < l ; i++ )
        {
            if( buf[i] == '.' ) buf[i] = '_';
        }
	return buf;
}

void symbol_node_setname(SYMBOL_NODE* symbol, char* name)
{
	int len,sz;
	assert(symbol != NULL);
	len = strlen(name);
	sz = sizeof(char) * (len+1);
	symbol->name = (char*)GC_MALLOC_ATOMIC(sz);
	memcpy(symbol->name, name, sz);
	symbol->name[sz-1] = '\0';
}

