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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "compiler.h"
#include "../nmath/nmath.h"
#include <ClearSilver.h>
#include "conf.h"
#include <pthread.h>
#include "chain.h"
#include "config.h"

int mode_verbose = 0;
int mode_codegen = 0;
int mode_thread = 1;
char boot_path[PATH_MAX];
char bin_path[PATH_MAX];

NEOERR* render(void* data, char* s)
{
	FILE* fp = (FILE*)data;

	fprintf(fp, s);
	//printf(s);
	
    return NULL;
}

char* gettemplatedir()
{
	if( mode_verbose ) printf("cd %s .. ", bin_path);
	if( chdir(bin_path) !=0 ) { if( mode_verbose) printf("NG\n"); return NULL; }
	if( mode_verbose ) printf("OK\n");	

	if( mode_verbose ) printf("cd %s .. ", "../");
	if( chdir("../") !=0 ) { if( mode_verbose) printf("NG\n"); return NULL; }
	if( mode_verbose ) printf("OK\n");	
	
	if( mode_verbose ) printf("cd %s .. ", "share");
	if( chdir("share") !=0 ) { if( mode_verbose) printf("NG\n"); return NULL; }
	if( mode_verbose ) printf("OK\n");	
	
	if( mode_verbose ) printf("cd %s .. ", PACKAGE_NAME);
	if( chdir(PACKAGE_NAME) !=0 ) { if( mode_verbose) printf("NG\n"); return NULL; }
	if( mode_verbose ) printf("OK\n");	
	
	if( mode_verbose ) printf("cd %s .. ", "templates");
	if( chdir("templates") !=0 ) { if( mode_verbose) printf("NG\n"); return NULL; }
	if( mode_verbose ) printf("OK\n");	
	
	if( mode_verbose ) printf("cd %s .. ", "ansic");
	if( chdir("ansic") !=0 ) { if( mode_verbose) printf("NG\n"); return NULL; }
	if( mode_verbose ) printf("OK\n");	

	char* buf = GC_MALLOC_ATOMIC(sizeof(char) * 1024);
	if( getcwd(buf, 1024) != NULL )
	{
		return buf;
	}
	return NULL;
}

void savesrc(char* templatedir, char* csfname, char* outfname, HDF* hdf)
{
	CSPARSE* parse;
	NEOERR* err;

	nerr_init();

	chdir(templatedir);
	printf("reading %s ... ", csfname);
    err = cs_init(&parse, hdf);
    if( nerr_match(err, NERR_PASS) ) { printf("NG\n"); nerr_log_error(err); exit(41); } 
	err = cs_parse_file(parse, csfname);
    if( nerr_match(err, NERR_PASS) ) { printf("NG\n"); nerr_log_error(err); exit(42); } 
	printf("OK\n");

	chdir(boot_path);
	chdir(CODEGEN_DIRNAME);
	printf("writing %s ... ", outfname);
	FILE* fp;
	fp = fopen(outfname, "w");
    if( fp != NULL )
	{
		err = cs_render(parse, (void*)fp, render);
		fclose(fp);
    	if( nerr_match(err, NERR_PASS) ) { printf("NG\n"); exit(43); } 
		printf("OK\n");
	}else{
		printf("can't open file");
	}
    cs_destroy(&parse);
}

void saveCODA(CHAIN* chain, int nmonitor, char** monitor_list)
{
	FILE* fp;
	int i, j, n, off;
	char fname[PATH_MAX];
	CHAINCORE* core;

	core  = &chain->core[0];
	fp = fopen("CODAindex.txt", "w");
	if( fp == NULL ) { printf("failed to create new file CODAindex.txt\n"); exit(31); }
	off = 0;
	for( i = 0 ; i < nmonitor; i++ )
	{
		fprintf(fp, "%s\t%d\t%d\n", monitor_list[i], off+1, off+core->monitor_counter);
		off += core->monitor_counter;
	}
	fclose(fp);
   
	for( j = 0 ; j < chain->nchain; j++ )
	{     
		sprintf(fname, "CODAchain%d.txt", j+1);
		fp = fopen(fname, "w");
		if( fp == NULL ) { printf("failed to create new file %s\n", fname); exit(32); }
        
		CHAINCORE* core = &chain->core[j];
        double* monitor_buff = core->monitor_buff;
        int monitor_count = core->monitor_counter;
        for( n = 0 ; n < nmonitor ; n++ )
        {
        	for( i = 0 ; i < monitor_count ; i++ )
        	{
        		fprintf(fp, "%d\t%f\n", i*core->nthin, monitor_buff[i*nmonitor+n]);
        	}
        }
   
		fclose(fp);
	}
}

int main(int argc, char** argv)
{
	char *bugname, *rname_init, *rname_data;
	int nupdate, nburnin,niteration;
	int thin=1;
	int nchain=1;
	int nmonitor=0;
	char* monitor_list[100];
	CHAIN chain;
	int i;

	strcpy(bin_path, argv[0]);
	for( i = strlen(bin_path)-1 ; i >= 0 ; i-- )
	{
		if( bin_path[i] == '/' ){
			bin_path[i] = '\0';
			break;
		}
	}
		

	getcwd(boot_path, PATH_MAX);

	if( argc <= 3 )
	{
		printf("usage: magro [bugs_file] [iteration] [burin-in] <options>\n");
		printf("options: -init=[R-init-file]\n");
		printf("         -data=[R-data-file]\n");
		printf("         -c=[number of chains]\n");
		printf("         -m=[monitoring variable]\n");
		printf("         -thin=[thinning interval]\n");
		printf("         -codegen\n");
		printf("         -st\n");
		printf("         -v\n");
		printf("bugs_file ... the model definition file.\n");
		printf("iteration ... the number of MCMC iterations.\n");
		printf("burn-in ... the number of MCMC burn-in periods.\n");
		printf("-init ... set the initialize parameters file. \n");
		printf("-data ... set the data file.\n");
		printf("-c ... set the number of chains. if you set more than ONE, you SHOULD NOT set -st option.\n");
		printf("-st ... disable multi threading support.\n");
		printf("-codegen ... generate human readable code set.\n");
		printf("-m ... set the monitoring values. (donot set array values)\n");
		printf("-v ... set verbose mode. \n");
		exit(1);
	}

	bugname = argv[1];
	
	if( sscanf(argv[2], "%d", &niteration) != 1 )
	{
		printf("iteration count must be integer.\n");
		exit(2);
	}

	if( sscanf(argv[3], "%d", &nburnin) != 1 )
	{
		printf("burn-in count must be integer.\n");
		exit(3);
	}

	if( niteration < nburnin )
	{
		printf("itertaion[%d] muse be more than burn-in[%d]\n", niteration, nburnin);
		exit(4);
	}
	nupdate = niteration - nburnin;

	rname_data = NULL;
	rname_init = NULL;

	for( i = 1 ; i < argc ; i++ )
	{
		if( argv[i][0] == '-' )
		{
			if( strncmp(argv[i], "-init=", 6) == 0 )
			{
				rname_init = &argv[i][6];
			}
			else if( strncmp(argv[i], "-data=", 6) == 0 )
			{
				rname_data = &argv[i][6];
			}
			else if( strcmp(argv[i], "-st") == 0 )
			{
				mode_thread = 0;
			}
			else if( strcmp(argv[i], "-codegen") == 0 )
			{
				mode_codegen = 1;
			}
			else if( strcmp(argv[i], "-v") == 0 )
			{
				mode_verbose = 1;
			}
			else if( strncmp(argv[i], "-thin=", 6) == 0 )
			{
				if( sscanf(&argv[i][6], "%d", &thin) != 1 )
				{
					printf("option '-thin': value must be integer.\n");
					exit(11);
				}
				if( thin < 1 )
				{
					printf("option '-thin': value must be greater than one.\n");
					exit(12);
				}
			}
			else if( strncmp(argv[i], "-c=", 3) == 0 )
			{
				if( sscanf(&argv[i][3], "%d", &nchain) != 1 )
				{
					printf("option '-c': value must be integer.\n");
					exit(13);
				}
				if( nchain < 1 || nchain>=CHAIN_MAX)
				{
					printf("option '-c': value must be greater than 0 and less than %d.\n", CHAIN_MAX-1);
					exit(14);
				}
			}
			else if( strncmp(argv[i], "-v=", 3) == 0 )
			{
				if( sscanf(&argv[i][3], "%d", &mode_verbose) != 1 )
				{
					printf("option '-v': value must be integer.\n");
					exit(13);
				}
				if( mode_verbose < 1 )
				{
					printf("option '-v': value must be greater than one.");
					exit(14);
				}
				
			}
			else if( strncmp(argv[i], "-m=", 3) == 0 )
			{
				char* p = &argv[i][3];
				int l = strlen(p);
				char* buff = p;//GC_MALLOC(sizeof(char) * l);
				int j,k,n;
				n = 0;
				for( j = 0 ; j < l ; j++ )
				{
					if( p[j] == ',' ) n++;
				}
				nmonitor = n+1;
				n = 0;
				k = 0;
				monitor_list[n++] = p;
				for( j = 0 ; j < l ; j++ )
				{
					if( buff[j] == ',' )
					{
						buff[j] = '\0';
						monitor_list[n++] = buff+j+1;
					}
				}
				buff[l] = '\0';	
			}
		}
	}

	GC_INIT();

	chain_init(&chain, nchain);
	
	int ret;	
	printf("reading model in %s ... ", bugname);
	ret = chain_loadmodel(&chain, bugname);
	if( ret == -1 )
	{
		printf("fail to open the file.\n");
		exit(1);
	}
	if( ret > 0 )
	{
		printf("%d errors found.\n", ret);
		exit(-1);
	}	

	if( rname_data != NULL )
	{
        printf("reading data in %s ... ", rname_data);
		ret = chain_loaddata(&chain, rname_data);
		if( ret != -1 )
        {
            if( ret > 0 )
			{
				printf("%d errors found.\n", ret);
                return -1;
            }
            printf("success\n");
        }
		else
		{
			printf("fail to open the file.\n");
			exit(1);	
		}
	}

	printf("compiling ... ");
	chain_compile(&chain);
	printf("success\n");

	if( mode_verbose > 1 )
	{
		for( i = 0 ; i < nchain ; i++ )
		{
			printf("chain #%d\n", i+1);
			printf("  number of samplers = %d\n", chain.core[i].compiler->model->samplers->count);
			printf("  number of nodes of the graph = %d\n", chain.core[i].compiler->graph->count);
		}
	}

	if( rname_init != NULL )
	{
		printf("reading parameter file in %s ... ", rname_init);
		ret = chain_loadinit(&chain, 0, rname_init);
		if( ret == -1 ) 
		{
			printf("faile to open the file\n");
			exit(1);
		}
		if( ret > 0 )
		{
			printf("%d errors found.\n", ret);
			return -1;
		}
		printf("success\n");
	}

	chain_initialize(&chain);

	int j,n;
    //SAMPLERLIST* slist = c->model->samplers;
    NODE* node;
	NODE* monitor_nodes[100];
	for ( j = 0 ; j < nchain ; j++ )
	{
		chain.core[j].nmonitor = nmonitor;
		for( i = 0 ; i < nmonitor ; i++ )
		{
			CHAINCORE* core = &chain.core[j];
			node = nodedic_findnode_byliteral(core->compiler->model->relations, monitor_list[i]);
			core->monitor_nodes[i] = node;
			if( node == NULL )
			{
				printf("cannot find monitoring variable named '%s'\n", monitor_list[i]);
				exit(99);
			}
		}	
	}

	if( mode_codegen == 0 )
	{
		for( i = 0 ; i < nchain ; i++ )
		{
			int monitor_size = (int)((double)nupdate-1 / thin) + 1;
			double* monitor_buff = GC_MALLOC(sizeof(double) * monitor_size * nmonitor);
//			int monitor_count = 0;
			CHAINCORE* core = &chain.core[i];
	
			core->monitor_buff = monitor_buff;
			core->monitor_counter = 0;
			core->nthin = thin;
		}

		printf("burn-in\n");
		printf("--------------------------------------------------| %d\n", nburnin);
		int ind=0;
		chain_update(&chain, nburnin, 0);
		printf("* 100%%\n");

		ind = 0;
		printf("updates\n");
		printf("--------------------------------------------------| %d\n", nupdate);
		chain_update(&chain, nupdate, 1);
		printf("* 100%%\n");

		//	printf("*** DUMP MONITORTED VALUES***\n");
		saveCODA(&chain, nmonitor, monitor_list);	
/*		
		for( j = 0 ; j < nchain ; j++ )
		{
			CHAINCORE* core = &chain.core[j];
			double* monitor_buff = core->monitor_buff;
			int monitor_count = core->monitor_counter;
			for( n = 0 ; n < nmonitor; n++ )
			{
				printf("%s:\n", monitor_list[n]);
				for( i = 0 ; i < monitor_count; i++ )
				{
					printf("%d\t%f\n", thin*i, monitor_buff[i*nmonitor+n]);
				}
			}
		}
*/
	}
	else if( mode_codegen == 1 )
	{
		DIR* dir = opendir(CODEGEN_DIRNAME);
		if( dir == NULL )
		{
			if( mkdir(CODEGEN_DIRNAME, S_IRWXU | S_IRWXG | S_IRWXO) != 0 )
			{
				printf("failed to make directory named '%s'\n", CODEGEN_DIRNAME);
				exit(90);
			}
		}
		else closedir(dir);

		chdir("codegen");

		printf("writing sampler.hdf ... ");
		FILE* fp = fopen("sampler.hdf", "w");
		if( fp != NULL )
		{
			chain_savehdf(&chain, fp, monitor_list, nmonitor, nburnin, nupdate, thin);
			fclose(fp);
			printf("OK\n");

			printf("reading sampler.hdf ... ");
			fp = fopen("sampler.hdf", "r");
			if( fp != NULL )
			{
				fseek(fp, 0, SEEK_END);
				int sz = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				
				char* buf = (char*)GC_MALLOC_ATOMIC(sz);
				fread(buf, 1, sz, fp);
				fclose(fp); 
				printf("OK\n");

				
				HDF* hdf;
				hdf_init(&hdf);
				printf("parsing hdf ... ");
				hdf_read_string(hdf, buf);
				printf("OK\n");

				char* templatedir = gettemplatedir();
				printf("templatedir = %s\n", templatedir);
				savesrc(templatedir, "main.c.cs", "main.c", hdf);
				savesrc(templatedir, "conf.h.cs", "conf.h", hdf);
				savesrc(templatedir, "nmath.h.cs", "nmath.h", hdf);
				savesrc(templatedir, "sampler.c.cs", "sampler.c", hdf);
				savesrc(templatedir, "sampler.h.cs", "sampler.h", hdf);
				savesrc(templatedir, "environment.c.cs", "environment.c", hdf);
				savesrc(templatedir, "environment.h.cs", "environment.h", hdf);
				
				hdf_destroy(&hdf);

				system("gcc  main.c environment.c sampler.c -L/usr/local/lib -lnmath -I/usr/local/include -W -Wall -lm -pthread");
			}
		}
	}

	chain_free(&chain);
	
	return 0;
}
