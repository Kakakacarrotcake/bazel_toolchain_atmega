/* origin: FreeBSD /usr/src/lib/msun/src/e_logf.c */
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

#include <math.h>
#include <stdint.h>
#include <errno.h>

static const float
ln2_hi = 6.9313812256e-01, /* 0x3f317180 */
ln2_lo = 9.0580006145e-06, /* 0x3717f7d1 */
/* |(log(1+s)-log(1-s))/s - Lg(s)| < 2**-34.24 (~[-4.95e-11, 4.97e-11]). */
Lg1 = 0xaaaaaa.0p-24, /* 0.66666662693 */
Lg2 = 0xccce13.0p-25, /* 0.40000972152 */
Lg3 = 0x91e9ee.0p-25, /* 0.28498786688 */
Lg4 = 0xf89e26.0p-26; /* 0.24279078841 */

float logf(float x)
{
	union {float f; uint32_t i;} ux = {x};
	float_t hfsq,s,z,R,w,t1,t2,dk;
	int k;

	k = 0;
	if (ux.i < 0x00800000 || (ux.i & (UINT32_C(1)<<31))) {  /* x < 2**-126  */
		if (ux.i<<1 == 0) {
#if math_errhandling & MATH_ERRNO
			ux.i = 0xff800000;
			return ux.f; /* log(+-0)=-inf */
#else
			return -1/(x*x);  /* log(+-0)=-inf */
#endif
		}
		if (ux.i & (UINT32_C(1)<<31)) {
#if math_errhandling & MATH_ERRNO
			errno = EDOM;
			ux.i = 0x7fffffff;
			return ux.f; /* log(-#) = NaN */
#else
			return (x-x)/0.0f; /* log(-#) = NaN */
#endif
		}
		/* subnormal number, scale up x */
		k -= 25;
		ux.f *= 0x1p25f;
	} else if (ux.i >= 0x7f800000) {
		return x;
	} else if (ux.i == 0x3f800000)
		return 0;

	/* reduce x into [sqrt(2)/2, sqrt(2)] */
	ux.i += 0x3f800000 - 0x3f3504f3;
	k += (int)(ux.i>>23) - 0x7f;
	ux.i = (ux.i&0x007fffff) + 0x3f3504f3;

	ux.f -= 1.0f;
	s = ux.f/(2.0f + ux.f);
	z = s*s;
	w = z*z;
	t1= w*(Lg2+w*Lg4);
	t2= z*(Lg1+w*Lg3);
	R = t2 + t1;
	hfsq = 0.5f*ux.f*ux.f;
	dk = k;
	return s*(hfsq+R) + dk*ln2_lo - hfsq + ux.f + dk*ln2_hi;
}
