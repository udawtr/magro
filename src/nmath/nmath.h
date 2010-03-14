/*
 *  NMathlib : A C Library of Special Functions
 *  Copyright (C) 2010 Wataru Uda
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef NMATH_H
#define NMATH_H

#include <math.h>

#ifndef __VC
#define NaN		0xfff8000000000000
#define NEGINF	0xfff0000000000000
#define POSINF	0x7ff0000000000000
#else
#define NAN	NaN
#define INFINITY POSINF
extern volatile double NaN;
extern volatile double NEGINF;
extern volatile double POSINF;
#include <float.h>
#define isnan(x)	_isnan(x)
#define isinf(x)	(!_finite(x))
#define isfinite(x)	_finite(x)
#endif
#define uniform unif_rand

struct _RGAMMA_STATE
{
   double aa ;//= 0.0;
   double aaa;// = 0.0;
   double s, s2, d;    /* no. 1 (step 1) */
   double q0, b, si, c;/* no. 2 (step 4) */
};

struct _RBINOM_STATE
{
    double c, fm, npq, p1, p2, p3, p4, qn;
    double xl, xll, xlr, xm, xr;
    double psave;// = -1.0;
    int nsave;// = -1;
    int m;	
};

struct _RNG_STATE
{
	unsigned int I1, I2, I3;
};

struct _GAMMAFN_STATE
{
    int ngam;
    double xmin, xmax, xsml, dxrel;
};

struct _RPOIS_STATE
{
    int l, m;

    double b1, b2, c, c0, c1, c2, c3;
    double pp[36], p0, p, q, s, d, omega;
    double big_l;/* integer "w/o overflow" */
    double muprev, muprev2;/*, muold    = 0.*/
};

typedef struct _NMATH_STATE NMATH_STATE;
struct _NMATH_STATE
{
	struct _RNG_STATE rng;
	struct _RGAMMA_STATE rgamma;
	struct _RBINOM_STATE rbinom;
	struct _GAMMAFN_STATE gammafn;
	struct _RPOIS_STATE rpois;
};

double rgamma(NMATH_STATE *state, double a, double scale);
double rnorm(NMATH_STATE *state, double mu, double sigma);
double qnorm5(NMATH_STATE *state, double p, double mu, double sigma, int lower_tail, int log_p);
double lgammafn(NMATH_STATE *state, double x);
double rbinom(NMATH_STATE *state, double nin, double pp);
double dgamma(NMATH_STATE *state, double x, double shape, double scale, int give_log);

double dpois_raw(NMATH_STATE *state, double shape, double scale, int give_log);
double dbinom_raw(NMATH_STATE *state, double x, double n, double p, double q, int give_log);
double dbinom(NMATH_STATE *state, double x, double n, double p, int give_log);

double dbeta(NMATH_STATE *state, double x, double a, double b, int give_log);
double rbeta(NMATH_STATE *state, double aa, double bb);
double lbeta(NMATH_STATE *state, double a, double b);

double rchisq(NMATH_STATE *state, double df);
double dchisq(NMATH_STATE *state, double x, double df, int give_log);
double dnbinom(NMATH_STATE *state, double x, double n, double p, int give_log);
double rnbinom(NMATH_STATE *state, double n /* size */, double p /* prob */);
double dpois(NMATH_STATE *state, double x, double lambda, int give_log);
double rpois(NMATH_STATE *state, double mu);
double dexp(NMATH_STATE *state, double x, double scale, int give_log);
double rexp(NMATH_STATE *state, double scale);
double dweibull(NMATH_STATE *state, double x, double shape, double scale, int give_log);
double rweibull(NMATH_STATE *state, double shape, double scale);
double dunif(NMATH_STATE *state, double x, double a, double b, int give_log);
double runif(NMATH_STATE *state, double a, double b);

#define dnorm dnorm4
double dnorm4(NMATH_STATE *state, double x, double mu, double sigma, int give_log);

double expm1(double x);
double log1p(double x);

int chebyshev_init(double dos[], int nos, double eta);
double chebyshev_eval(double x, double a[], int n);

double exp_rand(NMATH_STATE *state);
double norm_rand(NMATH_STATE *state);
double unif_rand(NMATH_STATE *state);
double exponential(NMATH_STATE *state);
void RNG_Init(NMATH_STATE *state, unsigned int seed);
void NMath_Init(NMATH_STATE *state);

double fmax2(double x, double y);
double fmin2(double x, double y);
int imax2(int x, int y);
int imin2(int x, int y);
double fsign(double x, double y);
double bd0(double x, double np);
double stirlerr(NMATH_STATE *state, double n);
double lgammafn(NMATH_STATE *state, double x);
double lgammacor(double x);
double gammafn(NMATH_STATE *state, double x);
#endif
