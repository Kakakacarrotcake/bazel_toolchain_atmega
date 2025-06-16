/* origin: FreeBSD /usr/src/lib/msun/src/s_tanf.c */
/*
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 * Optimized by Bruce D. Evans.
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

#include <stdbool.h>
#include "libm.h"

/* Small multiples of pi/2 rounded to double precision. */
static const double
t1pio2 = 1*M_PI_2, /* 0x3FF921FB, 0x54442D18 */
t2pio2 = 2*M_PI_2, /* 0x400921FB, 0x54442D18 */
t3pio2 = 3*M_PI_2, /* 0x4012D97C, 0x7F3321D2 */
t4pio2 = 4*M_PI_2; /* 0x401921FB, 0x54442D18 */

float tanf(float x)
{
	double y;
	int8_t n;
	union { float f; uint32_t i; } ix;
	bool sign;

	ix.f = x;
	sign = ix.i & (UINT32_C(1) << 31);
	ix.i &= 0x7fffffff;

	if (ix.i <= 0x3f490fda) {  /* |x| ~<= pi/4 */
		if (ix.i < 0x39800000) {  /* |x| < 2**-12 */
#if math_errhandling & MATH_ERREXCEPT
			/* raise inexact if x!=0 and underflow if subnormal */
			FORCE_EVAL(ix.i < 0x00800000 ? x/0x1p120f : x+0x1p120f);
#endif
			return x;
		}
		return __tandf(x, 0);
	}
	if (ix.i <= 0x407b53d1) {  /* |x| ~<= 5*pi/4 */
		if (ix.i <= 0x4016cbe3)  /* |x| ~<= 3pi/4 */
			return __tandf((sign ? x+t1pio2 : x-t1pio2), 1);
		else
			return __tandf((sign ? x+t2pio2 : x-t2pio2), 0);
	}
	if (ix.i <= 0x40e231d5) {  /* |x| ~<= 9*pi/4 */
		if (ix.i <= 0x40afeddf)  /* |x| ~<= 7*pi/4 */
			return __tandf((sign ? x+t3pio2 : x-t3pio2), 1);
		else
			return __tandf((sign ? x+t4pio2 : x-t4pio2), 0);
	}

	/* tan(Inf or NaN) is NaN */
	if (ix.i >= 0x7f800000) {
		ix.i |= 0xffffff;
		return ix.f;
	}

	/* argument reduction */
	n = __rem_pio2f(x, &y) & 1;
	return __tandf(y, n);
}
