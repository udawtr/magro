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
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "chain.h"
#include "conf.h"
#include "../nmath/nmath.h"

int rdataparse();
int bugsparse();

void chain_init(CHAIN* chain, int nchain)
{
	int i,t;

	assert(chain!=NULL);
	assert(nchain<=CHAIN_MAX);

	chain->nchain = nchain;
	t = (int)time(NULL);
	for( i = 0 ; i < nchain ; i++, t+=327)
	{
		CHAINCORE* core = &chain->core[i];
		core->threadid = NULL;
		core->nciter = 0;
		core->compiler = compiler_create();
		NMath_Init(&core->ms);
		RNG_Init(&core->ms, t);
	}	
	chain->niter = 0;
	chain->minterval = 100;
}

int chain_loadmodel(CHAIN* chain, const char* filename)
{
	FILE* fp;
	int i;

    fp = fopen(filename, "r");
    if( fp != NULL )
    {
        extern FILE* bugsin;
        bugsin = fp;
        extern BUGS_NODE* g_bugsnode;
    	extern int bugsnerrs;

        bugsparse();
        if( bugsnerrs > 0 )
        {
            return bugsnerrs;
        }
		fclose(fp);

        if( mode_verbose > 1)
            bugs_node_dump(g_bugsnode);
    
		chain->_bugsnode = g_bugsnode;
	}
    else
    {
		return -1;
    }
}

int chain_loaddata(CHAIN* chain, const char* filename)
{
	FILE* fp;
	int i;

	fp = fopen(filename, "r");
    if( fp != NULL )
    {   
    	extern FILE *rdatain;
        extern RDATA_NODE* g_rdatanode;
    	extern int rdatanerrs;
        rdatain = fp; 

		assert(chain->core[0].compiler->env->names != NULL);
        rdataparse();
        if( rdatanerrs > 0 ) 
        {   
        	printf("%d errors found.\n", rdatanerrs);
            return rdatanerrs; 
        }   
        fclose(fp);

		assert(chain->core[0].compiler->env->names != NULL);
        
		for( i = 0 ; i < chain->nchain ; i++ )
		{
			compiler_setrdata(chain->core[i].compiler, g_rdatanode);
       	}

		return 0;
	} 
	return -1;
}

int chain_loadinit(CHAIN* chain, int id, const char* filename)
{
	FILE* fp;

    fp = fopen(filename, "r");
    if( fp != NULL )
    {
    	extern FILE *rdatain;
    	extern RDATA_NODE* g_rdatanode;
    	extern int rdatanerrs;
    	rdatain = fp;

        rdataparse();
        if( rdatanerrs > 0 )
        {
            return rdatanerrs;
        }
        fclose(fp);
        
		compiler_setrinit(chain->core[id].compiler, g_rdatanode);
        
		return 0;
	}
        
	return -1;
}

int chain_initialize(CHAIN* chain)
{
	int i;
	for( i = 0 ; i < chain->nchain ; i++ )
	{
		CHAINCORE *core = &chain->core[i];
		//compiler_initialize(core->compiler, &core->ms);
	}
}

void chain_compile(CHAIN* chain)
{
	int i;
	for( i = 0 ; i < chain->nchain ; i++ )
	{
		COMPILER* c = chain->core[i].compiler; 
		compiler_compile(c, chain->_bugsnode);
	}
}

void chain_update_main_core(CHAIN_ARG* arg, int n)
{
	CHAIN* chain = arg->chain;
	CHAINCORE* core = &arg->chain->core[arg->id];
	COMPILER* c = core->compiler;
    SAMPLERLIST* slist = c->model->samplers;

	int i;
    for( i = slist->count-1 ; i >= 0 ; i-- )
    {
    	if( mode_verbose>2) printf("Sampling for %s[chain:%d]\n", sampler_getvarname(slist->items[i], c->model), arg->id );
    	sampler_update(slist->items[i], &core->ms);
    }
	core->nciter++;

    if( arg->fmonitor == 1 && n% core->nthin == 0 )
    {
    	for( i = 0 ; i < core->nmonitor ; i++ )
        {
        	core->monitor_buff[core->monitor_counter * core->nmonitor+ i] = node_getvalue(core->monitor_nodes[i]);
        }
        core->monitor_counter++;
	}
}

void chain_update_main(CHAIN_ARG* arg)
{
	CHAIN* chain = arg->chain;
	CHAINCORE* core = &arg->chain->core[arg->id];
	COMPILER* c = core->compiler;
    SAMPLERLIST* slist = c->model->samplers;

	int i, n;
	for( n = 0 ; n < chain->niter ; n++ )
	{
		chain_update_main_core(arg, n);
	}
}

int chain_update_indicator_core(CHAIN* chain, int *ind)
{
	int i, sum=0;
	int goal = chain->niter*chain->nchain;
	
	sum = 0;
	for ( i = 0 ; i < chain->nchain ; i ++ )
	{
		sum += chain->core[i].nciter;
	}	

	while( sum*50/goal - *ind >= 1 )
	{
		 (*ind)++;
		 printf("*");
	}
	fflush(stdout);

	return sum;
}

void chain_update_indicator(CHAIN* chain)
{
	int i, sum=0;
	int ind = 0;
	int goal = chain->niter*chain->nchain;

	while(1)
	{
		sum = chain_update_indicator_core(chain, &ind);
		if( sum < goal )
			usleep(chain->minterval);
		else
			break;
	}
}

void chain_update(CHAIN* chain, int niter, int fmonitor)
{
	int i;
	CHAIN_ARG args[CHAIN_MAX];
	pthread_t mthreadid;

	for( i = 0 ; i < chain->nchain; i++ )
	{
		assert(chain->core[i].compiler != NULL);	
		chain->core[i].nciter = 0;
		args[i].chain = chain;
		args[i].id = i;
		args[i].fmonitor = fmonitor;
	}
	chain->niter = niter;

	if( mode_thread == 0 )
	{
		//single thread mode
		int ind = 0, n;
		for( n = 0 ; n < chain->niter ; n++ )
		{
			for( i = 0 ; i < chain->nchain; i++ )
			{
				chain_update_main_core(&args[i], n);
			}
			chain_update_indicator_core(chain, &ind);
		}
		chain_update_indicator_core(chain, &ind);
	}
	else
	{
		//multi thread mode
		for( i = 0 ; i < chain->nchain; i++ )
		{
			int r;
			r = pthread_create(&chain->core[i].threadid, NULL, (void*)chain_update_main, (void*)&args[i]);
			assert(r==0);
		}
		pthread_create(&mthreadid, NULL, (void*)chain_update_indicator, (void*)chain);

		void* ret;
		for( i = 0 ; i < chain->nchain; i++ )
		{
			pthread_join(chain->core[i].threadid, &ret);
		}
		pthread_join(mthreadid, &ret);
	}
}

void chain_savehdf(CHAIN* chain, FILE* fp, char** monitor, int nmonitor, int burnin, int updates, int thin)
{
    //NODELIST* graph;
    NODELIST* list;
    int i,j,k,n;
	COMPILER* c = chain->core[0].compiler;
    SAMPLERLIST* slist = c->model->samplers;

    // DUMP HDF
    ARRAY_NODE* array;
    CONSTANT_NODE* constant;
    n = c->env->count;
    fprintf(fp,"var {\n");
	for( k = 0 ; k < chain->nchain ; k++ )
	{
		fprintf(fp, "\t%d {\n", k);
    	for( i = 0 ; i < n ; i++ )
    	{
    	    array = (ARRAY_NODE*)c->env->nodes[i];
    	    fprintf(fp,"\t\t%s {\n", env_getenvstring(c->env->names[i]));
    	    fprintf(fp,"\t\t\ttype = array\n");
    	    fprintf(fp,"\t\t\tsize = %d\n", array_node_getsize(array));
    	    fprintf(fp,"\t\t\tdimension {\n");
    	    for ( j = 0 ; j < ARRAY_MAX_DIMS ; j++ )
    	    {
    	        fprintf(fp, "\t\t\t\t%d = %d\n", j, (int)array->dims[j]);
    	    }
    	    fprintf(fp,"\t\t\t}\n");
    	    fprintf(fp,"\t\t\titems {\n");
    	    assert(array->node.nodetype == N_ARRAY);
    	    int l = array_node_getsize(array);
    	    for( j = 0 ; j < l ; j++ )
    	    {
    	        //constant = (CONSTANT_NODE*)array_node_getnode(array,j);
    	        constant = (CONSTANT_NODE*)array->node.parents->items[j];
    	        assert(constant->node.nodetype == N_CONSTANT);
    	        fprintf(fp,"\t\t\t\t%d = %f\n", j, constant->value);
    	    }
    	    fprintf(fp,"\t\t\t}\n");
    	    fprintf(fp,"\t\t}\n");
    		//printf("savehdf: '%s'=>'%s' size=%d\n", c->env->names[i], env_getenvstring(c->env->names[i]), l);
		}
		for( i = 0 ; i < slist->count ; i++ )
		{
			MODEL *m = NP(slist->items[i]->snode)->model;
			SYMBOL_NODE* sym = (SYMBOL_NODE*)nodedic_findsymbol(
					m->relations,
					(NODE*)slist->items[i]->snode
			);
			if( env_getindex(c->env, sym->name) < 0 )
			{
				fprintf(fp, "\t\t%s {\n\t\t\ttype = constant\n\t\t\tvalue = %f\n\t\t}\n", 
					sampler_getvarname(slist->items[i], m),
					stochastic_node_getvalue(slist->items[i]->snode)
				 );
			}
		}
    	fprintf(fp,"\t}\n");
	}
    fprintf(fp,"}\n");
    fprintf(fp,"samplers {\n");
    for( k = 0 ; k < chain->nchain ; k++ )
	{
		fprintf(fp, "\t%d {\n", k);
		for( i = slist->count-1 ; i >= 0 ; i-- )
    	{
       		fprintf(fp,"\t\t%s {\n", env_getenvstring(sampler_getvarname((SAMPLER*)slist->items[i], c->model)));
        	fprintf(fp,"\t\t\tsymbol = %s\n", node_toenvstring(nodedic_findsymbol(c->model->relations, (NODE*)slist->items[i]->snode)));
        	fprintf(fp,"\t\t\ttype = %s\n", sampler_gettypestr((SAMPLER*)slist->items[i]));
        	fprintf(fp,"\t\t\tdistribution = %s\n", distribution_tostring(slist->items[i]->snode->name));
        	fprintf(fp,"\t\t\tmean = %s\n", node_toenvstring(slist->items[i]->snode->node.parents->items[0]));
        	fprintf(fp,"\t\t\tprec = %s\n", node_toenvstring(slist->items[i]->snode->node.parents->items[1]));
        	fprintf(fp,"\t\t\tlogdensity = %s\n", stochastic_node_toenvstring_logdensity(slist->items[i]->snode));
        	fprintf(fp,"\t\t\tchildren {\n");

        	list = slist->items[i]->stochasticdescendant;
        	for( j = 0 ; j < list->count ; j++ )
        	{
        	    fprintf(fp,"\t\t\t\t%d {\n", j);
        	    fprintf(fp,"\t\t\t\t\tsymbol = %s\n", node_toenvstring(nodedic_findsymbol(c->model->relations, (NODE*)list->items[j])));
        	    fprintf(fp,"\t\t\t\t\tmean = %s\n", node_toenvstring((NODE*)list->items[j]->parents->items[0]));
        	    fprintf(fp,"\t\t\t\t\tprec = %s\n", node_toenvstring((NODE*)list->items[j]->parents->items[1]));
        	    fprintf(fp,"\t\t\t\t\tlogdensity = %s\n", stochastic_node_toenvstring_logdensity((STOCHASTIC_NODE*)list->items[j]));
        	    fprintf(fp,"\t\t\t\t}\n");
        	}

        	fprintf(fp,"\t\t\t}\n");
        	fprintf(fp,"\t\t}\n");
    	}
		fprintf(fp,"\t}\n");
	}
    fprintf(fp, "}\n");

    fprintf(fp, "method {\n");
    fprintf(fp, "\tburnin = %d\n", burnin);
    fprintf(fp, "\tupdate= %d\n", updates);
    fprintf(fp, "\tthin = %d\n", thin);
    fprintf(fp, "\tchain = %d\n", chain->nchain);
    fprintf(fp, "\tmode_thread = %d\n", mode_thread);
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
