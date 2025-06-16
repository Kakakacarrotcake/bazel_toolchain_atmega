/* origin: FreeBSD /usr/src/lib/msun/src/e_powf.c */
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

#include <errno.h>
#include "libm.h"

static const float
bp[]   = {1.0, 1.5,},
dp_h[] = { 0.0, 5.84960938e-01,}, /* 0x3f15c000 */
dp_l[] = { 0.0, 1.56322085e-06,}, /* 0x35d1cfdc */
two24  =  16777216.0,  /* 0x4b800000 */
#if !(math_errhandling & MATH_ERRNO)
huge   =  1.0e30,
tiny   =  1.0e-30,
#endif
/* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */
L1 =  6.0000002384e-01, /* 0x3f19999a */
L2 =  4.2857143283e-01, /* 0x3edb6db7 */
L3 =  3.3333334327e-01, /* 0x3eaaaaab */
L4 =  2.7272811532e-01, /* 0x3e8ba305 */
L5 =  2.3066075146e-01, /* 0x3e6c3255 */
L6 =  2.0697501302e-01, /* 0x3e53f142 */
P1 =  1.6666667163e-01, /* 0x3e2aaaab */
P2 = -2.7777778450e-03, /* 0xbb360b61 */
P3 =  6.6137559770e-05, /* 0x388ab355 */
P4 = -1.6533901999e-06, /* 0xb5ddea0e */
P5 =  4.1381369442e-08, /* 0x3331bb4c */
lg2     =  6.9314718246e-01, /* 0x3f317218 */
lg2_h   =  6.93145752e-01,   /* 0x3f317200 */
lg2_l   =  1.42860654e-06,   /* 0x35bfbe8c */
ovt     =  4.2995665694e-08, /* -(128-log2(ovfl+.5ulp)) */
cp      =  9.6179670095e-01, /* 0x3f76384f =2/(3ln2) */
cp_h    =  9.6191406250e-01, /* 0x3f764000 =12b cp */
cp_l    = -1.1736857402e-04, /* 0xb8f623c6 =tail of cp_h */
ivln2   =  1.4426950216e+00, /* 0x3fb8aa3b =1/ln2 */
ivln2_h =  1.4426879883e+00, /* 0x3fb8aa00 =16b 1/ln2*/
ivln2_l =  7.0526075433e-06; /* 0x36eca570 =1/ln2 tail*/

#ifndef _POWF_QUAL
#define _POWF_QUAL
#endif

_POWF_QUAL float powf(float x, float y)
{
	float z_h,z_l,p_l;
	float t2,r,s,u,v,w;
	int32_t i,j,k,yisint,n,iy;
#if math_errhandling & MATH_ERRNO
	uint32_t sn;
#else
	float sn;
#endif
	union fui {float f; uint32_t u; int32_t i;} hx, hy, ix, ax, is, t1, p_h, y1, z, t;

	hx.f = x;
	hy.f = y;
	ix.i = hx.i & 0x7fffffff;
	iy = hy.i & 0x7fffffff;

	/* x**0 = 1, even if x is NaN */
	if (iy == 0)
		return 1.0f;
	/* 1**y = 1, even if y is NaN */
	if (hx.i == 0x3f800000)
		return 1.0f;
	/* NaN if either arg is NaN */
	if (ix.i > 0x7f800000 || iy > 0x7f800000)
		return x + y;

	/* determine if y is an odd int when x < 0
	 * yisint = 0       ... y is not an integer
	 * yisint = 1       ... y is an odd int
	 * yisint = 2       ... y is an even int
	 */
	yisint  = 0;
	if (hx.i < 0) {
		if (iy >= 0x4b800000)
			yisint = 2; /* even integer y */
		else if (iy >= 0x3f800000) {
			k = (iy>>23) - 0x7f;         /* exponent */
			j = iy>>(23-k);
			if ((j<<(23-k)) == iy)
				yisint = 2 - (j & 1);
		}
	}

	/* special value of y */
	if (iy == 0x7f800000) {  /* y is +-inf */
		if (ix.i == 0x3f800000)      /* (-1)**+-inf is 1 */
			return 1.0f;
		else if (ix.i > 0x3f800000)  /* (|x|>1)**+-inf = inf,0 */
			return hy.i >= 0 ? y : 0.0f;
		else                       /* (|x|<1)**+-inf = 0,inf */
			return hy.i >= 0 ? 0.0f: -y;
	}
	if (iy == 0x3f800000)    /* y is +-1 */
		return hy.i >= 0 ? x : 1.0f/x;
	if (hy.i == 0x40000000)    /* y is 2 */
		return x*x;
	if (hy.i == 0x3f000000) {  /* y is  0.5 */
		if (hx.i >= 0)     /* x >= +0 */
			return sqrtf(x);
	}

	ax.u = ix.u;
	/* special value of x */
	if (ix.i == 0x7f800000 || ix.i == 0 || ix.i == 0x3f800000) { /* x is +-0,+-inf,+-1 */
		z.f = ax.f;
		if (hy.i < 0)  /* z = (1/|x|) */
			z.f = 1.0f/z.f;
		if (hx.i < 0) {
			if (((ix.i-0x3f800000)|yisint) == 0) {
#if math_errhandling & MATH_ERRNO
				errno = EDOM;
				z.u = UINT32_C(0x7fffffff); /* (-1)**non-int is NaN */
#else
				z.f = (z.f-z.f)/(z.f-z.f); /* (-1)**non-int is NaN */
#endif
			} else if (yisint == 1)
				z.f = -z.f;          /* (x<0)**odd = -(|x|**odd) */
		}
		return z.f;
	}

#if math_errhandling & MATH_ERRNO
	sn = 0;
#else
	sn = 1.0f; /* sign of result */
#endif

	if (hx.i < 0) {
		if (yisint == 0) { /* (x<0)**(non-int) is NaN */
#if math_errhandling & MATH_ERRNO
			errno = EDOM;
			z.u = UINT32_C(0x7fffffff);
			return z.f;
#else
			return (x-x)/(x-x);
#endif
		}
#if math_errhandling & MATH_ERRNO
		if (yisint == 1) /* (x<0)**(odd int) */
			sn = UINT32_C(0x80000000);
#else
		if (yisint == 1) /* (x<0)**(odd int) */
			sn = -1.0f;
#endif
	}

	/* |y| is huge */
	if (iy > 0x4d000000) { /* if |y| > 2**27 */
		/* over/underflow if x is not close to one */
		if (ix.i < 0x3f7ffff8) {
#if math_errhandling & MATH_ERRNO
			if (hy.i < 0) {
				z.u = sn | UINT32_C(0x7f800000);
				return z.f;
			}
			else {
				return 0.0f; // XC8-PIC does not have -0.0f and denorms are flushed to zero
			}
#else
			return hy.i < 0 ? sn*huge*huge : sn*tiny*tiny;
#endif
		}
		if (ix.i > 0x3f800007) {
#if math_errhandling & MATH_ERRNO
			if (hy.i > 0) {
				z.u = sn | UINT32_C(0x7f800000);
				return z.f;
			}
			else {
				return 0.0f; // XC8-PIC does not have -0.0f and denorms are flushed to zero
			}
#else
			return hy.i > 0 ? sn*huge*huge : sn*tiny*tiny;
#endif
		}
		/* now |1-x| is tiny <= 2**-20, suffice to compute
		   log(x) by x-x^2/2+x^3/3-x^4/4 */
		t.f = ax.f - 1;     /* t has 20 trailing zeros */
		w = (t.f*t.f)*(0.5f - t.f*(0.333333333333f - t.f*0.25f));
		u = ivln2_h*t.f;  /* ivln2_h has 16 sig. bits */
		v = t.f*ivln2_l - w*ivln2;
		t1.f = u + v;
		is.f = t1.f;
		t1.u = is.u & UINT32_C(0xfffff000);
		t2 = v - (t1.f-u);
	} else {
		union fui s_h, t_h;
		float s2,s_l,t_l;
		n = 0;
		/* take care subnormal number */
		if (ix.i < 0x00800000) {
			ax.f *= two24;
			n -= 24;
			ix.i = ax.i;
		}
		n += ((ix.i)>>23) - 0x7f;
		j = ix.i & 0x007fffff;
		/* determine interval */
		k = 0;
		ix.i = j | 0x3f800000;     /* normalize ix */
		if (j <= 0x1cc471) {       /* |x|<sqrt(3/2) */
			//k = 0;
		}
		else if (j < 0x5db3d7) {   /* |x|<sqrt(3)   */
			k = 1;
		}
		else {
			//k = 0;
			n += 1;
			ix.i -= 0x00800000;
		}
		ax.i = ix.i;

		/* compute s = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
		u = ax.f - bp[k];   /* bp[0]=1.0, bp[1]=1.5 */
		v = 1.0f/(ax.f+bp[k]);
		s = u*v;
		s_h.f = s;
		is.f = s_h.f;
		s_h.u = is.u & UINT32_C(0xfffff000);
		/* t_h=ax+bp[k] High */
		is.i = (int32_t)(((uint32_t)(ix.i>>1) & UINT32_C(0xfffff000)) | UINT32_C(0x20000000));
		t_h.i = is.i + 0x00400000 + (k<<21);
		t_l = ax.f - (t_h.f - bp[k]);
		s_l = v*((u - s_h.f*t_h.f) - s_h.f*t_l);
		/* compute log(ax) */
		s2 = s*s;
		r = s2*s2*(L1+s2*(L2+s2*(L3+s2*(L4+s2*(L5+s2*L6)))));
		r += s_l*(s_h.f+s);
		s2 = s_h.f*s_h.f;
		t_h.f = 3.0f + s2 + r;
		is.f = t_h.f;
		t_h.u = is.u & UINT32_C(0xfffff000);
		t_l = r - ((t_h.f - 3.0f) - s2);
		/* u+v = s*(1+...) */
		u = s_h.f*t_h.f;
		v = s_l*t_h.f + t_l*s;
		/* 2/(3log2)*(s+...) */
		p_h.f = u + v;
		is.f = p_h.f;
		p_h.u = is.u & UINT32_C(0xfffff000);
		p_l = v - (p_h.f - u);
		z_h = cp_h*p_h.f;  /* cp_h+cp_l = 2/(3*log2) */
		z_l = cp_l*p_h.f + p_l*cp+dp_l[k];
		/* log2(ax) = (s+..)*2/(3*log2) = n + dp_h + z_h + z_l */
		t.f = (float)n;
		t1.f = (((z_h + z_l) + dp_h[k]) + t.f);
		is.f = t1.f;
		t1.u = is.u & UINT32_C(0xfffff000);
		t2 = z_l - (((t1.f - t.f) - dp_h[k]) - z_h);
	}

	/* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
	is.f = y;
	y1.u = is.u & UINT32_C(0xfffff000);
	p_l = (y-y1.f)*t1.f + y*t2;
	p_h.f = y1.f*t1.f;
	z.f = p_l + p_h.f;
	j = z.i;
	if (j > 0x43000000) {          /* if z > 128 */
#if math_errhandling & MATH_ERRNO
		z.u = sn | UINT32_C(0x7f800000);
		return z.f;
#else
		return sn*huge*huge;  /* overflow */
#endif
	}
	else if (j == 0x43000000) {  /* if z == 128 */
		if (p_l + ovt > z.f - p_h.f) {
#if math_errhandling & MATH_ERRNO
		z.u = sn | UINT32_C(0x7f800000);
		return z.f;
#else
		return sn*huge*huge;  /* overflow */
#endif
		}
	} else if ((j&0x7fffffff) > 0x43160000) {  /* z < -150 */ // FIXME: check should be  (uint32_t)j > 0xc3160000
#if math_errhandling & MATH_ERRNO
		return 0.0f;  // XC8-PIC does not have -0.0f and denorms are flushed to zero
#else
		return sn*tiny*tiny;  /* underflow */
#endif
	}
	else if (j == 0xc3160000) {  /* z == -150 */
		if (p_l <= z.f-p_h.f) {
#if math_errhandling & MATH_ERRNO
		return 0.0f;  // XC8-PIC does not have -0.0f and denorms are flushed to zero
#else
		return sn*tiny*tiny;  /* underflow */
#endif
		}
	}
	/*
	 * compute 2**(p_h+p_l)
	 */
	i = j & 0x7fffffff;
	k = (i>>23) - 0x7f;
	n = 0;
	if (i > 0x3f000000) {   /* if |z| > 0.5, set n = [z+0.5] */
		n = j + (0x00800000>>(k+1));
		k = ((n&0x7fffffff)>>23) - 0x7f;  /* new k for n */
		t.i = n & ~(0x007fffff>>k);
		n = ((n&0x007fffff)|0x00800000)>>(23-k);
		if (j < 0)
			n = -n;
		p_h.f -= t.f;
	}
	t.f = p_l + p_h.f;
	is.f = t.f;
	t.u = is.u & UINT32_C(0xffff8000);
	u = t.f*lg2_h;
	v = (p_l-(t.f-p_h.f))*lg2 + t.f*lg2_l;
	z.f = u + v;
	w = v - (z.f - u);
	t.f = z.f*z.f;
	t1.f = z.f - t.f*(P1+t.f*(P2+t.f*(P3+t.f*(P4+t.f*P5))));
	r = (z.f*t1.f)/(t1.f-2.0f) - (w+z.f*w);
	z.f = 1.0f - (r - z.f);
	j = z.i;
	j += n<<23;
	if ((j>>23) <= 0)  /* subnormal output */
		z.f = scalbnf(z.f, (int)n);
	else
		z.i = j;
#if math_errhandling & MATH_ERRNO
	if (z.u != 0) {
		z.u ^= sn;
	}
	return z.f;
#else
	return sn*z.f;
#endif
}
