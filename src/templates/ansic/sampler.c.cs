/* sampler.c -- MCMC samplers genereated from MAGRO 

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

/************************************************************
     FOLLOWING CODE IS GENERATED FROM sampler.c.cs
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include "nmath.h"
#include "environment.h"
#include "sampler.h"
#include "conf.h"

<?cs each:sampler = samplers.0 ?>
<?cs if:sampler.type == "norm" ?><?cs include:"sampler_norm.c.cs" ?>
<?cs elif:sampler.type == "gamma" ?><?cs include:"sampler_gamma.c.cs" ?>
<?cs elif:sampler.type == "real" ?><?cs include:"sampler_real.c.cs" ?>
<?cs /if ?>
<?cs /each ?>

