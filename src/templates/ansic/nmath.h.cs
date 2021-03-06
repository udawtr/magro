#ifndef NMATH_H
#define NMATH_H

#include <math.h>

#define NaN		0xfff8000000000000
#define NEGINF	0xfff0000000000000
#define POSINF	0x7ff0000000000000
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

typedef struct _NMATH_STATE NMATH_STATE;
struct _NMATH_STATE
{
	struct _RNG_STATE rng;
	struct _RGAMMA_STATE rgamma;
	struct _RBINOM_STATE rbinom;
	struct _GAMMAFN_STATE gammafn;
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

double bd0(double x, double np);
double stirlerr(NMATH_STATE *state, double n);
double lgammafn(NMATH_STATE *state, double x);
double lgammacor(double x);
double gammafn(NMATH_STATE *state, double x);
#endif
