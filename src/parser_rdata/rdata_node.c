/*
 *  R DATA NODE: functions for R data node
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
#include <string.h>
#include <stdlib.h>
//#include <gc.h>
#include <assert.h>
#include "rdata_node.h"

#ifndef GC_MALLOC
#define GC_MALLOC malloc
#define GC_MALLOC_ATOMIC malloc
#define GC_FREE free
#endif

RDATA_NODE* rdata_node_create(RDATA_NODETYPE nodetype)
{
	RDATA_NODE* pnode;
	pnode = (RDATA_NODE*)GC_MALLOC(sizeof(RDATA_NODE));
	pnode->nodetype = nodetype;
	pnode->params = NULL;
	pnode->values = NULL;
	pnode->name = NULL;
	return pnode;
}

void rdata_node_setname(RDATA_NODE* pnode, char* name)
{
	int l,sz;

	if( pnode->name != NULL ) GC_FREE(pnode->name);
	l = strlen(name);
	sz = sizeof(char)*(l+1);
	pnode->name = GC_MALLOC_ATOMIC(sizeof(char) * sz);
	memcpy(pnode->name, name, sz);
	pnode->name[sz-1] = '\0';	
}

void rdata_node_addparam(RDATA_NODE* pnode, RDATA_NODE* param)
{
	if( pnode->params == NULL ) pnode->params = rdata_nodelist_create(0);
	rdata_nodelist_add(pnode->params, param);
}

void rdata_node_addvalue(RDATA_NODE* pnode, double value)
{
	if( pnode->values == NULL ) pnode->values = rdata_valuelist_create(0);
	rdata_valuelist_add(pnode->values, value);
}

void __rdata_node_dump_core(RDATA_NODE* pnode, int depth)
{
	int i,j;
	for( j = 0 ; j < depth ; j++ ) printf("  ");
	
	switch(pnode->nodetype)
	{
	case RDN_NAME: printf("RDN_NAME");break;
	case RDN_ASSIGN: printf("RDN_ASSIGN");break;
	case RDN_ARRAY: printf("RDN_ARRAY");break;
	case RDN_VECTOR: printf("RDN_VECTOR");break;
	case RDN_VALUE: printf("RDN_VALUE");break;
	case RDN_VAR: printf("RDN_VAR");break;
	}

	if( pnode->name != NULL )
	{
		printf(" name='%s'", pnode->name);
	}

	if( pnode->params != NULL )
	{
		printf(" nparams=%d", pnode->params->count);
	}
	else
	{	
		printf(" nparams=0");
	}

	if( pnode->values != NULL )
	{
		printf(" nvalues=%d", pnode->values->count);
	}
	else
	{
		printf(" nvalues=0");
	}

	printf("\n");
	
	if( pnode->params != NULL )
	{
		for( i = 0 ; i < pnode->params->count ; i++ )
		{
			__rdata_node_dump_core(pnode->params->items[i], depth+1);
		}
	}

	if( pnode->values != NULL )
	{
		for( i = 0 ; i < pnode->values->count ; i++ )
		{
			for( j = 0 ; j <= depth ; j++ ) printf("  ");
			printf("%f\n", pnode->values->items[i]);
		}
	}
}

void rdata_node_dump(RDATA_NODE* pnode)
{
	printf("**BEGIN DUMP**\n");
	__rdata_node_dump_core(pnode, 0);	
	printf("**END DUMP**\n");
}

void rdata_node_free(RDATA_NODE* pnode)
{
	if( pnode != NULL)
	{
		if( pnode->name != NULL ) GC_FREE(pnode->name);
		if( pnode->params != NULL ) rdata_nodelist_free(pnode->params);
		if( pnode->values != NULL ) rdata_valuelist_free(pnode->values);
		GC_FREE(pnode);
	}
}

RDATA_NODELIST* rdata_nodelist_create(int count)
{
	RDATA_NODELIST* pnodelist;
	int i ;
	pnodelist = (RDATA_NODELIST*)GC_MALLOC(sizeof(RDATA_NODELIST));
	pnodelist->count = count;
	if( count > 0 )
	{
		pnodelist->items= (RDATA_NODE**)GC_MALLOC(sizeof(RDATA_NODE*) * count);
		for ( i = 0 ; i < count ; i++ )
		{
			pnodelist->items[i] = NULL;
		}
	}
	else
	{
		pnodelist->items = NULL;
	}
	return pnodelist;
}

void rdata_nodelist_add(RDATA_NODELIST* pnodelist, RDATA_NODE* pnode)
{
	RDATA_NODE** pnewitems;
	RDATA_NODE** polditems = pnodelist->items;
	int count = pnodelist->count + 1;

	pnewitems = (RDATA_NODE**)GC_MALLOC(sizeof(RDATA_NODE*) * count);
	if( polditems != NULL )
	{
		memcpy(pnewitems, polditems, sizeof(RDATA_NODE*) * (count-1));
		GC_FREE(polditems);
		polditems = NULL;
	}
	pnewitems[count-1] = pnode;

	pnodelist->items = pnewitems;
	pnodelist->count = count;
}

void rdata_nodelist_free(RDATA_NODELIST* pnodelist)
{
	int i;
	int count;
	RDATA_NODE* pnode;
	if( pnodelist != NULL )
	{
		count = pnodelist->count;
		for( i = 0 ; i < count ; i++ )
		{
			pnode = pnodelist->items[i];
			if( pnode != NULL)
			{
				rdata_node_free(pnode);
			}
		}
		GC_FREE(pnodelist->items);
		GC_FREE(pnodelist);
	}
}

RDATA_VALUELIST* rdata_valuelist_create(int count)
{
	RDATA_VALUELIST* plist;
	plist = (RDATA_VALUELIST*)GC_MALLOC_ATOMIC(sizeof(RDATA_VALUELIST));
	plist->count = count;
	if( count > 0 ){
		plist->items = (double*)GC_MALLOC_ATOMIC(sizeof(double) * count);
	}else{
		plist->items = NULL;
	}
	return plist;
}

void rdata_valuelist_add(RDATA_VALUELIST* plist, double value)
{
#ifdef DEBUG
	printf("** rdata_valuelist_add:\n");
	printf("     plist = %u\n", plist);
	printf("     value = %f\n", value);
#endif
	double *pnewitems;
	double *polditems = plist->items;
	int count = plist->count+1;
	pnewitems = (double*)GC_MALLOC_ATOMIC(sizeof(double) * count); 
	if( polditems != NULL )
	{
		memcpy(pnewitems, polditems, sizeof(double) * (count-1));
		GC_FREE(polditems);
		polditems = NULL;
	}
	pnewitems[count-1] = value;
	plist->items = pnewitems;
	plist->count = count;
}

void rdata_valuelist_free(RDATA_VALUELIST* plist)
{
	if( plist != NULL )
	{
		if( plist->items != NULL) GC_FREE(plist->items);
		GC_FREE(plist);
	}
}


