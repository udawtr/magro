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

#ifndef __CHAIN_H__
#define __CHAIN_H__

#include <pthread.h>
#include "compiler.h"
#include "../parser_bugs/bugs_node.h"
#include "../nmath/nmath.h"

#define CHAIN_MAX	100

typedef struct _CHAIN CHAIN;
typedef struct _CHAINCORE CHAINCORE;

struct _CHAINCORE
{
	/* for thread */
	pthread_t threadid;

	/* for iteration */
	int nciter;

	/* for compile */
	COMPILER *compiler;

	/* for monitor */
	int nmonitor;
	int nthin;
	NODE* monitor_nodes[100];	
	double* monitor_buff;
	int monitor_counter;	

	/* for math */
	NMATH_STATE ms;
};

struct _CHAIN
{
	int nchain;
	CHAINCORE core[CHAIN_MAX];
	int niter;
	int minterval;
	BUGS_NODE* _bugsnode;
};

typedef struct _CHAIN_ARG CHAIN_ARG;
struct _CHAIN_ARG
{
    int id;
    CHAIN* chain;
    int fmonitor;
};

void chain_init(CHAIN* chain, int nchain);
int chain_loadmodel(CHAIN* chain, const char* filename);
int chain_loaddata(CHAIN* chain, const char* filename);
int chain_loadinit(CHAIN* chain, int id, const char* filename);
int chain_initialize(CHAIN* chain);
void chain_compile(CHAIN* chain);
void chain_update_main(CHAIN_ARG* arg);
void chain_update_indicator(CHAIN* chain);
void chain_update(CHAIN* chain, int niter, int fmonitor);
void chain_savehdf(CHAIN* chain, FILE* fp, char** monitor, int nmonitor, int burnin, int updates, int thin);

#endif

