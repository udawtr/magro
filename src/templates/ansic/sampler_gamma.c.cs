<?cs
#  sampler_gamma.c.cs -- the template of gamma distribution conjugate sampler 
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
     FOLLOWING CODE IS GENERATED FROM sampler_gamma.c.cs
************************************************************/

//double sampler_<?cs name:sampler ?>_mean = <?cs var:sampler.mean ?>;
//double sampler_<?cs name:sampler ?>_prec = <?cs var:sampler.prec ?>;
double sampler_<?cs name:sampler ?>_coef[NCHAIN][<?cs var:subcount(sampler.children) ?>];
int sampler_<?cs name:sampler ?>_coef_init[NCHAIN];

void sampler_<?cs name:sampler ?>_init()
{
	int i;
	for( i = 0 ; i < NCHAIN ; i++ ) 
		sampler_<?Cs name:sampler ?>_coef_init[i] = 0;
}
void sampler_<?cs name:sampler ?>_term(){}

void sampler_<?cs name:sampler ?>_calCoef(int index)
{
	double xold, val;
	
	xold = <?cs var:sampler.symbol ?>;
<?cs each:child = sampler.children 
?>	sampler_<?cs name:sampler ?>_coef[index][<?cs name:child ?>] = -<?cs var:child.prec ?>;
<?cs /each 
?>
	val = xold + 1.0;
	<?cs var:sampler.symbol ?> = val;
	
<?cs each:child = sampler.children 
?>	sampler_<?cs name:sampler ?>_coef[index][<?cs name:child ?>] += <?cs var:child.prec ?>;
<?cs /each 
?>
	<?cs var:sampler.symbol ?> = xold;
}

void sampler_<?cs name:sampler ?>_update(int index, NMATH_STATE *state)
{
	//const int nchildren = <?cs var:subcount(sampler.children) ?>;

	//Prior
	double r = <?cs var:sampler.mean ?>; // shape
	double mu = <?cs var:sampler.prec ?>; // 1/scale
	
	// likelihood
	double coef_i, Y, ymean, xnew;
	
#ifdef _DEBUG
    printf("*dgamma:  ");
    printf(" r=%%f, mu=%%f", r, mu);
#endif

	if( sampler_<?cs name:sampler ?>_coef_init[index] == 0)
	{	
		sampler_<?cs name:sampler ?>_calCoef(index);
		sampler_<?cs name:sampler ?>_coef_init[index] = 1;
	}

#ifdef _DEBUG
    printf(" nchildren=<?cs var:subcount(sampler.children)?>\n");
#endif

	<?cs each:item = sampler.children ?>
	// child.<?cs name:item ?>
	{
		coef_i = sampler_<?cs name:sampler ?>_coef[index][<?cs name:item ?>];
#ifdef _DEBUG
        printf("  coef[<?cs name:item ?>] = %%f\n", coef_i);
#endif
		if( coef_i > 0 )
		{
			Y = <?cs var:item.symbol ?>;
			
			r += 0.5;
			ymean = <?cs var:item.mean ?>; 
			mu += coef_i * (Y-ymean)*(Y-ymean)/2;
		}
#ifdef _DEBUG
            printf("    Y = %%f, r = %%f, ymean = %%f, mu = %%f\n", Y, r, ymean, mu);
#endif
	}<?cs /each ?>
	
	xnew = rgamma(state, r, 1/mu);
	
	<?cs var:sampler.symbol ?> = xnew;
	
#ifdef _DEBUG
    printf(" post_r=%%f, post_mu=%%f, xnew=%%f\n", r, 1 / mu, xnew);
#endif
}
