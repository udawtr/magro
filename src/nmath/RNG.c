#include <stdio.h>
#include "nmath.h"

#define MT(p)	state->rng.p

double norm_rand(NMATH_STATE *state)
{
    double u1;
	double nr;

    const double BIG = 134217728; /* 2^27 */
    /* unif_rand() alone is not of high enough precision */
    u1 = unif_rand(state);
    u1 = (int)(BIG * u1) + unif_rand(state);
    nr = qnorm5(state, u1 / BIG, 0.0, 1.0, 1, 0);
	return nr;
}

double fixup(double x)
{
    const double i2_32m1 = 2.328306437080797e-10; /* = 1/(2^32 - 1) */
    /* ensure 0 and 1 are never returned */
    if (x <= 0.0) return 0.5 * i2_32m1;
    if ((1.0 - x) <= 0.0) return 1.0 - 0.5 * i2_32m1;
    return x;
}

void FixupSeeds(NMATH_STATE *state)
{
    MT(I1) = MT(I1) % 30269; 
	MT(I2) = MT(I2) % 30307;
	MT(I3) = MT(I3) % 30323;

    /* map values equal to 0 mod modulus to 1. */
    if (MT(I1) == 0) MT(I1) = 1;
    if (MT(I2) == 0) MT(I2) = 1;
    if (MT(I3) == 0) MT(I3) = 1;
}

double unif_rand(NMATH_STATE *state)
{
	double value, r;

    MT(I1) = MT(I1) * 171 % 30269;
    MT(I2) = MT(I2) * 172 % 30307;
    MT(I3) = MT(I3) * 170 % 30323;

    value = MT(I1) / 30269.0 + MT(I2) / 30307.0 + MT(I3) / 30323.0;
    r = fixup(value - (int)value);/* in [0,1) */
    return r;
}

void RNG_Init(NMATH_STATE *state, unsigned int seed)
{
	int j;
            
	/* Initial scrambling */
    for (j = 0; j < 50; j++)
    {
    	seed = (69069 * seed + 1);
	}
    seed = (69069 * seed + 1);
    MT(I1) = seed;
    seed = (69069 * seed + 1);
    MT(I2) = seed;
    seed = (69069 * seed + 1);
    MT(I3) = seed;
    FixupSeeds(state);
}

double exponential(NMATH_STATE *state)
{
    /* q[k-1] = sum(log(2)^k / k!)  k=1,..,n, */
    /* The highest n (here 8) is determined by q[n-1] = 1.0 */
    /* within standard precision */
    const double q[] =
    {
	0.6931471805599453,
	0.9333736875190459,
	0.9888777961838675,
	0.9984959252914960,
	0.9998292811061389,
	0.9999833164100727,
	0.9999985691438767,
	0.9999998906925558,
	0.9999999924734159,
	0.9999999995283275,
	0.9999999999728814,
	0.9999999999985598,
	0.9999999999999289,
	0.9999999999999968,
	0.9999999999999999,
	1.0000000000000000
    };
    double a, u, ustar, umin;
    int i;

    a = 0.;
    /* precaution if u = 0 is ever returned */
    u = uniform(state);
    while(u <= 0.0 || u >= 1.0) u = uniform(state);
    for (;;) {
	u += u;
	if (u > 1.0)
	    break;
	a += q[0];
    }
    u -= 1.;

    if (u <= q[0])
	return a + u;

    i = 0;
    ustar = uniform(state);
    umin = ustar;
    do {
	ustar = uniform(state);
	if (ustar < umin)
	    umin = ustar;
	i++;
    } while (u > q[i]);
    return a + umin * q[0];
}

