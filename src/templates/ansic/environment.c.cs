/* environment.c -- definitions for sampling environment

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

#include "conf.h"
#include "environment.h"

struct ENVIRONMENT env[NCHAIN];

void env_init()
{
<?cs each:chain = var ?>
<?cs each:item = chain ?>
<?cs if:item.type == "array" ?>
	<?cs each:index = item.items ?>
	env[<?cs name:chain ?>].<?cs name:item?><?cs if item.size != 1?>[<?cs name:index ?>]<?cs /if?> = <?cs var:index ?>;
	<?cs /each ?>
<?cs /if ?>
<?cs if:item.type == "constant" ?>
	env[<?cs name:chain ?>].<?cs name:item?> = <?cs var:item.value ?>;
<?cs /if ?>
<?cs /each ?>
<?cs /each ?>
}

double env_mean(double* dat, int count)
{
	int i;
	double sum = 0.0;
	for( i = 0 ; i < count ; i++ )
	{
		sum += dat[i];
	}
	return sum/count;
}

double env_sum(double* dat, int count)
{
	int i;
	double sum = 0.0;
	for( i = 0 ; i < count ; i++ )
	{
		sum += dat[i];
	}
	return sum;
}

