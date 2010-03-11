<?cs
#  sampler_real.c.cs -- the template of real-value slice sampler
#
#  Copyright 2010, Wataru Uda.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
?>
/************************************************************
     FOLLOWING CODE IS GENERATED FROM sampler_real.c.cs
************************************************************/

const int sampler_<?cs name:sampler ?>_width = 1.0;
const int sampler_<?cs name:sampler ?>_max = 10; 

void sampler_<?cs name:sampler ?>_init(){}
void sampler_<?cs name:sampler ?>_term(){}

double sampler_<?cs name:sampler ?>_logfullconditional(int index, NMATH_STATE *state)
{
    double lprior, llike, lfc;
   
	lprior = <?cs var:sampler.logdensity ?>;

	llike = 0.0;
<?cs each:child = sampler.children
?>	llike += <?cs var:child.logdensity ?>;
<?cs /each
?>

    lfc = lprior + llike;
    return lfc;
}

void sampler_<?cs name:sampler ?>_update(int index, NMATH_STATE *state)
{
	double g, g0, z, xold, xnew, L, R;
	int i,j,k;
	
	g0 = sampler_<?cs name:sampler ?>_logfullconditional(index,state);
	z = g0 - exponential(state);

	xold = <?cs var:sampler.symbol ?>; 
	L = xold - uniform(state) * sampler_<?cs name:sampler ?>_width;
	R = L + sampler_<?cs name:sampler?>_width;

#ifdef _DEBUG
	printf("*reals: g0=%%f, z=%%f, xold=%%f, L=%%f, R=%%f\n", g0, z, xold, L, R);
#endif

	j = (int)(uniform(state) * sampler_<?cs name:sampler ?>_max);
	k = sampler_<?cs name:sampler ?>_max - 1 - j;

	<?cs var:sampler.symbol ?> = L;
	while(j-- > 0 && sampler_<?cs name:sampler ?>_logfullconditional(index, state) > z) {
		L -= sampler_<?cs name:sampler ?>_width;
		<?cs var:sampler.symbol ?> = L;
	}

	<?cs var:sampler.symbol ?> = R;
	while( k-- > 0 && sampler_<?cs name:sampler ?>_logfullconditional(index, state) > z) {
		R += sampler_<?cs name:sampler ?>_width;
		<?cs var:sampler.symbol ?> = R;
	}

	// Keep sampling from the interval until acceptance ( the loop is guaranteed to terminate.)
	for(;;){
		xnew = L + uniform(state) * (R-L);
		<?cs var:sampler.symbol ?> = xnew;
		g = sampler_<?cs name:sampler ?>_logfullconditional(index, state);
#ifdef _DEBUG
		printf("      L=%%f, R=%%f, g=%%f, xnew=%%f\n",  L, R, g, xnew);
#endif
		if( g >= z - DBL_EPSILON ) {
			//accept point
			break;
		}else{
			// shrink the interval
			if( xnew < xold ) {
				L = xnew;
			}else{
				R = xnew;
			}
		}
	}
}
