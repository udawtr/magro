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

#ifndef __BUGS_NODE_H__
#define __BUGS_NODE_H__

#include <math.h>
#define JAGS_NA	NAN	

typedef enum _BUGS_NODETYPE BUGS_NODETYPE;
enum _BUGS_NODETYPE
{
	BN_NAME, BN_VALUE, BN_VAR, BN_RELATIONS, BN_FOR, BN_COUNTER, BN_DETERMREL, BN_STOCHREL, BN_LINK, BN_FUNC, BN_DIM, BN_LENGTH, BN_RANGE, BN_LIST
};

typedef struct _BUGS_NODE BUGS_NODE;
typedef struct _BUGS_NODELIST BUGS_NODELIST;
typedef struct _BUGS_VALUELIST BUGS_VALUELIST;

struct _BUGS_NODE
{
	BUGS_NODETYPE nodetype;
	BUGS_NODELIST* params;
	BUGS_VALUELIST* values;
	char* name;
};

struct _BUGS_NODELIST
{
	int count;
	BUGS_NODE** items;
};

struct _BUGS_VALUELIST
{
	int count;
	double* items;
};

BUGS_NODE* bugs_node_create(BUGS_NODETYPE nodetype);
void bugs_node_setname(BUGS_NODE* pnode, char* name);
void bugs_node_addparam(BUGS_NODE* pnode, BUGS_NODE* param);
void bugs_node_addvalue(BUGS_NODE* pnode, double value);
void bugs_node_free(BUGS_NODE* pnode);
void bugs_node_dump(BUGS_NODE* pnode);

BUGS_NODELIST* bugs_nodelist_create(int count);
void bugs_nodelist_free(BUGS_NODELIST* pnodelist); 
void bugs_nodelist_add(BUGS_NODELIST* pnodelist, BUGS_NODE* pnode);

BUGS_VALUELIST* bugs_valuelist_create(int count);
void bugs_valuelist_free(BUGS_VALUELIST* pvaluelist);
void bugs_valuelist_add(BUGS_VALUELIST* pvaluelist, double value);

#endif

