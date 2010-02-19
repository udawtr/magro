/* conf.h -- configuration header file  

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

#ifndef __CONF_H__

#define NBURNIN		<?cs var:method.burnin?>
#define NUPDATE		<?cs var:method.update ?>
#define NTHIN		<?cs var:method.thin ?>
#define NCHAIN		<?cs var:method.chain ?>
#define NMONITOR	<?cs var:subcount(monitors) ?>

#define MINTERVAL	100

#define MODE_THREAD	<?cs var:method.mode_thread ?>

#endif

