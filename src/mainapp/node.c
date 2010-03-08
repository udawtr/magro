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

NODE* node_create(MODEL* m)
{
	NODE* node;
	node = (NODE*)GC_MALLOC(sizeof(NODE));
	node_init(node, m);
	node->nodetype = N_NODE;
	return node;
}

void node_init(NODE* node, MODEL* m)
{
	assert(node!=NULL);
	assert(m!=NULL);
	node->model = m;
	node->parents = nodelist_create();
	node->children = nodelist_create();
	node->order = 0;
	node->_isobserved = 0;
	node->_isinitialized = 0;
	node->refcount = 1;
}

void node_initialize(NODE* node, NMATH_STATE *ms)
{
    switch(node->nodetype)
    {
    case N_NODE:
    case N_ARRAY:
    case N_CONSTANT:
    case N_SYMBOL:
    case N_FUNCTION:
    	break;
	case N_STOCHASTIC:
    	stochastic_node_randomsample((STOCHASTIC_NODE*)node, ms);
	}
}

void node_destroy(NODE* node)
{
	assert( node != NULL );
	if( node->parents != NULL )
	{
		nodelist_free(node->parents);
		node->parents = NULL;
	}
	if( node->children != NULL )
	{
		nodelist_free(node->children);
		node->children = NULL;
	}
}

void node_free(NODE* node)
{	
	assert( node != NULL );
	switch(node->nodetype)
	{
	case N_ARRAY:
		array_node_free((ARRAY_NODE*)node);
		break;
	case N_CONSTANT:
		constant_node_free((CONSTANT_NODE*)node);
		break;
	case N_RANGE:
		range_node_free((RANGE_NODE*)node);
		break;
	case N_SYMBOL:
		symbol_node_free((SYMBOL_NODE*)node);
		break;
	case N_STOCHASTIC:
		stochastic_node_free((STOCHASTIC_NODE*)node);
		break;
	case N_FUNCTION:
		function_node_free((FUNCTION_NODE*)node);
		break;
	default:
		exit(99);
	}		
}

char* node_tostring(NODE* node)
{
	if( node == NULL ) return "[NULL]";
	switch(node->nodetype)
	{
	case N_ARRAY:
		return array_node_tostring((ARRAY_NODE*)node);
	case N_CONSTANT:
		return constant_node_tostring((CONSTANT_NODE*)node);
	case N_RANGE:
		return range_node_tostring((RANGE_NODE*)node);
	case N_SYMBOL:
		return symbol_node_tostring((SYMBOL_NODE*)node);	
	case N_STOCHASTIC:
		return stochastic_node_tostring((STOCHASTIC_NODE*)node);
	case N_FUNCTION:
		return function_node_tostring((FUNCTION_NODE*)node);
	default:
		return "[?]";
	}
}


char* node_toenvstring(NODE* node)
{
    if( node == NULL ) return "[NULL]";
    switch(node->nodetype)
    {
	case N_NODE:
		return "node";
    case N_ARRAY:
        return array_node_toenvstring((ARRAY_NODE*)node);
    case N_CONSTANT:
        return constant_node_toenvstring((CONSTANT_NODE*)node);
    case N_SYMBOL:
        return symbol_node_toenvstring((SYMBOL_NODE*)node);
    case N_STOCHASTIC:
        return stochastic_node_toenvstring((STOCHASTIC_NODE*)node);
    case N_FUNCTION:
        return function_node_toenvstring((FUNCTION_NODE*)node);
    }
	return "[?]";
}

void node_addparent(NODE* node, NODE* parent)
{
	assert(node != NULL && node->parents);
	nodelist_add(node->parents, parent);
}

void node_addchild(NODE* node, NODE* child)
{
	assert(node != NULL && node->children);
	nodelist_add(node->children, child);
}

int node_isobserved(NODE* node)
{
	int i, n;
	assert( node != NULL );
	switch( node->nodetype )
	{
	case N_FUNCTION:
	case N_ARRAY:
		n = node->parents->count;
		for( i = 0 ; i < n ; i++ )
		{
			if( node_checkancestorsobserved(node->parents->items[i]) == 0 ) return 0;
		}
		return 1;
		break;
	case N_STOCHASTIC:
		return node_isobserved( nodedic_findsymbol(node->model->relations, node) );
	case N_CONSTANT:
		return 1;
	default:
		return node->_isobserved;
	}
}

void node_setvalue(NODE* node, double value)
{
	assert(node != NULL);
	switch(node->nodetype)
	{
	case N_CONSTANT:
		constant_node_setvalue((CONSTANT_NODE*)node, value);
	case N_STOCHASTIC:
		stochastic_node_setvalue((STOCHASTIC_NODE*)node, value);
	/*case N_NODE:
	case N_ARRAY:
	case N_RANGE:
	case N_SYMBOL:
	case N_FUNCTION:
		//printf("node_setvalue: can't set value [node type: %d]\n", node->nodetype);	
	*/}
}

double (*func_getvalue[8])(NODE* node) = {
	NULL,
	NULL, //N_NODE
	NULL, //N_ARRAY
	constant_node_getvalue, //N_CONSTANT
	NULL, //N_RANGE
	NULL, //N_SYMBOL
	stochastic_node_getvalue, //N_STOCHASTIC
	function_node_getvalue //N_FUNCTION
};

double node_getvalue(NODE* node)
{
	char *tmp;
	assert(node != NULL);
	//if( func_getvalue[node->nodetype] != NULL )
	{
		return func_getvalue[node->nodetype](node);
	}
	
	switch(node->nodetype)
	{
	case N_CONSTANT:
		return constant_node_getvalue((CONSTANT_NODE*)node);
	case N_FUNCTION:
		return function_node_getvalue((FUNCTION_NODE*)node);
	case N_STOCHASTIC:
		return stochastic_node_getvalue((STOCHASTIC_NODE*)node);
	case N_NODE:
		printf("node_getvalue: can't get value [node type:N_NODE]\n");
		break;
	case N_ARRAY:
		tmp = node_tostring(node);
		printf("node_getvalue: can't get value [node type:N_ARRAY] expressoin=%s\n", tmp);
		GC_FREE(tmp);
		break;
	case N_RANGE:
		printf("node_getvalue: can't get value [node type:N_RANGE]\n");
		break;
	case N_SYMBOL:
		printf("node_getvalue: can't get value [node type:N_SYMBOL]\n");
		break;
	}
	return 0.0;
}

int node_checkancestorsobserved(NODE* node)
{
	int i, n;
	if( node_isobserved(node) == 0 ) return 0;
	n = node->parents->count;
	for( i = 0 ; i < n ; i++ )
	{
		if( node_checkancestorsobserved(node->parents->items[i]) == 0 ) return i+1;
	}
	return 0;
}

int node_isdescendant(NODE* node, NODE* target)
{
	assert(node!=NULL && target!=NULL);
	int i;
	if( node == target ) return 1;
	if( node->children != NULL )
	{
		for( i = 0 ; i < node->children->count ; i++ )
		{
			if( node_isdescendant(node->children->items[i], target) ) return 1;
		}
	}
	return 0;
}

int node_isancestor(NODE* node, NODE* target)
{
	assert(node!=NULL && target!=NULL);
	int i;
	if( node == target ) return 1;
	if( node->parents != NULL )
	{
		for( i = 0 ; i < node->parents->count ; i++ )
		{
			if( node_isancestor(node->parents->items[i], target) ) return 1;
		}
	}
	return 0;
}

int node_getdescendantcount(NODE* node)
{
	int i, n, t=0;
	assert(node!=NULL);
	if( node->children != NULL)
	{
		n = node->children->count;
		for( i = 0 ; i < n ; i++ )
		t += node_getdescendantcount(node->children->items[i]);
	}
	return t;
}

int node_getancestorcount(NODE* node)
{
	int i, n, t=0;
	assert(node!=NULL);
	if( node->parents!= NULL)
	{
		n = node->parents->count;
		for( i = 0 ; i < n ; i++ )
		t += node_getancestorcount(node->parents->items[i]);
	}
	return t;
}

void node_setobserved(NODE* node)
{
	assert( node != NULL );
	node->_isobserved = 1;
}

void node_setinitialized(NODE* node)
{
	assert( node != NULL );
	node->_isinitialized = 1;
}

int node_isinitialized(NODE* node)
{
	assert( node != NULL );
	return node->_isinitialized;
}

void node_check(NODE* node)
{
	return;
	NODE_RANGE_CHECK(node);
	
	if( node->parents != NULL )
	{
		nodelist_check(node->parents);	
	}
	if( node->children != NULL )
	{
		nodelist_check(node->children);
	}	
}

void nodelist_check(NODELIST* nodelist)
{
	int i;
	assert(nodelist != NULL);
	assert(nodelist->count >= 0);
	for( i = 0 ; i < nodelist->count ; i++ )
	{
		node_check(nodelist->items[i]);
	}
}

NODELIST* nodelist_create()
{
	NODELIST* list;
	list = (NODELIST*)GC_MALLOC(sizeof(NODELIST));
	list->count = 0;
	list->items = NULL;
	return list;
}

void nodelist_add(NODELIST* pnodelist, NODE* pnode)
{
	NODE** pnewitems;
	NODE** polditems = pnodelist->items;
	int count = pnodelist->count + 1;

	pnewitems = (NODE**)GC_MALLOC(sizeof(NODE*) * count);
	if( polditems != NULL )
	{
		memcpy(pnewitems, polditems, sizeof(NODE*) * (count-1));
		GC_FREE(polditems);
		polditems = NULL;
	}
	pnewitems[count-1] = pnode;
	pnode->refcount++;

	pnodelist->items = pnewitems;
	pnodelist->count = count;
}

int nodelist_contains(NODELIST* list, NODE* node)
{
	int i, n;
	assert( list != NULL && node != NULL );
	n = list->count;
	for( i = 0 ; i < n ; i++ )
	{
		if( list->items[i] == node ) return 1;
	}
	return 0;
}

void nodelist_free(NODELIST* list)
{
	int i;
	if( list != NULL )
	{	
		for( i = 0 ; i < list->count ; i++ )
		{
			node_free(list->items[i]);
			list->items[i] = NULL;
		}
		GC_FREE(list);
	}
}

NODEDIC* nodedic_create()
{
    NODEDIC* dic;
    dic = (NODEDIC*)GC_MALLOC(sizeof(NODEDIC));
    dic->count = 0;
    dic->items = NULL;
    dic->symbols = NULL;
	return dic;
}

void nodedic_add(NODEDIC* pnodelist, NODE* symbol, NODE* pnode)
{
    NODE** pnewitems;
    NODE** polditems = pnodelist->items;
	NODE** pnewsymbols;
	NODE** poldsymbols = pnodelist->symbols;
	int count = pnodelist->count + 1;

    pnewitems = (NODE**)GC_MALLOC(sizeof(NODE*) * count);
    if( polditems != NULL )
    {
        memcpy(pnewitems, polditems, sizeof(NODE*) * (count-1));
        GC_FREE(polditems);
        polditems = NULL;
    }
    pnewitems[count-1] = pnode;
	pnode->refcount++;
    pnodelist->items = pnewitems;

	pnewsymbols = (NODE**)GC_MALLOC(sizeof(NODE*) * count);
	if( poldsymbols != NULL )
	{
		memcpy(pnewsymbols, poldsymbols, sizeof(NODE*) * (count-1));
		GC_FREE(poldsymbols);
		poldsymbols = NULL;
	}
	pnewsymbols[count-1] = symbol;
	symbol->refcount++;
	pnodelist->symbols = pnewsymbols;

    pnodelist->count = count;
}

int nodedic_contains(NODEDIC* dic, NODE* node)
{
    int i, n;
    assert( dic != NULL && node != NULL );
    n = dic->count;
    for( i = 0 ; i < n ; i++ )
    {
        if( dic->items[i] == node ) return 1;
    }
    return 0;
}

NODE* nodedic_findsymbol(NODEDIC* dic, NODE* node)
{
    int i, n;
    assert( dic != NULL && node != NULL );
    n = dic->count;
    for( i = 0 ; i < n ; i++ )
    {
        if( dic->items[i] == node ) return dic->symbols[i];;
    }
    return NULL;
}

NODE* nodedic_findnode_byliteral(NODEDIC* dic, char* literal)
{
//	printf("nodedic_findnode_byliteral: begin\n");
	int i, n;
	assert( dic != NULL && literal != NULL );
	n = dic->count;
	for( i = 0 ; i < n ; i++ )
	{
		char *s = node_tostring(dic->symbols[i]);
		if( strcmp(s, literal) == 0 )
		{
			GC_FREE(s);
			return dic->items[i];
		}
		GC_FREE(s);
	}
	return NULL;
}

void nodedic_free(NODEDIC* dic)
{
	int i;
    assert(dic!= NULL);
	for( i = 0 ; i < dic->count ; i++ )
	{
		node_free(dic->symbols[i]);
		node_free(dic->items[i]);
		dic->symbols[i] = NULL;
		dic->items[i] = NULL;
	}
    GC_FREE(dic);
}
