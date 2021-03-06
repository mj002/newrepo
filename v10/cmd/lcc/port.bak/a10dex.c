/* a10dex.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

extern struct {
    real wv[30], rv[30];
    integer iv[40];
    logical lv[20];
} a10dey_;

#define a10dey_1 a10dey_

extern struct {
    real tj, dtj;
    logical getjac, sepate, usengj, usenns, usenfd;
} a1odeg_;

#define a1odeg_1 a1odeg_

extern struct {
    real theta, egive;
    integer minit, maxit, keejac, ircs;
} a1odem_;

#define a1odem_1 a1odem_

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int a10dex_(tstart, tstop, xa, f, da, d, beta, gamma, delta, 
	x, nx, dt, n, kmax, mmax, xpoly, serror, error, errpar, inmi, sout, 
	output, pespar, hfract, kinit)
real *tstart, *tstop;
/* Subroutine */ int (*xa) (), (*f) (), (*da) (), (*d) ();
real *beta, *gamma, *delta, *x;
integer *nx;
real *dt;
integer *n, *kmax, *mmax;
logical *xpoly;
logical (*serror) ();
/* Subroutine */ int (*error) ();
real *errpar;
/* Subroutine */ int (*inmi) (), (*sout) (), (*output) ();
real *pespar, *hfract;
integer *kinit;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
#define done ((logical *)&a10dey_1 + 106)
    static integer nerr, m;
    extern /* Subroutine */ int leave_(), enter_();
#define t0 ((real *)&a10dey_1 + 9)
#define t1 ((real *)&a10dey_1 + 10)
    extern logical a4ssoe_(), a4ssoi_(), a4ssom_(), a4ssor_();
#define ie ((integer *)&a10dey_1 + 81)
    extern logical a4ssox_();
#define ok ((logical *)&a10dey_1 + 101)
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((real *)&cstak_1)
    extern integer nerror_();
#define ix1 ((integer *)&a10dey_1 + 71)

/* SCRATCH SPACE ALLOCATED - */
/*     S(A10DEX) <= 2*MMAX + 1 + NX*(KMAX+1) + */
/*     ( 5*KMAX + 2*MMAX + 3 ) INTEGER + */
/*     MAX ( S(XA), NX*(KMAX+1) REAL + */
/*           MAX ( KMAX + KMAX INTEGER, S(SERROR) ), */
/*           NX REAL + S(SOUT) ) */
/* REAL. */
/* THE PORT LIBRARY STACK AND ITS ALIASES. */
    /* Parameter adjustments */
    --errpar;
    --n;
    --x;

    /* Function Body */
    enter_(&c__1);
    if (! a4ssoi_(a10dey_1.wv, a10dey_1.rv, a10dey_1.iv, a10dey_1.lv, tstart, 
	    tstop, beta, gamma, delta, nx, dt, &n[1], kmax, mmax, xpoly, &
	    errpar[1], pespar, hfract, kinit)) {
	goto L1;
    }
    leave_();
    return 0;
L1:
    if (*t0 == *tstop) {
	goto L7;
    }
/* TAKE THE TIME-STEPS. */
    if (a1odem_1.keejac != 2) {
	goto L2;
    }
    a1odeg_1.getjac = *t0 != a1odeg_1.tj;
    a1odeg_1.tj = *t0;
/* BUILD THE EXTRAPLOATION LOZENGE. */
L2:
    i__1 = *mmax;
    for (m = 1; m <= i__1; ++m) {
/* GET XA((T1-T0)/N(M)). */
	*ok = TRUE_;
	(*xa)(t0, &x[1], t1, &ws[*ix1 - 1], nx, &n[m], f, d, da, ok, error, &
		errpar[1], inmi);
	if (*ok) {
	    goto L4;
	}
	if (nerror_(&nerr) == 0) {
	    goto L3;
	}
	leave_();
	return 0;
L3:
/*     EXTRAPOLATE THE RESULTS. */
L4:
	if (a4ssox_(a10dey_1.wv, a10dey_1.rv, a10dey_1.iv, a10dey_1.lv, &n[1],
		 &m)) {
	    goto L6;
	}
	if (m > 1) {
	    *done = (*serror)(&ws[*ix1 - 1], nx, t1, dt, &errpar[1], delta, &
		    rs[*ie - 1], error);
	}
/* CHECK FOR CONVERGENCE. */
/*     CHECK FOR A RESTART. */
	if (a4ssor_(a10dey_1.wv, a10dey_1.rv, a10dey_1.iv, a10dey_1.lv, &
		errpar[1])) {
	    goto L6;
	}
/* L5: */
    }
/*   GET OPTIMAL DT AND ORDER ( LOZENGE SIZE ). */
L6:
    if (a4ssom_(a10dey_1.wv, a10dey_1.rv, a10dey_1.iv, a10dey_1.lv, dt)) {
	goto L7;
    }
/*   OUTPUT THE RESULTS FOR THIS TIME-STEP. */
    (*sout)(t0, &x[1], t1, &ws[*ix1 - 1], nx, dt, tstop, &rs[*ie - 1], ok, 
	    output);
/*   WIND-UP THIS TIME-STEP. */
    if (a4ssoe_(a10dey_1.wv, a10dey_1.rv, a10dey_1.iv, a10dey_1.lv, &x[1], 
	    tstop, dt)) {
	goto L7;
    }
    goto L1;
L7:
    leave_();
    return 0;
} /* a10dex_ */

#undef ix1
#undef ws
#undef rs
#undef ls
#undef is
#undef ok
#undef ie
#undef t1
#undef t0
#undef done


