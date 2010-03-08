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
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../parser_bugs/bugs_node.h"
#include "../parser_rdata/rdata_node.h"
#include "env.h"
#include "compiler.h"
#include "sampler.h"
#include "conf.h"

COMPILER* compiler_create()
{
	COMPILER* c;
	c = GC_MALLOC(sizeof(COMPILER));
	c->env = env_create(NULL);
	c->model = model_create();
	c->graph = NULL;
	return c;
}

void compiler_free(COMPILER* c)
{
	assert( c != NULL );
	if( c->env != NULL)
	{
		ENV* env = env_stackout(c->env);
		while( env != NULL )
		{
			env = env_stackout(env);
		}
		c->env = NULL;
	}
	if( c->model != NULL )
	{
		model_free(c->model);
		c->model = NULL;
	}
	if( c->graph != NULL )
	{
		nodelist_free(c->graph);
		c->graph = NULL;
	}	
	GC_FREE(c);
}

void compiler_buildrelations(COMPILER* c, BUGS_NODE* node)
{
	int j;
	BUGS_NODE* rel;
	BUGS_NODE* counter;
	BUGS_NODE* relations;
	BUGS_NODE* var;
	BUGS_NODE* func;
	STOCHASTIC_NODE* snode;
	NODE* symbol;

		rel = node;
		
		switch(rel->nodetype)
		{
		case BN_LIST:
			for( j = 0 ; j < rel->params->count ; j++ )
			{
				compiler_buildrelations(c, rel->params->items[j]);
			}
			break;

		case BN_RELATIONS:
			if( rel->params != NULL )
			{
				for( j = 0 ; j < rel->params->count ; j++ )
				{
					compiler_buildrelations(c, rel->params->items[j]);
				}
			}
			break;

		case BN_FOR:
			counter = rel->params->items[0];
			assert( counter != NULL && counter->nodetype == BN_COUNTER );
			relations = rel->params->items[1];
			c->env = env_stackin(c->env);
			{
				RANGE_NODE* range = (RANGE_NODE*)compiler_eval(c, counter->params->items[0]);
				for( j = range->begin ; j <= range->end ; j++ )
				{
					CONSTANT_NODE* loopcounter  = constant_node_create(c->model);
					constant_node_setvalue(loopcounter, (double)j);
					env_setsymbol(c->env, counter->name, NP(loopcounter)); 
					compiler_buildrelations(c, relations);
					constant_node_free(loopcounter);
				}
				range_node_free(range);
			}
			c->env = env_stackout(c->env);
			break;

		case BN_STOCHREL:
			var = rel->params->items[0];
			func = rel->params->items[1];
			snode = stochastic_node_create(c->model);
			symbol = compiler_eval(c, var);
			stochastic_node_setname(snode, distribution_find(func->name));
			for( j = 0 ; j < func->params->count ; j++ )
			{
				node_addparent(NP(snode), compiler_eval(c, func->params->items[j]));
			}
			if( node_isobserved(symbol) )
			{
				node_setobserved(NP(snode));
				node_setvalue(NP(snode), node_getvalue(symbol));
			}
			model_addrelation(c->model, symbol, NP(snode));
			break;
	
		case BN_DETERMREL:
			var = rel->params->items[0];
			func = rel->params->items[1];
			model_addrelation(c->model, compiler_eval(c,var), compiler_eval(c,func));
			break;
			
		default:
			break;
		}

    int i,n = c->model->relations->count;
    for( i = 0 ; i < n ; i++ )
    {
        NODE_CHECK(c->model->relations->symbols[i]);
        NODE_CHECK(c->model->relations->items[i]);
    }

}

/*
SYMBOL compiler_create_symbol(BUGS_NODE* node)
{
}
*/

const char* compiler_getinversefuncname(char* name)
{
	if( strcmp(name, "cloglog") == 0 ) return "icloglog";
	else if( strcmp(name, "log") == 0 ) return "exp";
	else if( strcmp(name, "logit") == 0 ) return "ilogit";
	else if( strcmp(name, "probit") == 0 ) return "phi";
	return "----";
}

NODE* compiler_eval(COMPILER* c, BUGS_NODE* bnode)
{
	int i;
	CONSTANT_NODE* begin;
	CONSTANT_NODE* end;
	CONSTANT_NODE* index;
	FUNCTION_NODE* fnode;
	ARRAY_NODE* array;
	SYMBOL_NODE* symbol;
	
	assert(bnode != NULL);
	
	switch(bnode->nodetype)
	{
	case BN_RANGE:
		begin = (CONSTANT_NODE*)compiler_eval(c, bnode->params->items[0]);
		if( bnode->params->count > 1 )
			end   = (CONSTANT_NODE*)compiler_eval(c, bnode->params->items[1]);	
		else
			end   = begin;
		return (NODE*)range_node_create(c->model, constant_node_getvalue(begin), constant_node_getvalue(end));
		break;
	case BN_VAR:
		//printf("BN_VAR: name=%s\n", bnode->name);
		array =  (ARRAY_NODE*)env_find(c->env,  bnode->name);
		if( array != NULL )
		{
			array_node_setname(array, bnode->name);
			if( array->node.nodetype == N_CONSTANT )
			{
				//printf("BN_VAR: %s is constant \n", bnode->name);
				return (NODE*)array;
			}
			else if( array->node.nodetype == N_ARRAY )
			{
				//printf("BN_VAR: %s is array\n", bnode->name);
				if( bnode->params != NULL && bnode->params->count > 0 )
				{
					int dims[ARRAY_MAX_DIMS];
					for( i = 0 ; i < ARRAY_MAX_DIMS ; i++ ) dims[i] = 0;
					for( i = 0 ; i < bnode->params->count ; i++ )
					{
						//printf("BN_VAR => index: %s\n", bnode->params->items[i]->name);
						index = (CONSTANT_NODE*)env_find(c->env, bnode->params->items[i]->name);
						assert(index != NULL);
						assert(index->node.nodetype == N_CONSTANT);
						//printf("BN_VAR => index: %s = %f\n", bnode->params->items[0]->name, constant_node_getvalue(index));
						dims[i] = (int)constant_node_getvalue(index)-1;
					}
					return (NODE*)array_node_getnode(array, dims);
				}
				else
				{
					if( array_node_getsize(array) == 1 )
						return (NODE*)array_node_getnode(array, NULL);
					else
						return (NODE*)array;
				}
			}
		}
		else
		{
			symbol = symbol_node_create(c->model);
			symbol_node_setname(symbol, bnode->name);
			if( bnode->params != NULL && bnode->params->count > 0 )
			{
				for( i = 0 ; i < bnode->params->count ; i++ )
				{
//					printf("symbol %s index: %s\n", bnode->name,  node_tostring(compiler_eval(c, bnode->params->items[i])));
					node_addparent(&symbol->node, compiler_eval(c, bnode->params->items[i])); 
				}
			}
			return (NODE*)symbol;
		}
		break;
	case BN_VALUE:
		begin = (CONSTANT_NODE*)constant_node_create(c->model);
		constant_node_setvalue(begin, bnode->values->items[0]);
		return (NODE*)begin;
		break;
	case BN_STOCHREL:
		//printf("BN_STOCHREL\n");
		break;
	case BN_DETERMREL:
		//printf("BN_DETERMREL\n");
		break;
	case BN_FUNC:
		fnode = function_node_create(c->model);
		function_node_setname(fnode, bnode->name);
        for( i = 0 ; i < bnode->params->count ; i++ )
        {
        	node_addparent(&fnode->node, compiler_eval(c, bnode->params->items[i]));
        }
		return (NODE*)fnode; 
		break;
	case BN_LINK:
		fnode = function_node_create(c->model);
		function_node_setname(fnode, (char*)compiler_getinversefuncname(bnode->name));
        for( i = 0 ; i < bnode->params->count ; i++ )
        {
        	node_addparent(&fnode->node, compiler_eval(c, bnode->params->items[i]));
        }
		return (NODE*)fnode; 
		break;
	default:
		//printf("*******\n");
		return NULL;
	}
	return NULL;
}

/*
NODE* compiler_resolve_symbol()
{
}

void compiler_resolve_children(NODE* node)
{
}

void compiler_serialize_graph()
{
}
*/

void compiler_setrinit(COMPILER* c, RDATA_NODE* rnode)
{
	int i, j, n, len;
	RDATA_NODE* rarray;
	RDATA_NODE* rvector;
	RDATA_NODE* rvalue;
	char* name;
	ARRAY_NODE* array;
	CONSTANT_NODE* constant;;
	char* buf = (char*)GC_MALLOC_ATOMIC(sizeof(char) * 255);
	
	assert( rnode != NULL && rnode->nodetype == RDN_ASSIGN );	

	n = rnode->params->count;
	for( i = 0 ; i < n ; i++ )
	{
		rarray = rnode->params->items[i];
		if( rarray->nodetype == RDN_ARRAY )
		{
			name = rarray->name;
			array = array_node_create(c->model);
			array_node_setname(array, name);
			
			rvector = rarray->params->items[0];
			if( rvector->nodetype == RDN_VECTOR )
			{
				rvalue = rvector->params->items[0];
				if( rvalue->nodetype == RDN_VALUE )
				{
					int l = rvalue->values->count;
					for( j = 0 ; j < l ; j++ )
					{
						if( l > 1 )
    						len = sprintf(buf, "%s[%d]", name, j);
						else
							len = sprintf(buf, "%s", name);
						assert(len < 255);

						constant = constant_node_create(c->model);
						constant_node_setvalue(constant, rvalue->values->items[j]);
						constant_node_setname(constant, buf);
						array_node_add(array, NP(constant));
						
						NODE* node = nodedic_findnode_byliteral(c->model->relations, buf);
						if( node != NULL )//&& node->nodetype == N_STOCHASTIC )
						{
//							printf("compiler_setrinit: set initial value of %s to %f\n", buf, rvalue->values->items[j]);
							assert(node->nodetype == N_STOCHASTIC);
							stochastic_node_setvalue((STOCHASTIC_NODE*)node, rvalue->values->items[j]);
							node_setinitialized(node);
						}
						else
						{
							printf("compiler_setrinit: cannot find a stochastic node named %s\n", buf);
						}	
					}
				}
			}

			int index[1];
			index[0] = array_node_getsize(array);
			array_node_setdimension(array, &index[0], 1);
			env_setsymbol(c->env, name, NP(array));
		}
	}

	GC_FREE(buf); buf = NULL;
} 

void compiler_setrdata(COMPILER* c, RDATA_NODE* rnode)
{
	int i, j, n, len;
	RDATA_NODE* rarray;
	RDATA_NODE* rvector;
	RDATA_NODE* rvalue;
	char* name;
	ARRAY_NODE* array;
	CONSTANT_NODE* constant;
	char* buf = (char*)GC_MALLOC(sizeof(char) * 255);
	
	assert( rnode != NULL && rnode->nodetype == RDN_ASSIGN );	

	n = rnode->params->count;
	for( i = 0 ; i < n ; i++ )
	{
		rarray = rnode->params->items[i];
		if( rarray->nodetype == RDN_ARRAY )
		{
			name = rarray->name;
			array = array_node_create(c->model);
			array_node_setname(array, name);
			NODE_CHECK(array);
			
			rvector = rarray->params->items[0];
			if( rvector->nodetype == RDN_VECTOR )
			{
				rvalue = rvector->params->items[0];
				if( rvalue->nodetype == RDN_VALUE )
				{
					int l = rvalue->values->count;
					for( j = 0 ; j < l ; j++ )
					{
						if( l > 1 ) 
    						len = sprintf(buf, "%s[%d]", name, j);
    					else
							len = sprintf(buf, "%s", name);
						assert(len < 255);

						constant = constant_node_create(c->model);
						constant_node_setvalue(constant, rvalue->values->items[j]);
						constant_node_setname(constant, buf);
						array_node_add(array, NP(constant));
						NODE_CHECK(constant);
					}
				}
			}

			for( j = 1 ; j < rarray->params->count ; j++ )
			{
				rvector = rarray->params->items[j];
				if( strcmp(rvector->name, "dim") == 0 )
				{
					rvalue = rvector->params->items[0];
					if( rvalue->nodetype == RDN_VALUE )
					{
						int dims[ARRAY_MAX_DIMS];
						assert(rvalue->values->count < ARRAY_MAX_DIMS);
						for( j = 0 ; j < rvalue->values->count ; j++ )
						{
							dims[j] = (int)rvalue->values->items[j];
							//printf("%d\n", dims[j]);
						}
						array_node_setdimension(array, dims, rvalue->values->count);
					}
				}
			}

			NODE_CHECK(array);
			env_setsymbol(c->env, name, NP(array));
		}
	}

	GC_FREE(buf); buf = NULL;

	//DUMP env
	if( mode_verbose >=2 )
	{
		printf("***BEGIN DUMP ENV***\n");
		n = c->env->count;
		for( i = 0 ; i < n ; i++ )
		{
			printf("%s = [", c->env->names[i]);
			array = (ARRAY_NODE*)c->env->nodes[i];
			assert(array->node.nodetype == N_ARRAY);
			int l = array_node_getsize(array);
			int idx[3] = {0,0,0};
			for( j = 0 ; j < l ; j++ )
			{
				idx[0] = j;
				constant = (CONSTANT_NODE*)array_node_getnode(array, idx);
				assert(constant->node.nodetype == N_CONSTANT);
				printf("%f", constant->value);
				if( j < l-1 ) printf(",");
			}
			printf("]\n");
		}
		printf("***END DUMP ENV***\n");
	}
} 

void __compiler_resolvesymbols(NODE**, NODE**, int, NODE**);

void __compiler_resolvesymbols(NODE** pnode, NODE** symbols, int nsymbols, NODE** targets)
{
	assert(pnode!=NULL && symbols!=NULL);
	int i,n;
	NODE* node = *pnode;

	if( node->nodetype != N_SYMBOL )
	{
		n = node->parents->count;
		for( i = 0 ; i < n ; i++ )
		{
			__compiler_resolvesymbols(&node->parents->items[i], symbols, nsymbols, targets);
		}
	}
	else
	{
		for( i = 0 ; i < nsymbols ; i++ )
		{
			//assert(node->nodetype == N_SYMBOL);
			//assert(symbols[i]->nodetype == N_SYMBOL);

			if( NP(node) != NP(symbols[i])
			 && symbol_node_compare((SYMBOL_NODE*)node, (SYMBOL_NODE*)symbols[i]) == 0 )
			{
				//node_free(*pnode);
				*pnode = targets[i];
			}
		}
	}
}

void compiler_resolvesymbols(COMPILER* c)
{
	assert(c!=NULL);
	
	int i, n;
	NODE** symbols;
	NODE** targets;

	n = c->model->relations->count;
	symbols = (NODE**)GC_MALLOC(sizeof(NODE*) * n); 
	targets = (NODE**)GC_MALLOC(sizeof(NODE*) * n);
	for( i = 0 ; i < n ; i++ )
	{
        NODE_CHECK(c->model->relations->symbols[i]);
        NODE_CHECK(c->model->relations->items[i]);
		symbols[i] = c->model->relations->symbols[i]; 
		targets[i] = c->model->relations->items[i];
		NODE_CHECK(symbols[i]);
		NODE_CHECK(targets[i]);
	}
	
	for( i = 0 ; i < n ; i++ )
	{
		__compiler_resolvesymbols(&c->model->relations->items[i], symbols, n, targets);
	}
	
	if( mode_verbose > 2 )
	{
		printf("*** BEGIN DUMP SYMBOL TABLE***\n");
		for( i = 0 ; i < n ; i++ )
		{
			char *s1, *s2;
			s1 = node_tostring((NODE*)symbols[i]);
			s2 = node_tostring(targets[i]);
			printf("%s = %s\n", s1, s2);
			GC_FREE(s1);
			GC_FREE(s2);
		}
		printf("*** END DUMP SYMBOL TABLE***\n");
	}

	GC_FREE(symbols);
	GC_FREE(targets);
}

void __compiler_resolvechildren(NODE* node);

void __compiler_resolvechildren(NODE* node)
{
	int i, n;
	NODE* parent;

	assert(node != NULL);

	n = node->parents->count;
	assert(n>=0);

	for( i = 0 ; i < n ; i++ )
	{
		parent = node->parents->items[i];
		assert(parent != NULL);
		NODE_CHECK(parent);
		if( nodelist_contains(parent->children, node) == 0 )
		{
			NODE* sym = nodedic_findsymbol(node->model->relations, node);
			//printf("__compiler_resolvechildren: add child %s to parent %s\n",
			//	node_tostring(sym != NULL ? sym : node),
			//	node_tostring(nodedic_findsymbol(node->model->relations, parent)));
			//if(sym != NULL)
				node_addchild(parent, node);
		}
		//__compiler_resolvechildren(parent);
	}
}

void compiler_resolvechildren(NODELIST* graph)
{
	int i, n;	

	assert(graph != NULL);

	n = graph->count;
	assert(n>=0);
	for( i = 0 ; i < n ; i++ )
	{
		//printf("compiler_resolvechildren: add children to parent %s\n",
		//	node_tostring(nodedic_findsymbol(graph->items[i]->model->relations, graph->items[i])));
		__compiler_resolvechildren(graph->items[i]);
	}
}

void __compiler_serializegraph(NODE* node, NODELIST* graph)
{
	int i, j, n, nn, t, f;

	assert( node != NULL && graph != NULL );
	assert( graph->count >= 0);
	NODE_CHECK(node);

	n = graph->count;
	f = 0;
	for( i = 0 ; i < n ; i++ )
	{
		if( graph->items[i] == node ) 
		{
			f = 1;
			break;
		}
	}
	if( f == 0 )
	{
		nodelist_add(graph, node);
		n = node->parents->count;
		for( i = 0 ; i < n ; i++ )
		{
			__compiler_serializegraph(node->parents->items[i], graph);
		}
	}
}

void compiler_ordergraph(NODELIST* graph)
{
	int i, j, n, count, isfounder;
	NODE *node, *parent, **sorted;

	assert(graph != NULL);
	assert(graph->count > 0);

	count = 0;
	n = graph->count;
	sorted = (NODE**)GC_MALLOC(sizeof(NODE*)*n);

	// for each nodes, determinates the node is a leaf(has no parents) or not.
    // if the node is a leaf, set the node's order to '1';
	for( i = 0 ; i < n ; i++ )
	{
		node = graph->items[i];
		assert(node != NULL);
		isfounder = 1;
		/*
		for( j = 0 ; j < node->parents->count ; j++ )
		{
			if( nodelist_contains(graph, node->parents->items[j]) )
			{
				isfounder = 0;
				break;
			} 
		}
		*/
		if( isfounder == 1 )
		{
			node->order = 1;
			sorted[count++] = node;
		}
	}

	while(n>count)
	{
		int loopcheck = 1;
		for( i = 0 ; i < n ; i++ )
		{
			node = graph->items[i];
			if( node->order == 0 )
			{
				int canmark = 1;
				for( j = 0 ; j < node->parents->count ; j++ )
				{
					parent = node->parents->items[j];
					if( nodelist_contains(graph, parent) && parent->order == 0 )
					{
						canmark = 0;
						break;
					}
				}
				if( canmark ) {
					node->order = 1;
					sorted[count++] = node;
					loopcheck = 0;
				}
			}
		}
		if( loopcheck )
		{
			printf("* failure in compiler_ordergraph: directed cycle in graph\n");
			exit(90);
		}
	}

	memcpy(graph->items, sorted, sizeof(NODE*)*n);
	GC_FREE(sorted);
}

int __compiler_comparenode(const void* a, const void* b)
{
	NODE* na = *(NODE**)a;
	NODE* nb = *(NODE**)b;
	
	NODE_CHECK(na);
	NODE_CHECK(nb);

	return na->order - nb->order;
}

void compiler_serializegraph(COMPILER* c, NODELIST* graph)
{
	int i, n;
	
	assert( c != NULL && graph != NULL );
	n = c->model->relations->count;
	assert( n >= 0 );
	
	for( i = 0 ; i < n ; i ++ )
	{
		if( mode_verbose > 2 ) { printf("[%d]", graph->count); fflush(stdout); }
		__compiler_serializegraph(c->model->relations->items[i], graph);
	}
}


int __compiler_sampler_compare(const void *a, const void *b)
{
	SAMPLER* sa = *(SAMPLER**)a;
	SAMPLER* sb = *(SAMPLER**)b;

	SAMPLER_CHECK(sa);
	SAMPLER_CHECK(sb);
	NODE_CHECK(sa->snode);
	NODE_CHECK(sb->snode);

	int inda = NP(sa->snode)->order;
	int indb = NP(sb->snode)->order;

	return inda - indb;
}

void compiler_buildsamplers(COMPILER* c, NODELIST* graph)
{
	int i, j, n;
	NODE* node;
	NODELIST* list;

	assert(graph!=NULL);
	n = graph->count;
	for( i = 0 ; i < n; i++ )
	{
		node = graph->items[i];
		if( node->nodetype == N_STOCHASTIC && node_isobserved(node) == 0 )
		{
			list = nodelist_create();
			stochastic_node_findstochasticdescendant((STOCHASTIC_NODE*)node, list);
			for( j = 0 ; j < list->count ; j++ )
			{
//				printf("   => %s\n", node_tostring(list->items[j]));
			}
			model_addsampler(c->model, sampler_create((STOCHASTIC_NODE*)node));
			nodelist_free(list);
		}
	}

	for ( i = 0 ; i < n ; i ++)
		graph->items[i]->order = i;

	qsort(&c->model->samplers->items[0], c->model->samplers->count, 
			sizeof(SAMPLER*),
			__compiler_sampler_compare);
}

void compiler_initialize(COMPILER* c, NMATH_STATE *ms)
{
	assert(c!=NULL);

	//printf("***BEGIN INITIALIZE***\n");
	//printf("graph->count = %d\n", c->graph->count);

    int i,n = c->graph->count;
	NODE* node;
    for( i = 0 ; i < n ; i++ )
    {
		node = c->graph->items[i];
		if( node_isobserved(node) == 0 && node_isinitialized(node) == 0 )
		{
			node_initialize(node, ms);		
			/*
			printf("[%d] %s => %f\n",
				i,
				node_tostring(nodedic_findsymbol(c->model->relations, node)),
				node_getvalue(node)
			);
			*/
		}
	}
	//printf("***END INITIAILIZE***\n");
}

void compiler_compile(COMPILER* c,  BUGS_NODE* pnode)
{
	NODELIST* graph;
	NODELIST* list;
	int i,j,n;

	assert(c!=NULL);
	if( mode_verbose ) { printf("building relations ..."); fflush(stdout); }
	compiler_buildrelations(c, pnode);
	
	if( mode_verbose ) printf(" success (%d)\n", c->model->relations->count);
	if( mode_verbose > 1 )
	{
		//DUMP relations
		n = c->model->relations->count;
		printf("***BEGIN DUMP RELATIONS***\n");
		for( i = 0 ; i < n ; i++ )
		{
			printf("[%s]: %s\n", node_tostring(c->model->relations->symbols[i]), node_tostring(c->model->relations->items[i]));
		}	
		printf("***END DUMP RELATIONS***\n");
	}

    n = c->model->relations->count;
    for( i = 0 ; i < n ; i++ )
    {
        NODE_CHECK(c->model->relations->symbols[i]);
        NODE_CHECK(c->model->relations->items[i]);
    }

	if( mode_verbose ) { printf("resolving symbols ... "); fflush(stdout); }
	compiler_resolvesymbols(c);
	if( mode_verbose ) printf("success\n");

	if( mode_verbose ) printf("begin making the graph\n");
	graph = nodelist_create();
	if( mode_verbose ) { printf("  serializing nodes ... "); fflush(stdout); }
	compiler_serializegraph(c, graph);
	if( mode_verbose ) printf("success\n");
	if( mode_verbose ) { printf("  resolving the relations of children and parents of nodes ... "); fflush(stdout); }
	compiler_resolvechildren(graph);
	if( mode_verbose ) printf("success\n");
	if( mode_verbose ) { printf("  ordering the graph ... "); fflush(stdout); }
    compiler_ordergraph(graph);
	if( mode_verbose ) printf("success\n");
	if( mode_verbose ) printf("succeed in making the graph\n");
	
	// DUMP GRAPH
	if( mode_verbose > 2 )
	{
		printf("***BEGIN DUMP GRAPH***\n");
		n = graph->count;
		for( i = 0 ; i < n ; i++ )
		{
			printf("%s: (%s) order=%d, isobserved=%d, nchildren=%d, nparents=%d\n", 
			node_tostring(nodedic_findsymbol(c->model->relations, graph->items[i])),
			node_tostring(graph->items[i]), graph->items[i]->order, node_isobserved(graph->items[i]), graph->items[i]->children->count, graph->items[i]->parents->count);
			for( j = 0 ; j < graph->items[i]->parents->count ; j++ )
			{
				printf("     parents->items[%d] = %s\n", j, node_tostring(graph->items[i]->parents->items[j]));
			}
		}
		printf("***END DUMP GRAPH***\n");
	}
	c->graph = graph;

	compiler_buildsamplers(c, graph);

	if( mode_verbose > 1 )
	{
		printf("***BEGIN DUMP SAMPLERS***\n");
		SAMPLERLIST* slist = c->model->samplers;
	    for( i = 0 ; i < slist->count ; i++ )
		{
	   		list = slist->items[i]->stochasticdescendant;
			printf("%s (number of stochastic descendants = %d)\n", node_tostring((NODE*)slist->items[i]->snode), list->count);
	   		for( j = 0 ; j < list->count ; j++ )
	    	{
	    		printf("   => %s\n", node_tostring(list->items[j]));
	    	}
		}
		printf("***END DUMP SAMPLERS***\n");
	}
}

void compiler_savehdf(COMPILER* c, FILE* fp, char** monitor, int nmonitor, int burnin, int updates, int thin)
{
    //NODELIST* graph;
    NODELIST* list;
    int i,j,n;
	SAMPLERLIST* slist = c->model->samplers;
	
	// DUMP HDF
	ARRAY_NODE* array;
	CONSTANT_NODE* constant;
    n = c->env->count;
	fprintf(fp,"var {\n");
    for( i = 0 ; i < n ; i++ )
    {
		array = (ARRAY_NODE*)c->env->nodes[i];
        fprintf(fp,"\t%s {\n", env_getenvstring(c->env->names[i]));
        fprintf(fp,"\t\ttype = array\n");
		fprintf(fp,"\t\tsize = %d\n", array_node_getsize(array));
		fprintf(fp,"\t\tdimension {\n");
		for ( j = 0 ; j < ARRAY_MAX_DIMS ; j++ )
		{
			fprintf(fp, "\t\t\t%d = %d\n", j, (int)array->dims[j]);
		}
		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t\titems {\n");
        assert(array->node.nodetype == N_ARRAY);
        int l = array_node_getsize(array);
        for( j = 0 ; j < l ; j++ )
        {
            //constant = (CONSTANT_NODE*)array_node_getnode(array,j);
            constant = (CONSTANT_NODE*)array->node.parents->items[j];
            assert(constant->node.nodetype == N_CONSTANT);
            fprintf(fp,"\t\t\t%d = %f\n", j, constant->value);
        }
        fprintf(fp,"\t\t}\n");
        fprintf(fp,"\t}\n");
    }
	fprintf(fp,"}\n");

	fprintf(fp,"samplers {\n");
	for( i = slist->count-1 ; i >= 0 ; i-- )
	{
		fprintf(fp,"\t%s {\n", env_getenvstring(sampler_getvarname((SAMPLER*)slist->items[i], c->model)));
		fprintf(fp,"\t\tsymbol = %s\n", node_toenvstring(nodedic_findsymbol(c->model->relations, (NODE*)slist->items[i]->snode)));
		fprintf(fp,"\t\ttype = %s\n", sampler_gettypestr((SAMPLER*)slist->items[i]));
		fprintf(fp,"\t\tdistribution = %s\n", distribution_tostring(slist->items[i]->snode->name));
		fprintf(fp,"\t\tmean = %s\n", node_toenvstring(slist->items[i]->snode->node.parents->items[0]));
		fprintf(fp,"\t\tprec = %s\n", node_toenvstring(slist->items[i]->snode->node.parents->items[1]));
		fprintf(fp,"\t\tlogdensity = %s\n", stochastic_node_toenvstring_logdensity(slist->items[i]->snode)); 
		fprintf(fp,"\t\tchildren {\n");

        list = slist->items[i]->stochasticdescendant;
        for( j = 0 ; j < list->count ; j++ )
        {
			fprintf(fp,"\t\t\t%d {\n", j);
			fprintf(fp,"\t\t\t\tsymbol = %s\n", node_toenvstring(nodedic_findsymbol(c->model->relations, (NODE*)list->items[j])));
			fprintf(fp,"\t\t\t\tmean = %s\n", node_toenvstring((NODE*)list->items[j]->parents->items[0])); 
			fprintf(fp,"\t\t\t\tprec = %s\n", node_toenvstring((NODE*)list->items[j]->parents->items[1])); 
			fprintf(fp,"\t\t\t\tlogdensity = %s\n", stochastic_node_toenvstring_logdensity((STOCHASTIC_NODE*)list->items[j])); 
        	fprintf(fp,"\t\t\t}\n");
		}

		fprintf(fp,"\t\t}\n");
		fprintf(fp,"\t}\n");
	}
	fprintf(fp, "}\n");

	fprintf(fp, "method {\n");
	fprintf(fp, "\tburnin = %d\n", burnin);
	fprintf(fp, "\tupdate= %d\n", updates);
	fprintf(fp, "\tthin = %d\n", thin);
	fprintf(fp, "}\n");

    NODE* monitor_nodes[100];
    for( i = 0 ; i < nmonitor ; i++ )
    {
        NODE* node = nodedic_findnode_byliteral(c->model->relations, monitor[i]);
        monitor_nodes[i] = node;
        if( node == NULL )
        {
            printf("cannot find monitoring variable named '%s'\n", monitor[i]);
            exit(99);
        }
    }

	fprintf(fp, "monitors {\n");
    for( i = 0 ; i < nmonitor ; i++ )
    {
        fprintf(fp, "\t%d {\n", i );
        fprintf(fp, "\t\tname= %s\n", monitor[i] );
        fprintf(fp, "\t\tsymbol = %s\n", node_toenvstring(monitor_nodes[i]) );
        fprintf(fp, "\t}\n");
	}
	fprintf(fp, "}\n");
}
 
