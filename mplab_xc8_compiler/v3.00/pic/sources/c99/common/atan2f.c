/* origin: FreeBSD /usr/src/lib/msun/src/e_atan2f.c */
/*
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include "libm.h"

static const float
pi     = 3.1415927410e+00, /* 0x40490fdb */
pi_lo  = -8.7422776573e-08; /* 0xb3bbbd2e */

float atan2f(float y, float x)
{
	float z;
	union { float f; uint32_t i;} ix,iy;
	uint8_t m;

	if (isnan(x) || isnan(y))
		return x+y;
	ix.f = x;
	iy.f = y;
	if (ix.i == 0x3f800000)  /* x=1.0 */
		return atanf(y);
	m = ((iy.i>>31)&1) | ((ix.i>>30)&2);  /* 2*sign(x)+sign(y) */
	ix.i &= 0x7fffffff;
	iy.i &= 0x7fffffff;

	/* when y = 0 */
	if (iy.i == 0) {
		switch (m) {
		case 0:
		case 1: return y;   /* atan(+-0,+anything)=+-0 */
		case 2: return  pi; /* atan(+0,-anything) = pi */
		case 3: return -pi; /* atan(-0,-anything) =-pi */
		}
	}
	/* when x = 0 */
	if (ix.i == 0)
		return m&1 ? -pi/2 : pi/2;
	/* when x is INF */
	if (ix.i == 0x7f800000) {
		if (iy.i == 0x7f800000) {
			switch (m) {
			case 0: return  pi/4; /* atan(+INF,+INF) */
			case 1: return -pi/4; /* atan(-INF,+INF) */
			case 2: return 3*pi/4;  /*atan(+INF,-INF)*/
			case 3: return -3*pi/4; /*atan(-INF,-INF)*/
			}
		} else {
			switch (m) {
			case 0: return  0.0f;    /* atan(+...,+INF) */
			case 1: return -0.0f;    /* atan(-...,+INF) */
			case 2: return  pi; /* atan(+...,-INF) */
			case 3: return -pi; /* atan(-...,-INF) */
			}
		}
	}
	/* |y/x| > 0x1p26 */
	if (ix.i+(26L<<23) < iy.i || iy.i == 0x7f800000)
		return m&1 ? -pi/2 : pi/2;

	/* z = atan(|y/x|) with correct underflow */
	if ((m&2) && iy.i+(26L<<23) < ix.i)  /*|y/x| < 0x1p-26, x < 0 */
		z = 0.0;
	else
		z = atanf(fabsf(y/x));
	switch (m) {
	case 0: return z;              /* atan(+,+) */
	case 1: return -z;             /* atan(-,+) */
	case 2: return pi - (z-pi_lo); /* atan(+,-) */
	default: /* case 3 */
		return (z-pi_lo) - pi; /* atan(-,-) */
	}
}
