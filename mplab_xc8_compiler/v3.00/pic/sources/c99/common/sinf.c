/* origin: FreeBSD /usr/src/lib/msun/src/s_sinf.c */
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
s1pio2 = 1*M_PI_2, /* 0x3FF921FB, 0x54442D18 */
s2pio2 = 2*M_PI_2, /* 0x400921FB, 0x54442D18 */
s3pio2 = 3*M_PI_2, /* 0x4012D97C, 0x7F3321D2 */
s4pio2 = 4*M_PI_2; /* 0x401921FB, 0x54442D18 */

float sinf(float x)
{
	double y;
	uint8_t n;
	union { float f; uint32_t i; } ux;
	bool sign;

	ux.f = x;
	sign = ux.i & (UINT32_C(1)<<31);
	ux.i &= 0x7fffffff;

	if (ux.i <= 0x3f490fda) {  /* |x| ~<= pi/4 */
		if (ux.i < 0x39800000) {  /* |x| < 2**-12 */
#if math_errhandling & MATH_ERREXCEPT
			/* raise inexact if x!=0 and underflow if subnormal */
			FORCE_EVAL(ux.i < 0x00800000 ? x/0x1p120f : x+0x1p120f);
#endif
			return x;
		}
		return __sindf(x);
	}
	if (ux.i <= 0x407b53d1) {  /* |x| ~<= 5*pi/4 */
		if (ux.i <= 0x4016cbe3) {  /* |x| ~<= 3pi/4 */
			if (sign)
				return -__cosdf(x + s1pio2);
			else
				return __cosdf(x - s1pio2);
		}
		return __sindf(sign ? -(x + s2pio2) : -(x - s2pio2));
	}
	if (ux.i <= 0x40e231d5) {  /* |x| ~<= 9*pi/4 */
		if (ux.i <= 0x40afeddf) {  /* |x| ~<= 7*pi/4 */
			if (sign)
				return __cosdf(x + s3pio2);
			else
				return -__cosdf(x - s3pio2);
		}
		return __sindf(sign ? x + s4pio2 : x - s4pio2);
	}

	/* sin(Inf or NaN) is NaN */
	if (ux.i >= 0x7f800000) {
		ux.i |= UINT32_C(0xffffff);
		return ux.f;
	}

	/* general argument reduction needed */
	n = __rem_pio2f(x, &y) & 3;
	switch (n) {
	case 2: y = -y;
			// fall thru
	case 0: return  __sindf(y);
	case 1: return  __cosdf(y);
	default: return -__cosdf(y);
	}
}
