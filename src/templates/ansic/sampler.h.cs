/* sampler.h -- include file for sampler.c

   Copyright 1992, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2005,
   2006, 2007, 2008, 2010 Free Software Foundation, Inc.

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

#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include "nmath.h"

<?cs each:sampler = samplers.0 ?><?cs 
if:sampler.type == "norm" ?><?cs include:"sampler_norm.h.cs" ?><?cs /if ?><?cs
if:sampler.type == "gamma" ?><?cs include:"sampler_gamma.h.cs" ?><?cs /if ?><?cs 
if:sampler.type == "real" ?><?cs include:"sampler_real.h.cs" ?><?cs /if ?>
<?cs /each ?>
#endif

