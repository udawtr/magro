/*
 *  BUGS NODE: functions for bugs node 
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
#include "bugs_node.h"

#ifndef GC_MALLOC
#define GC_MALLOC malloc
#define GC_MALLOC_ATOMIC malloc
#define GC_FREE free
#endif

BUGS_NODE* bugs_node_create(BUGS_NODETYPE nodetype)
{
	BUGS_NODE* pnode;
	pnode = (BUGS_NODE*)GC_MALLOC(sizeof(BUGS_NODE));
	pnode->nodetype = nodetype;
	pnode->params = NULL;
	pnode->values = NULL;
	pnode->name = NULL;
	return pnode;
}

void bugs_node_setname(BUGS_NODE* pnode, char* name)
{
	int len, sz;
	if( pnode->name != NULL ) GC_FREE(pnode->name);
	len = strlen(name);
	sz = sizeof(char) * (len+1);
	pnode->name = GC_MALLOC(sz);
	memcpy(pnode->name, name, sz);	
	pnode->name[sz-1] = '\0';
//	printf("bugs_node_setname: '%s'(%d) => '%s'(%d)\n", name, strlen(name), pnode->name, strlen(pnode->name));
}

void bugs_node_addparam(BUGS_NODE* pnode, BUGS_NODE* param)
{
	if( pnode->params == NULL ) pnode->params = bugs_nodelist_create(0);
	bugs_nodelist_add(pnode->params, param);
}

void bugs_node_addvalue(BUGS_NODE* pnode, double value)
{
	if( pnode->values == NULL ) pnode->values = bugs_valuelist_create(0);
	bugs_valuelist_add(pnode->values, value);
}

void __bugs_node_dump_core(BUGS_NODE* pnode, int depth)
{
	int i,j;
	for( j = 0 ; j < depth ; j++ ) printf("  ");
	
	switch(pnode->nodetype)
	{
	case BN_NAME: printf("BN_NAME");break;
	case BN_VALUE: printf("BN_VALUE");break;
	case BN_VAR: printf("BN_VAR");break;
	case BN_RELATIONS: printf("BN_RELATIONS");break;
	case BN_FOR: printf("BN_FOR");break;
	case BN_COUNTER: printf("BN_COUNTER");break;
	case BN_DETERMREL: printf("BN_DETERMREL");break;
	case BN_STOCHREL: printf("BN_STOCHREL");break;
	case BN_LINK: printf("BN_LINK");break;
	case BN_FUNC: printf("BN_FUNC");break;
	case BN_DIM: printf("BN_DIM");break;
	case BN_LENGTH: printf("BN_LENGTH");break;
	case BN_RANGE: printf("BN_RANGE");break;
	case BN_LIST: printf("BN_LIST");break;
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
			__bugs_node_dump_core(pnode->params->items[i], depth+1);
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

void bugs_node_dump(BUGS_NODE* pnode)
{
	printf("**BEGIN DUMP**\n");
	__bugs_node_dump_core(pnode, 0);	
	printf("**END DUMP**\n");
}

void bugs_node_free(BUGS_NODE* pnode)
{
	if( pnode != NULL)
	{
		if( pnode->name != NULL ) GC_FREE(pnode->name);
		if( pnode->params != NULL ) bugs_nodelist_free(pnode->params);
		if( pnode->values != NULL ) bugs_valuelist_free(pnode->values);
		GC_FREE(pnode);
	}
}

BUGS_NODELIST* bugs_nodelist_create(int count)
{
	BUGS_NODELIST* pnodelist;
	int i ;
	pnodelist = (BUGS_NODELIST*)GC_MALLOC(sizeof(BUGS_NODELIST));
	pnodelist->count = count;
	if( count > 0 )
	{
		pnodelist->items= (BUGS_NODE**)GC_MALLOC(sizeof(BUGS_NODE*) * count);
		for ( i = 0 ; i < count ; i++ )
		{
			pnodelist->items[i] = NULL;
		}
	}else{
		pnodelist->items = NULL;
	}
	return pnodelist;
}

void bugs_nodelist_add(BUGS_NODELIST* pnodelist, BUGS_NODE* pnode)
{
	BUGS_NODE** pnewitems;
	BUGS_NODE** polditems = pnodelist->items;
	int count = pnodelist->count + 1;

	pnewitems = (BUGS_NODE**)GC_MALLOC(sizeof(BUGS_NODE*) * count);
	if( polditems != NULL )
	{
		memcpy(pnewitems, polditems, sizeof(BUGS_NODE*) * (count-1));
		GC_FREE(polditems);
		polditems = NULL;
	}
	pnewitems[count-1] = pnode;

	pnodelist->items = pnewitems;
	pnodelist->count = count;
}

void bugs_nodelist_free(BUGS_NODELIST* pnodelist)
{
	int i;
	int count;
	BUGS_NODE* pnode;
	if( pnodelist != NULL )
	{
		count = pnodelist->count;
		for( i = 0 ; i < count ; i++ )
		{
			pnode = pnodelist->items[i];
			if( pnode != NULL)
			{
				bugs_node_free(pnode);
			}
		}
		GC_FREE(pnodelist->items);
		GC_FREE(pnodelist);
	}
}

BUGS_VALUELIST* bugs_valuelist_create(int count)
{
	BUGS_VALUELIST* plist;
	plist = (BUGS_VALUELIST*)GC_MALLOC(sizeof(BUGS_VALUELIST));
	plist->count = count;
	if( count > 0 ){
		plist->items = (double*)GC_MALLOC(sizeof(double) * count);
	}else{
		plist->items = NULL;
	}
	return plist;
}

void bugs_valuelist_add(BUGS_VALUELIST* plist, double value)
{
#ifdef DEBUG
	printf("** bugs_valuelist_add:\n");
	printf("     plist = %u\n", plist);
	printf("     value = %f\n", value);
#endif
	double *pnewitems;
	double *polditems = plist->items;
	int count = plist->count+1;
	pnewitems = (double*)GC_MALLOC(sizeof(double) * count); 
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

void bugs_valuelist_free(BUGS_VALUELIST* plist)
{
	if( plist != NULL )
	{
		if( plist->items != NULL) GC_FREE(plist->items);
		GC_FREE(plist);
	}
}


