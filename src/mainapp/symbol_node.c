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
	if( symbol != NULL )
	{
		node_init(&symbol->node, m);
		symbol->node.nodetype = N_SYMBOL;
	}

	return symbol;
}

void symbol_node_free(SYMBOL_NODE* symbol)
{
	assert(symbol != NULL);

	if( symbol != NULL )
	{
		symbol->node.refcount--;
		if( symbol->node.refcount <= 0 )
		{
			node_destroy((NODE*)symbol);
			if( symbol->name != NULL )
			{
				GC_FREE(symbol->name);
				symbol->name = NULL;
			}
			GC_FREE(symbol);
		}
	}
}

int symbol_node_compare(SYMBOL_NODE* s1, SYMBOL_NODE* s2)
{
	int i, n;
	if( s1 != NULL && s2 != NULL && s1->node.nodetype == N_SYMBOL && s2->node.nodetype == N_SYMBOL)
	{
		printf("%s compare with %s\n", node_tostring(s1), node_tostring(s2));
		if( strcmp(node_tostring(s1), node_tostring(s2) ) == 0 ) return 0;
		return -1;

		if( strcmp(s1->name, s2->name) != 0 )
		{
			return 1;
		}

		if( s1->node.parents->count == s2->node.parents->count )
		{
			n = s1->node.parents->count;
			for( i = 0 ; i < n ; i++ )
			{
				NODE *p1, *p2;
				CONSTANT_NODE *c1, *c2;
				p1 = s1->node.parents->items[i];
				p2 = s2->node.parents->items[i];
				c1 = (CONSTANT_NODE*)p1;
				c2 = (CONSTANT_NODE*)p2;
				if( p1->nodetype == N_SYMBOL && p2->nodetype == N_SYMBOL )
				{
					printf("%s compare with %s", node_tostring(p1), node_tostring(p2));
					if( strcmp(node_tostring(p1), node_tostring(p2) ) != 0 )
					{
						printf(" NEQ\n");
						return -2;
					}
					printf(" EQ\n");
					return 0;
				}
				else if( p1->nodetype == N_CONSTANT && p2->nodetype == N_SYMBOL )
				{
					printf("%s compare with %s", c1->name, node_tostring(p2));
					if( strcmp(c1->name, node_tostring(p2) ) != 0 )
					{
						printf(" NEQ\n");
						return -2;
					}
					printf(" EQ\n");
					return 0;
				}
				else if( p1->nodetype == N_SYMBOL && p2->nodetype == N_CONSTANT )
				{
					printf("%s compare with %s", node_tostring(p1), c2->name);
					if( strcmp(node_tostring(p1), c2->name ) != 0 )
					{
						printf(" NEQ\n");
						return -2;
					}
					printf(" EQ\n");
					return 0;
				}
				else
				{
					printf("%f compare with %f", node_getvalue(p1), node_getvalue(p2));
					if( node_getvalue(p1) != node_getvalue(p2) )
					{
						printf(" NEQ\n");
						return -2;
					}
					printf(" EQ\n");
					return 0;
				}
			}
			return 0;
		}
	}
	return -1;
}

char *_symbol_node_tostring(SYMBOL_NODE* symbol, int index_base)
{
	int i, off, sz;
	int nindex;
	char** sparams;
	char* buf;

	assert(symbol != NULL);
	assert(symbol->node.nodetype == N_SYMBOL);

	if( symbol != NULL )
	{
		nindex = symbol->node.parents->count;
		sparams = (char**)GC_MALLOC_ATOMIC(sizeof(char*)*nindex);
		sz = strlen(symbol->name);
		for( i = 0 ; i < nindex ; i++ )
		{
			NODE* p = symbol->node.parents->items[i];
			char* s = node_tostring(p);
			sparams[i] = (char*)GC_MALLOC_ATOMIC(sizeof(char)*(strlen(s)+1));
			snprintf(sparams[i], strlen(s)+1, "%s", s);
			sz += strlen(sparams[i]);
		}
		sz += (nindex-1)+2+1;

		buf = (char*)GC_MALLOC_ATOMIC(sizeof(char)*sz);
		if( buf != NULL )
		{
			off = 0;	

			snprintf(buf, sz, "%s", symbol->name);	
			if( nindex > 0 )
			{
				strcat(buf, "[");
				for( i = 0 ; i < nindex ; i++ )
				{
					strcat(buf, sparams[i]);
					if( i < nindex-1 )
						strcat(buf, ",");
					GC_FREE(sparams[i]);
				}	
				strcat(buf, "]");
			}
			GC_FREE(sparams);

			assert(strlen(buf) < sz);

			return buf;
		}
	}

	return "[symbol]";
}

char* symbol_node_tostring(SYMBOL_NODE* symbol)
{
	return _symbol_node_tostring(symbol, 0);
}

char* symbol_node_toenvstring(SYMBOL_NODE* symbol)
{
    int i,l;
	char* str = _symbol_node_tostring(symbol, 0);
	char* buf = GC_MALLOC_ATOMIC(sizeof(char)*(strlen(str)+255));

	sprintf(buf, "env[index].%s", str);
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

