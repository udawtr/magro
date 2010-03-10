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
#include <string.h>
#include <assert.h>
#include "env.h"
#include "conf.h"

ENV* env_create(ENV* parent)
{
	int i;
	ENV* env;

	env = (ENV*)GC_MALLOC(sizeof(ENV));
	env->count = 0;
	env->size = 100;
	env->names = (char**)GC_MALLOC(sizeof(char*) * env->size);
	env->nodes = (NODE**)GC_MALLOC(sizeof(NODE*) * env->size);
	env->parent = parent;

	assert(env->names != NULL);
	assert(env->nodes != NULL);

	for( i = 0 ; i < env->size ; i++ )
	{
		env->names[i] = NULL;
		env->nodes[i] = NULL;
	}

//	printf("env_create: address %p\n", env);

	return env;
}

void env_free(ENV* env)
{
	int i, sz;
	assert( env != NULL );

	sz = env->count;
	for( i = 0 ; i < sz ; i++ )
	{
		GC_FREE(env->names[i]);
		node_free(env->nodes[i]);
	}
	GC_FREE(env->names);
	GC_FREE(env->nodes);

	env->names = NULL;
	env->nodes = NULL;

	GC_FREE(env);
}

char* env_getenvstring(const char* name)
{
	int l = strlen(name);
	int sz = sizeof(char) *(l+1);
	char* buff = GC_MALLOC_ATOMIC(sz);
	int i;
	memcpy(buff, name, sz);
	buff[sz-1] = '\0';
	for( i = 0 ; i < l ; i ++ )
	{
		if( buff[i] == '.'  || buff[i] == '[' || buff[i] == ']') buff[i] = '_';
	}
	return buff;
}

NODE* env_find(ENV* env, const char* name)
{
	int index;

	assert(env != NULL);
	assert(name != NULL && strcmp(name, "") != 0);

	index = env_getindex(env, name);
	if( index >= 0 ) 
	{
		//printf("env_find: name=%s found address=%x\n", name, env->nodes[index]);
		NODE_CHECK(env->nodes[index]);
		return env->nodes[index];
	}
	if( env->parent != NULL )
	{
		return env_find(env->parent, name);
	}
	//printf("env_find: name=%s return NULL\n", name);
	return NULL;
}

int env_getindex(ENV* env, const char* name)
{
	int i;

	assert(env != NULL);
	assert(name != NULL);
	assert(env->count >= 0);
	assert(env->names != NULL);

	for( i = 0 ; i < env->count ; i++ )
	{
		//printf("env_getindex: compare '%s'(%d) - '%s'(%d) ", env->names[i], strlen(env->names[i]), name, strlen(name));
		if( strcmp(env->names[i], name) == 0 )
		{
			//printf("EQ\n");
			 return i;
		}
		//printf("NEQ\n");
	}
	return -1;
}

void env_setsymbol(ENV* env, const char* name, NODE* node)
{
	int i, l, sz;
	
	assert(env != NULL);
	assert(name != NULL);
	NODE_CHECK(node);

	l = strlen(name);
	i= env_getindex(env, name);
	if( i>= 0 )
	{
		//redefine the environment value
		NODE* oldnode;
		
		if( env->names[i] != NULL ) GC_FREE(env->names[i]);
		sz = sizeof(char) * (strlen(name)+1);
		env->names[i] = GC_MALLOC_ATOMIC(sz);
		assert(env->names[i] != NULL);
		memcpy(env->names[i], name, sz);
		env->names[i][sz-1] = '\0';
		NODE_CHECK(node);

		oldnode = env->nodes[i];
		env->nodes[i] = node;

		node_free(oldnode);
		node->refcount++;
	}
	else if( env->count < env->size )
	{
		i = env->count;
		env->count = env->count + 1;
		
		sz = sizeof(char) * (strlen(name)+1);
		env->names[i] = GC_MALLOC_ATOMIC(sizeof(char) * (strlen(name)+1));
		assert(env->names[i] != NULL);
		memcpy(env->names[i], name, sz);
		env->names[i][sz-1] = '\0';
		NODE_CHECK(node);
		
		env->nodes[i] = node;
		node->refcount++;
		//char *tmp = node_tostring(node);
		//printf("env_setsymbol: '%s'(%d) = %s\n", name, strlen(name), tmp);
		//GC_FREE(tmp);
	}
	else
	{
		printf("env_setsymbol: buffer overflow.\n");
		exit(1);
	}

	node->refcount++;
}

ENV* env_stackout(ENV* env)
{
	ENV* parent;
	assert(env != NULL);
	
	parent = env->parent;
	env_free(env);
	
	return parent;	
}

ENV* env_stackin(ENV* env)
{
	return env_create(env);	
}

