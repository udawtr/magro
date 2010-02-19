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

#ifndef __MODEL_H__
#define __MODEL_H__

#include "node.h"
#include "sampler.h"

typedef struct _NODE __NODE;
typedef struct _NODEDIC __NODEDIC;
typedef struct _SYMBOL_NODE __SYMBOL_NODE;
typedef struct _SAMPLERLIST __SAMPLERLIST;
typedef struct _MODEL MODEL;
struct _MODEL {
	__NODEDIC* relations;
	__SAMPLERLIST* samplers;
};

MODEL* model_create();
void model_free(MODEL* m);
void model_addrelation(MODEL* m, __NODE* symbol, __NODE* rel);
void model_addsampler(MODEL* m, SAMPLER* sampler);

#endif

