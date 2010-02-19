<?cs
#  sampler_norm.c.cs -- the template of normal distribution conjugate sampler
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
     FOLLOWING CODE IS GENERATED FROM sampler_norm.c.cs
************************************************************/

//double sampler_<?cs name:sampler ?>_mean = <?cs var:sampler.mean ?>;
//double sampler_<?cs name:sampler ?>_prec = <?cs var:sampler.prec ?>;
double sampler_<?cs name:sampler ?>_beta[NCHAIN][<?cs var:subcount(sampler.children) ?>];
int sampler_<?cs name:sampler ?>_beta_init[NCHAIN];

void sampler_<?cs name:sampler ?>_init()
{
    int i;
    for( i = 0 ; i < NCHAIN ; i++ ) 
        sampler_<?cs name:sampler ?>_beta_init[i] = 0;
}
void sampler_<?cs name:sampler ?>_term(){}

void sampler_<?cs name:sampler ?>_calBeta(int index)
{
	double xold, xnew;
	
	xold = <?cs var:sampler.symbol ?>;

	xnew = xold+1.0;
	<?cs var:sampler.symbol ?> = xnew;

<?cs each:child = sampler.children 
?>	sampler_<?cs name:sampler ?>_beta[index][<?cs name:child ?>] = <?cs var:child.mean ?>;
<?cs /each 
?>
	<?cs var:sampler.symbol ?> = xold;
	
<?cs each:child = sampler.children 
?>	sampler_<?cs name:sampler ?>_beta[index][<?cs name:child ?>] -= <?cs var:child.mean ?>;
<?cs /each 
?>
}

void sampler_<?cs name:sampler ?>_update(int index, NMATH_STATE *state)
{
#ifdef _DEBUG
	const	int nchildren = <?cs var:subcount(sampler.children) ?>;
#endif
	double xold = <?cs var:sampler.symbol ?>;
	double priormean = <?cs var:sampler.mean ?> - xold;
	double priorprec = <?cs var:sampler.prec ?>;
	
	double A = priormean * priorprec;
	double B = priorprec;

<?cs if:subcount(sampler.children) > 0 
?>	double Y,tau, alpha, tau_beta;<?cs /if?>
	double postmean, postsd, xnew;
	
#ifdef _DEBUG
    printf("*dnorm:  ");
    printf(" xold=%%f, priormean=%%f, priorprec=%%f", xold, priormean, priorprec);
#endif
		
	if( sampler_<?cs name:sampler ?>_beta_init[index] == 0)
	{
		sampler_<?cs name:sampler ?>_calBeta(index);
		sampler_<?cs name:sampler ?>_beta_init[index] = 1;
	}

#ifdef _DEBUG
    printf(" nchildren=%%d\n", nchildren);
#endif
	
	<?cs each:item = sampler.children ?>
	// child.<?cs name:item ?>
	{
		Y = <?cs var:item.symbol ?>;
		tau = <?cs var:item.prec ?>;
		alpha = <?cs var:item.mean ?>; 
#ifdef _DEBUG
		printf("  Y=%%f, tau=%%f, alpha=%%f, beta=%%f\n", Y, tau, alpha, sampler_<?cs name:sampler ?>_beta[index][<?cs name:item?>]);
#endif
		tau_beta = tau * sampler_<?cs name:sampler ?>_beta[index][<?cs name:item ?>];
		A += (Y - alpha) * tau_beta;
		B += sampler_<?cs name:sampler ?>_beta[index][<?cs name:item ?>] * tau_beta;
	}<?cs /each ?>
	
	postmean = xold + A/B;
	postsd = sqrt(1/B);
	xnew = rnorm(state, postmean, postsd);
	
	<?cs var:sampler.symbol ?> = xnew;

#ifdef _DEBUG
	printf(" postmean=%%f, postsd=%%f, xnew=%%f\n", postmean, postsd, xnew);
#endif
}
