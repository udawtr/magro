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

#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "node.h"
#include "../nmath/nmath.h"

typedef enum _SAMPLER_TYPE SAMPLERTYPE;
typedef struct _SAMPLER SAMPLER;
typedef struct _SAMPLERLIST SAMPLERLIST;
typedef struct _NORM_SAMPLER NORM_SAMPLER;
typedef struct _GAMMA_SAMPLER GAMMA_SAMPLER;
typedef struct _REAL_SAMPLER REAL_SAMPLER;

typedef struct _STOCHASTIC_NODE __STOCHASTIC_NODE;
typedef struct _NODELIST __NODELIST;
typedef struct _MODEL ___MODEL;

enum _SAMPLER_TYPE
{
	S_UNKNOWN=1, S_NORM, S_GAMMA, S_REAL
};

#define SAMPLER_CHECK(s)	assert(((SAMPLER*)s)->samplertype >= S_UNKNOWN && ((SAMPLER*)s)->samplertype <= S_REAL);

struct _SAMPLER
{
	SAMPLERTYPE samplertype;
	__STOCHASTIC_NODE* snode;
	__NODELIST* stochasticdescendant;
};

struct _SAMPLERLIST
{
	int size;
	int count;
	SAMPLER** items;
};

struct _NORM_SAMPLER
{
	struct _SAMPLER sampler;
	double* beta;
};

struct _GAMMA_SAMPLER
{
	struct _SAMPLER sampler;
	double* coef;
};

struct _REAL_SAMPLER
{
	struct _SAMPLER sampler;
	int width;
	int max;
	double sumdiff;
	int iter;
	int adapt;
};

SAMPLER* sampler_create(__STOCHASTIC_NODE* snode);
void sampler_init(SAMPLER* s);
void sampler_destroy(SAMPLER* s);
void sampler_free(SAMPLER* s);
const char* sampler_gettypestr(SAMPLER* s);
void sampler_update(SAMPLER* s, NMATH_STATE* ms);
double sampler_logfullconditional(SAMPLER* s, NMATH_STATE *ms);
char* sampler_getvarname(SAMPLER* s, ___MODEL* m);

SAMPLERLIST* samplerlist_create();
void samplerlist_free(SAMPLERLIST* list);
void samplerlist_add(SAMPLERLIST* list, SAMPLER* sampler);

SAMPLER* norm_sampler_create(__STOCHASTIC_NODE* snode);
int norm_sampler_cansample(__STOCHASTIC_NODE* snode);
void norm_sampler_free(NORM_SAMPLER* s);
void norm_sampler_calbeta(NORM_SAMPLER* s, double* beta);
void norm_sampler_update(NORM_SAMPLER* s, NMATH_STATE* ms);

SAMPLER* gamma_sampler_create(__STOCHASTIC_NODE* snode);
int gamma_sampler_cansample(__STOCHASTIC_NODE* snode);
void gamma_sampler_free(GAMMA_SAMPLER* s);
double gamma_sampler_getscale(__STOCHASTIC_NODE* snode);
void gamma_sampler_calcoef(GAMMA_SAMPLER* s, double* coef);
void gamma_sampler_update(GAMMA_SAMPLER* s, NMATH_STATE* ms);

SAMPLER* real_sampler_create(__STOCHASTIC_NODE* snode);
int real_sampler_cansample(__STOCHASTIC_NODE* snode);
void real_sampler_free(REAL_SAMPLER* s);
void real_sampler_update(REAL_SAMPLER* s, NMATH_STATE* ms);

#endif
