/* main.c -- MAGRO generated MCMC sampler's main

   Copyright 2010, Wataru Uda.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "nmath.h"
#include "environment.h"
#include "sampler.h"
#include "conf.h"

pthread_t threadid[NCHAIN];
pthread_t mthreadid;
NMATH_STATE ms[NCHAIN];
int counter[NCHAIN];
double monitor_buff[NCHAIN][NMONITOR][NUPDATE/NTHIN];
int monitor_count[NCHAIN];
int indicator_state = 0;

void chain_update_main_core(int index, int n)
{
<?cs each:item = samplers.0 
?>	sampler_<?cs name:item ?>_update(index, &ms[index]);
<?cs /each 
?><?cs 
if:subcount(monitors) > 0 
?>
	if( n > NBURNIN && n%% NTHIN == 0 )
    {
		int off = (n-NBURNIN)/NTHIN;<?cs 
		each:mon = monitors ?>
		monitor_buff[index][<?cs name:mon?>][off] = <?cs var:mon.symbol  ?>;<?cs
 /each ?>
    }
<?cs /if 
?>}

void chain_update_main(void* arg)
{
    int i, n;
	i = *(int*)arg;
    for( n = 0 ; n < NBURNIN + NUPDATE; n++ )
    {
        chain_update_main_core(i, n);
		monitor_count[i]++;
    }
}

int chain_update_indicator_core(int *ind)
{
    int i, sum;
    int goal;

    sum = 0;
    for ( i = 0 ; i < NCHAIN ; i ++ )
    {
        sum += monitor_count[i];
    }
    
	if( indicator_state == 0 )
	{
    	printf("burn-in\n");
    	printf("--------------------------------------------------| %%d\n", NBURNIN);
    	indicator_state++;
	}
	else if( indicator_state == 1 )
	{
		goal = NBURNIN * NCHAIN;
		while( sum*50/goal - *ind >= 1 && *ind<50)
    	{
    	     (*ind)++;
    	     printf("*");
    	}
    	fflush(stdout);
		if( *ind >= 50 )
			indicator_state++;
	}
	
	if( indicator_state == 2 )
	{
		printf("* 100%%%%\n");
    	printf("update\n");
    	printf("--------------------------------------------------| %%d\n", NUPDATE);
		indicator_state++;
		*ind=0;
	}
	else if( indicator_state == 3 )
	{
		goal = (NUPDATE) * NCHAIN;
		while( (sum-NBURNIN*NCHAIN)*50/goal - *ind >= 1 )
    	{
    	     (*ind)++;
    	     printf("*");
    	}
    	fflush(stdout);
		if( *ind >= 49 )
		{
			printf("** 100%%%%\n");
			indicator_state++;
		}
	}
	return sum;
}

void chain_update_indicator(void* arg)
{
    int sum;
    int ind = 0;
    int goal = (NUPDATE+NBURNIN)*NCHAIN;

    while(1)
    {
        sum = chain_update_indicator_core(&ind);
        if( sum < goal )
            usleep(MINTERVAL);
        else
            break;
    }
}

void do_iteration()
{<?cs 
if:method.mode_thread == 0 ?>
	int i, ind = 0, n;
	for( n = 0 ; n < NBURNIN+NUPDATE ; n++ )
	{
		for( i = 0 ; i < NCHAIN; i++ )
		{
			chain_update_main_core(i, n);
			monitor_count[i] = n;
		}
		chain_update_indicator_core(&ind);
	}
	chain_update_indicator_core(&ind);
<?cs else ?>	
	int i, index[NCHAIN];
	for( i = 0 ; i < NCHAIN; i++ )
	{
		int r;
		index[i] = i;
		monitor_count[i] = 0;
		r = pthread_create(&threadid[i], NULL, (void*)chain_update_main, (void*)&index[i]);
		//assert(r==0);
	}
	pthread_create(&mthreadid, NULL, (void*)chain_update_indicator, (void*)NULL);

	void* ret;
	for( i = 0 ; i < NCHAIN ; i++ )
	{
		pthread_join(threadid[i], &ret);
	}
	pthread_join(mthreadid, &ret);
<?cs /if
?>}

void saveCODA()
{
    FILE* fp;
    int i, j, n, off;
	const int len = NUPDATE / NTHIN;
    char fname[255];

    fp = fopen("CODAindex.txt", "w");
    if( fp == NULL ) { printf("failed .c create new file CODAindex.txt\n"); exit(31); }
    off = 0;
<?cs each:item = monitors 
?>	fprintf(fp, "<?cs var:item.name  ?>\t%%d\t%%d\n", off+1, off+len);
    off += len;
<?cs /each ?>
    fclose(fp);

    for( j = 0 ; j < NCHAIN ; j++ )
    {
        sprintf(fname, "CODAchain%%d.txt", j+1);
        fp = fopen(fname, "w");
        if( fp == NULL ) { printf("failed .c create new file %%s\n", fname); exit(32); }

        for( n = 0 ; n < NMONITOR ; n++ )
        {
            for( i = 0 ; i < len ; i++ )
            {
                fprintf(fp, "%%d\t%%f\n", i*NTHIN, monitor_buff[j][n][i]);
            }
        }

        fclose(fp);
    }
}

void samplers_init()
{
<?cs each:item = samplers.0 ?>	sampler_<?cs name:item ?>_init();
<?cs /each?>
}

void samplers_term()
{
<?cs each:item = samplers.0 ?>	sampler_<?cs name:item ?>_term();
<?cs /each?>
}

int main(int argc, char* argv[])
{
	int i,index,t;

	env_init();

	//Init NMath
	t = (int)time(NULL);
	for( i = 0 ; i < NCHAIN ; i++, t+=371 ){
		NMath_Init(&ms[i]);
		RNG_Init(&ms[i], t);
	}

	samplers_init();
	do_iteration();
	saveCODA();

	return 0;
}
