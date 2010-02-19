/*
 *  R DATA NODE: functions for R data node
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

#ifndef __RDATA_NODE_H__
#define __RDATA_NODE_H__

#include <math.h>
#define JAGS_NA	NAN	

typedef enum _RDATA_NODETYPE RDATA_NODETYPE;
enum _RDATA_NODETYPE
{
	RDN_NAME, RDN_ASSIGN, RDN_ARRAY, RDN_VECTOR, RDN_VALUE, RDN_VAR
};

typedef struct _RDATA_NODE RDATA_NODE;
typedef struct _RDATA_NODELIST RDATA_NODELIST;
typedef struct _RDATA_VALUELIST RDATA_VALUELIST;

struct _RDATA_NODE
{
	RDATA_NODETYPE nodetype;
	RDATA_NODELIST* params;
	RDATA_VALUELIST* values;
	char* name;
};

struct _RDATA_NODELIST
{
	int count;
	RDATA_NODE** items;
};

struct _RDATA_VALUELIST
{
	int count;
	double* items;
};

RDATA_NODE* rdata_node_create(RDATA_NODETYPE nodetype);
void rdata_node_setname(RDATA_NODE* pnode, char* name);
void rdata_node_addparam(RDATA_NODE* pnode, RDATA_NODE* param);
void rdata_node_addvalue(RDATA_NODE* pnode, double value);
void rdata_node_free(RDATA_NODE* pnode);
void rdata_node_dump(RDATA_NODE* pnode);

RDATA_NODELIST* rdata_nodelist_create(int count);
void rdata_nodelist_free(RDATA_NODELIST* pnodelist); 
void rdata_nodelist_add(RDATA_NODELIST* pnodelist, RDATA_NODE* pnode);

RDATA_VALUELIST* rdata_valuelist_create(int count);
void rdata_valuelist_free(RDATA_VALUELIST* pvaluelist);
void rdata_valuelist_add(RDATA_VALUELIST* pvaluelist, double value);

#endif

