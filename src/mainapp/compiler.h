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

#ifndef __COMPILER_H__
#define __COMPILER_H__

#include <stdio.h>
#include "env.h"
#include "model.h"
#include "../parser_bugs/bugs_node.h"
#include "../parser_rdata/rdata_node.h"
#include "../nmath/nmath.h"

typedef struct _COMPILER COMPILER;
struct _COMPILER {
	ENV* env;	
	MODEL* model;	
	NODELIST* graph;
};

COMPILER* compiler_create();
void compiler_free(COMPILER* c);
void compiler_compile(COMPILER* c, BUGS_NODE* node);
NODE* compiler_eval(COMPILER* c, BUGS_NODE* node);
void compiler_setrdata(COMPILER* c, RDATA_NODE* rnode);
void compiler_setrinit(COMPILER* c, RDATA_NODE* rnode);
void compiler_savehdf(COMPILER* c, FILE* fp, char** monitor, int nmonitor, int burnin, int updates, int thin);
void compiler_initialize(COMPILER* c, NMATH_STATE *ms);

#endif

