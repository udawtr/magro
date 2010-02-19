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

#ifndef __CONF_H__
#define __CONF_H__

#include <limits.h>
#include <stdlib.h>

#define CODEGEN_DIRNAME	"codegen"

extern int mode_verbose;
extern int mode_codegen;
extern int mode_thread;
extern char boot_path[PATH_MAX];

//#define ENABLE_GC	
#ifdef ENABLE_GC
#include <gc.h>
#endif

#ifndef GC_MALLOC
#define GC_INIT()
#define GC_MALLOC malloc
#define GC_MALLOC_ATOMIC malloc
#define GC_FREE free
#endif

#endif
