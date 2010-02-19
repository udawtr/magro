/* environment.h -- header file of environment.c 

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

#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "conf.h"

/* declaration of each chain's variables set */
struct ENVIRONMENT
{<?cs
each:item = var.0 ?><?cs 
if:item.type == "array" 
?>	double <?cs name:item ?><?cs 
	if:item.size != 1 ?>[<?cs var:item.size ?>]<?cs /if?>;<?cs 
/if?><?cs 
if:item.type == "constant" 
?>	double <?cs name:item ?>;<?cs /if?>
<?cs /each 
?>};

/* declaration of instance  */
extern struct ENVIRONMENT env[NCHAIN];

void env_init();
double env_mean(double* dat, int count);

#endif
