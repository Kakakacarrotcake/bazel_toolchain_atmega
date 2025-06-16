/* origin: FreeBSD /usr/src/lib/msun/src/e_log10f.c */
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
/*
 * See comments in log10.c.
 */

#include <math.h>
#include <stdint.h>
#include <errno.h>

static const float
ivln10hi  =  4.3432617188e-01, /* 0x3ede6000 */
ivln10lo  = -3.1689971365e-05, /* 0xb804ead9 */
log10_2hi =  3.0102920532e-01, /* 0x3e9a2080 */
log10_2lo =  7.9034151668e-07, /* 0x355427db */
/* |(log(1+s)-log(1-s))/s - Lg(s)| < 2**-34.24 (~[-4.95e-11, 4.97e-11]). */
Lg1 = 0xaaaaaa.0p-24, /* 0.66666662693 */
Lg2 = 0xccce13.0p-25, /* 0.40000972152 */
Lg3 = 0x91e9ee.0p-25, /* 0.28498786688 */
Lg4 = 0xf89e26.0p-26; /* 0.24279078841 */

float log10f(float x)
{
	union {float f; uint32_t i;} ux = {x};
	float_t hfsq,f,s,z,R,w,t1,t2,dk,hi,lo;
	int k;

	k = 0;
	if (ux.i < 0x00800000 || (ux.i & (UINT32_C(1)<<31))) {  /* x < 2**-126  */
		if (ux.i<<1 == 0) {
			/* log(+-0)=-inf */
#if math_errhandling & MATH_ERRNO
			ux.i = UINT32_C(0xff800000);
			return ux.f;
#else
			return -1/(x*x);
#endif
		}
		if (ux.i & (UINT32_C(1)<<31)) {
			/* log(-#) = NaN */
#if math_errhandling & MATH_ERRNO
			errno = EDOM;
			ux.i = UINT32_C(0x7fffffff);
			return ux.f;
#else
			return (x-x)/0.0f;
#endif
		}
		/* subnormal number, scale up x */
		k -= 25;
		x *= 0x1p25f;
		ux.f = x;
	} else if (ux.i >= 0x7f800000) {
		return x;
	} else if (ux.i == 0x3f800000)
		return 0;

	/* reduce x into [sqrt(2)/2, sqrt(2)] */
	ux.i += 0x3f800000 - 0x3f3504f3;
	k += (int)(ux.i>>23) - 0x7f;
	ux.i = (ux.i&0x007fffff) + 0x3f3504f3;

	f = ux.f - 1.0f;
	s = f/(2.0f + f);
	z = s*s;
	w = z*z;
	t1= w*(Lg2+w*Lg4);
	t2= z*(Lg1+w*Lg3);
	R = t2 + t1;
	hfsq = 0.5f*f*f;

	hi = f - hfsq;
	ux.f = hi;
	ux.i &= 0xfffff000;
	hi = ux.f;
	lo = f - hi - hfsq + s*(hfsq+R);
	dk = k;
	return dk*log10_2lo + (lo+hi)*ivln10lo + lo*ivln10hi + hi*ivln10hi + dk*log10_2hi;
}
