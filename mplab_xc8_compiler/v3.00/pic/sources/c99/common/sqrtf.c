/* origin: FreeBSD /usr/src/lib/msun/src/e_sqrtf.c */
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

#include <stdint.h>
#include <errno.h>
#include "libm.h"

#if math_errhandling & MATH_ERREXCEPT
static const float tiny = 1.0e-30;
#endif

float sqrtf(float x)
{
	float z;
	int32_t s,q,m,t,i;
	uint32_t r;
	union { float f; int32_t i; } ix;

	ix.f = x;

	/* take care of Inf and NaN */
	if ((ix.i&0x7f800000) == 0x7f800000) {
		/* sqrt(NaN)=NaN, sqrt(+inf)=+inf, sqrt(-inf)=sNaN */
		if ((ix.i & 0x7fffff) || (ix.i < 0)) {
			ix.i |= INT32_C(0x7fffff);
		}
		return ix.f;
	}

	/* take care of zero */
	if (ix.i <= 0) {
		if ((ix.i & ~(int32_t)INT32_C(0x80000000)) == 0) {
			// XC8-PIC does not have -0.0
			return 0.0f;  /* sqrt(+-0) = +-0 */
		}
		if (ix.i < 0) {
			errno = EDOM;
			ix.i = INT32_C(0x7fffffff);
			return ix.f;
		}
	}
	/* normalize x */
	m = ix.i>>23;
	if (m == 0) {  /* subnormal x */
		for (i = 0; (ix.i&0x00800000) == 0; i++)
			ix.i<<=1;
		m -= i - 1;
	}
	m -= 127;  /* unbias exponent */
	ix.i = (ix.i&0x007fffff)|0x00800000;
	if (m&1)  /* odd m, double x to make it even */
		ix.i += ix.i;
	m >>= 1;  /* m = [m/2] */

	/* generate sqrt(x) bit by bit */
	ix.i += ix.i;
	q = s = 0;       /* q = sqrt(x) */
	r = 0x01000000;  /* r = moving bit from right to left */

	while (r != 0) {
		t = s + (int32_t)r;
		if (t <= ix.i) {
			s = t+(int32_t)r;
			ix.i -= t;
			q += r;
		}
		ix.i += ix.i;
		r >>= 1;
	}

	/* use floating add to find out rounding direction */
	if (ix.i != 0) {
#if math_errhandling & MATH_ERREXCEPT
		z = 1.0f - tiny; /* raise inexact flag */
		if (z >= 1.0f) {
			z = 1.0f + tiny;
			if (z > 1.0f)
				q += 2;
			else
				q += q & 1;
		}
#else
		/* XC8-PIC always takes this path as 1.0f +/- tiny is reduced to 1.0f */
		q += q & 1;
#endif
	}
	ix.i = (q>>1) + 0x3f000000;
	ix.i += m << 23;
	return ix.f;
}
