/* vfprintf with configurable support for format conversions */
/* This code is specifically for XC8 */
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inline.h>
#include <stdbool.h>

/* Configuration options */
#ifdef _VFPF_ALL
#define _VFPF_CONVERT
#define _VFPF_SPCSIGN
#define _VFPF_MANSIGN
#define _VFPF_FILL
#define _VFPF_LEFT
#define _VFPF_ALTERN
#define _VFPF_WIDTH
#define _VFPF_PRECISION
#define _VFPF_PERC
#define _VFPF_UPCASE
#define _VFPF_RETVALUE

#define _VFPF_HH
#define _VFPF_H
#define _VFPF_L
#define _VFPF_LL
#define _VFPF_J
#define _VFPF_T
#define _VFPF_Z

#define _VFPF_A
#define _VFPF_C
#define _VFPF_D
#define _VFPF_E
#define _VFPF_F
#define _VFPF_G
#define _VFPF_O
#define _VFPF_N
#define _VFPF_P
#define _VFPF_S
#define _VFPF_U
#define _VFPF_X
#define _VFPF_x
#endif

#if defined(_VFPF_SPCSIGN) || defined(_VFPF_MANSIGN) || defined(_VFPF_FILL) || defined(_VFPF_LEFT) || defined(_VFPF_ALTERN)
#define _VFPF_FLAGS
#endif

#if defined(_VFPF_D)
#define _VFPF_SIGNED_CONV
#define _VFPF_INT_CONV
#endif

#if defined(_VFPF_X) || defined(_VFPF_x)
#define _VFPF_HEX_CONV
#define _VFPF_INT_CONV
#endif

#if defined(_VFPF_O) || defined(_VFPF_U) || defined(_VFPF_HEX_CONV)
#define _VFPF_UNSIGNED_CONV
#define _VFPF_INT_CONV
#endif

#if defined(_VFPF_A) || defined(_VFPF_E) || defined(_VFPF_F) || defined(_VFPF_G)
#define _VFPF_FLOAT_CONV
#endif

#if defined(_VFPF_H) || defined(_VFPF_HH) || defined(_VFPF_L) || defined(_VFPF_LL) || defined(_VFPF_J) \
	|| defined(_VFPF_T) || defined(_VFPF_Z)
#define _VFPF_LENGTH_MOD
#endif

// largest integral types needed
#if defined(_VFPF_INT_CONV) || defined(_VFPF_P)
#define _VFPF_INT_LENGTH int

#if (defined(_VFPF_P) && defined(_PIC18)) 
#undef _VFPF_INT_LENGTH
#define _VFPF_INT_LENGTH __int24
#endif

#if defined(_VFPF_L)
#undef _VFPF_INT_LENGTH
#define _VFPF_INT_LENGTH long
#endif
#if (defined(_VFPF_LL) || defined(_VFPF_J)) && (__SIZEOF_LONG_LONG__ > __SIZEOF_LONG__)
#undef _VFPF_INT_LENGTH
#define _VFPF_INT_LENGTH long long
#endif

typedef signed _VFPF_INT_LENGTH vfpf_sint_t;
typedef unsigned _VFPF_INT_LENGTH vfpf_uint_t;
#endif // _VFPF_INT_CONV || _VFPF_P

#if !defined(_VFPF_RETVALUE) && (defined(SNPRINTF) || defined(VSNPRINTF) || defined(VSPRINTF))
#define _VFPF_RETVALUE
#endif
#if defined(_VFPF_N) || defined(_VFPF_RETVALUE)
#define _VFPF_COUNT_CHARS
#define _COUNT_TYPE int
#else
#define _COUNT_TYPE	void
#endif
#define _COUNT_CAST	(_COUNT_TYPE)

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)	(sizeof(a)/sizeof(a[0]))
#endif
#ifndef CSTRLEN
#define CSTRLEN(s)	(ARRAYSIZE(s)-1)
#endif

#if defined(_VFPF_C) && !defined(_VFPF_LEFT) && !defined(_VFPF_WIDTH)
#define _VFPF_SIMPLE_C
#endif

#if defined(_VFPF_S) && !defined(_VFPF_LEFT) && !defined(_VFPF_WIDTH) && !defined(_VFPF_PRECISION)
#define _VFPF_SIMPLE_S
#endif

#if defined(_VFPF_D) && !defined(_VFPF_SPCSIGN) && !defined(_VFPF_MANSIGN) && !defined(_VFPF_FILL) && !defined(_VFPF_LEFT) \
	&& !defined(_VFPF_SPCSIGN) && !defined(_VFPF_WIDTH) && !defined(_VFPF_PRECISION)
#define _VFPF_SIMPLE_D
#endif

#if defined(_VFPF_U) && !defined(_VFPF_FILL) && !defined(_VFPF_LEFT) && !defined(_VFPF_SPCSIGN) && !defined(_VFPF_WIDTH) \
	&& !defined(_VFPF_PRECISION)
#define _VFPF_SIMPLE_U
#endif

#if defined(_VFPF_U) && !defined(_VFPF_FILL) && !defined(_VFPF_LEFT) && !defined(_VFPF_SPCSIGN) && !defined(_VFPF_WIDTH) \
	&& !defined(_VFPF_PRECISION) && !defined(_VFPF_ALTERN)
#define _VFPF_SIMPLE_O
#endif

#if defined(_VFPF_HEX_CONV) && !defined(_VFPF_FILL) && !defined(_VFPF_LEFT) && !defined(_VFPF_SPCSIGN) && !defined(_VFPF_WIDTH) \
	&& !defined(_VFPF_PRECISION) && !defined(_VFPF_ALTERN)
#define _VFPF_SIMPLE_HEX
#endif

/* Flags, precision, width */
#define MINUS_FLAG (1 << 0)
#define ZERO_FLAG (1 << 1)
#define PLUS_FLAG (1 << 2)
#define SPACE_FLAG (1 << 3)
#define POUND_FLAG (1 << 4)
static int prec, width;
static char flags;

union flui {
	long double	f;
#if __SIZEOF_LONG_DOUBLE__ == 8
	int64_t	u;
#else
	int32_t	u;
#endif
};
#if __SIZEOF_LONG_DOUBLE__ == 8
#define FNEARLYEQ(x,y)	(llabs((x).u-(y).u) <= 1)
#else
#define FNEARLYEQ(x,y)	(labs((x).u-(y).u) <= 1)
#endif

#ifdef _VFPF_CONVERT
/* This buffer should be at least 32 bytes long for this code to be safe */
/* Output that would exceed buffer capacity is truncated.
 * Baseline parts have only 16 bytes of GPR per bank to which objects are limited to.*/
#ifdef _PIC12
#define DBLEN 16
#define EXPLEN 5
#else
#if defined(_VFPF_A) || defined(_VFPF_E) || defined(_VFPF_F) || defined(_VFPF_G)
#define DBLEN 80
#define EXPLEN 5
#else
#define DBLEN 32
#endif
#endif
static char dbuf[DBLEN];

#ifdef _VFPF_COUNT_CHARS
/* Character count */
static _COUNT_TYPE nout;
#endif

/* Output the string in dbuf, padded on the left or right */
static _INLINE _COUNT_TYPE pad(FILE *fp, char *buf, int p)
{
    int i;

#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
    /* Left justify ? Put out string */
    if (flags & MINUS_FLAG) {
        fputs((const char *)buf, fp);
    }
#endif

    /* Put out padding */
	if (p < 0) {
		p = 0;
	}
    i = 0;
    while (i < p) {
        fputc(' ', fp);
        ++i;
    }

    /* Right justify ? Put out string */
#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
    if (!(flags & MINUS_FLAG)) {
#endif
        fputs((const char *)buf, fp);
#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
    }
#endif

#ifdef _VFPF_COUNT_CHARS
    return (int)(strlen(buf) + (size_t)p);
#endif
}
#endif

#ifdef _VFPF_A
#ifdef _VFPF_UPCASE
static _INLINE _COUNT_TYPE atoa(FILE *fp, long double f, char c)
#define _ATOA(fp,f,c)	atoa(fp,f,c)
#else
static _INLINE _COUNT_TYPE atoa(FILE *fp, long double f)
#define _ATOA(fp,f,c)	atoa(fp,f)
#endif
{
    char pp, sign, esign;
    int d, e, i, m, n, ne, p, t, w;
    long double g, h, l, ou, u;

	/* Record if it's %A or %a */
#ifdef _VFPF_UPCASE
	c = isupper(c) ? 1 : 0;
#endif
    /* Record sign, get absolute value */
    sign = 0;
    g = f;
    if (g < 0.0) {
        sign = 1;
        g = -g;
    }

    /* Print sign, prefix */
    n = 0;
    w = width;
    if (sign
#ifdef _VFPF_MANSIGN
			|| (flags & PLUS_FLAG)
#endif
			) {
		sign = sign ? '-' : '+';
    }
#if defined(_VFPF_SPCSIGN)
	else if (flags & SPACE_FLAG) {
		sign = ' ';
	}
#endif
    /* Catch infinities, NaNs here */
    if (isinf(g)) {
		if (sign) {
			dbuf[0] = sign;
			n = 1;
			w--;
		}
#ifdef _VFPF_UPCASE
        if (c) {
            strcpy(&dbuf[n], "INF");
        }
		else
#endif
		{
            strcpy(&dbuf[n], "inf");
        }
        w -= CSTRLEN("inf");
        return _COUNT_CAST pad(fp, &dbuf[0], w);
    }
    if (isnan(g)) {
		if (sign) {
			dbuf[0] = sign;
			n = 1;
			w--;
		}
#ifdef _VFPF_UPCASE
        if (c) {
            strcpy(&dbuf[n], "NAN");
        }
		else
#endif
		{
            strcpy(&dbuf[n], "nan");
        }
        w -= CSTRLEN("nan");
        return _COUNT_CAST pad(fp, &dbuf[0], w);
    }

    /* First find the largest power of 2 not larger than number to print */
    u = 1.0;
    e = 0;
    if (!(g == 0.0)) {
        while (!(g < (u*2.0))) {
            u = u*2.0;
            ++e;
        }
        while (g < u) {
            u = u/2.0;
            --e;
        }
    }

    /* Get precision */
    p = (prec < 0) ? (DBLEN-EXPLEN-1) : prec;

    /* Hex places, total */
    m = p + 1;

    /* Go through the conversion once to get to the rounding step */
    i = 0;
    h = g;
    ou = u;
    while (i < m) {
        l = floor(h/u);
        d = (int)l;
        h -= l*u;
        u = u/16.0;
        ++i;
    }
    
    /* Remainder >= halfway ? */
    l = u*8.0;
    if (h < l) {
        l = 0.0;
    } else {
        /* On tie choose even number */
        if ((h == l) && !(d % 2)) {
            l = 0.0;
        }
    }

    /* Round */
    h = g + l;
    
    /* Convert again, after rounding */
    u = ou;
    ne = 0;
    pp = 0;
    t = 0;
    i = 0;
    while ((i < m) && (n < (DBLEN - EXPLEN))) {
        l = floor(h/u);
        d = (int)l;
        if (!d && (ne < 0)
#ifdef _VFPF_ALTERN
				&& !(flags & POUND_FLAG)
#endif
				) {
            ++t;
        } else {
            if (!pp && (ne < 0)) {
                dbuf[n++] = '.';
                --w;
                pp = 1;
            }
            while (t) {
                dbuf[n++] = '0';
                --w;
                --t;
            }
            d = (d < 10) ? (int)'0' + d : (int)'a' + (d - 10);
#ifdef _VFPF_UPCASE
            if (c && isalpha(d)) {
                d = toupper(d);
            }
#endif
            dbuf[n++] = (char)d;
            --w;
        }
        h -= l*u;
        u = u/16.0;
        --ne;
        ++i;
		if (prec < 0 && h == 0) {
			break;
		}
    }
    /* the above code defers writing zeros and the decimal point after the decimal
       place until a non-zero digit is seen, which fails to handle
       explicit precision modifiers correctly. rectify that here. */
	if (!pp && t != 0 && prec > 0 && (n < (DBLEN - EXPLEN))) {
		pp = 1;
		dbuf[n++] = '.';
		--w;
	}
	/* printf trailing zeros */
	while ((t != 0) && (n < (DBLEN - EXPLEN))) {
		dbuf[n++] = '0';
		--w;
		--t;
	}
#ifdef _VFPF_ALTERN
    if (!pp && (flags & POUND_FLAG)) {
        dbuf[n++] = '.';
    }
#endif
    dbuf[n] = '\0';

    /* Convert exponent */
    i = sizeof(dbuf) - 1;
    dbuf[i] = '\0';
    esign = 0;
    if (e < 0) {
        esign = 1;
        e = -e;
    }
    p = 1;
    while (e || (0 < p)) {
        --i;
        dbuf[i] = '0' + (e % 10);
        e = e / 10;
        --p;
        --w;
    }
    --i;
    dbuf[i] = esign ? '-' : '+';
    --w;
    --i;
#ifdef _VFPF_UPCASE
    dbuf[i] = c ? 'P' : 'p';
#else
    dbuf[i] = 'p';
#endif
    --w;

	// Attach mantissa to exponent
    memcpy(&dbuf[i-n], &dbuf[0], (size_t)n);
	n = i-n;
	i = sign ? 3 /* sign and 0x chars */: 2 /* 0x chars */;
#ifdef _VFPF_FILL
	// fill with zeros before 0x
	if (flags & ZERO_FLAG) {
		while (w > i && n > i) {
			dbuf[--n] = '0';
			w--;
		}
	}
#endif
	// add hex prefix
#ifdef _VFPF_UPCASE
    dbuf[--n] = c ? 'X' : 'x';
#else
    dbuf[--n] = 'x';
#endif
    dbuf[--n] = '0';
	// put on the sign
	if (sign) {
		dbuf[--n] = sign;
	}
	w -= i;

    /* Put out padded string */
    return _COUNT_CAST pad(fp, &dbuf[n], w);
}
#endif

#if defined(_VFPF_C) && !defined(_VFPF_SIMPLE_C)
static _INLINE _COUNT_TYPE ctoa(FILE *fp, char c)
{
    int l,w;

    /* Get width */
    w = width ? width - 1 : width;

#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
    /* Left justify ? Put out character */
    if (flags & MINUS_FLAG) {
        fputc(c, fp);
    }
#endif

    /* Put out padding */
    w = (w < 0) ? 0 : w;
    l = 0;
    while (l < w) {
        fputc(' ', fp);
        ++l;
    }
    /* Right justify ? Put out string */
#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
    if (!(flags & MINUS_FLAG)) {
#endif
        fputc(c, fp);
#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
    }
#endif

#ifdef _VFPF_COUNT_CHARS
    return l+1;
#endif
}
#endif

#if defined(_VFPF_D) && !defined(_VFPF_SIMPLE_D)
static _INLINE _COUNT_TYPE dtoa(FILE *fp, vfpf_sint_t d)
{
	char s;
    int i, w;
#ifdef _VFPF_PRECISION
	int p;
#else
	bool p = true;
#endif

    /* Record sign */
    s = d < 0 ? 1 : 0;

#if defined(_VFPF_PRECISION)
#if defined(_VFPF_FILL)
    /* Adjust flags, precision, width */
    if (!(prec < 0)) {
        flags &= ~ZERO_FLAG;
    }
#endif
    p = (0 <= prec) ? prec : 1;
#endif
    w = width;
    if (s
#ifdef _VFPF_MANSIGN
			|| (flags & PLUS_FLAG)
#endif
	   ) {
        --w;
		s = s ? '-' : '+';
    }
#if defined(_VFPF_SPCSIGN)
	else if (flags & SPACE_FLAG) {
        --w;
		s = ' ';
	}
#endif

    /* Convert to decimal, possibly filling on the left with zeroes */
    i = sizeof(dbuf) - 1;
    dbuf[i] = '\0';
    while (!(i < 1) && (d != 0
#ifdef _VFPF_PRECISION
				|| (0 < p)
#else
				|| p
#endif
#ifdef _VFPF_FILL
				|| ((0 < w) && (flags & ZERO_FLAG))
#endif
				)) {
        --i;
        dbuf[i] = (char)((int)'0' + abs(d % 10));
#ifdef _VFPF_PRECISION
        --p;
#else
		p = false;
#endif
        --w;
        d = d / 10;
    }

    /* Display sign if required */
    if (s != 0) {
        --i;
        dbuf[i] = s;
    }

    /* Put out padded string */
    return _COUNT_CAST pad(fp, &dbuf[i], w);
}
#endif

#if defined(_VFPF_E) || defined(_VFPF_F) || defined(_VFPF_G)
static _INLINE _COUNT_TYPE efgtoa(FILE *fp, long double f, char c)
{
    char mode, nmode, pp, sign, esign;
    int d, e, i, m, n, ne, p, t, w;
    long double h, l;
	union flui g, u, ou;

    /* Record sign, get absolute value */
    sign = 0;
    g.f = f;
    if (g.f < 0.0) {
        sign = 1;
        g.f = -g.f;
    }

    /* Print sign */
    n = 0;
    w = width;
    if (sign
#ifdef _VFPF_MANSIGN
			|| (flags & PLUS_FLAG)
#endif
			) {
		sign = sign ? '-' : '+';
    }
#if defined(_VFPF_SPCSIGN)
	else if (flags & SPACE_FLAG) {
		sign = ' ';
	}
#endif

    /* Catch infinities, NaNs here */
    if (isinf(g.f)) {
		if (sign) {
			dbuf[0] = sign;
			w--;
			n = 1;
		}
#ifdef _VFPF_UPCASE
        if (isupper((int)c)) {
            strcpy(&dbuf[n], "INF");
        }
		else
#endif
		{
            strcpy(&dbuf[n], "inf");
        }
        w -= CSTRLEN("inf");
        return _COUNT_CAST pad(fp, &dbuf[0], w);
    }
    if (isnan(g.f)) {
		if (sign) {
			dbuf[0] = sign;
			w--;
			n = 1;
		}
#ifdef _VFPF_UPCASE
        if (isupper((int)c)) {
            strcpy(&dbuf[n], "NAN");
        }
		else
#endif
		{
            strcpy(&dbuf[n], "nan");
        }
        w -= CSTRLEN("nan");
        return _COUNT_CAST pad(fp, &dbuf[0], w);
    }

    /* First find the largest power of 10 not larger than number to print */
    u.f = 1.0;
    e = 0;
	if (!(g.f == 0.0)) {
		while (FNEARLYEQ(g,(ou.f = u.f*10.0,ou)) || g.f > ou.f) {
			u = ou;
			++e;
		}
		if (FNEARLYEQ(g,u)) {
			g = u;
		}
		else {
			while (g.f < u.f) {
				u.f = u.f/10.0;
				--e;
				if (FNEARLYEQ(g,u)) {
					g = u;
					break;
				}
			}
		}
    }

    /* Get mode, precision */
#ifdef _VFPF_UPCASE
    mode = (char)tolower((int)c);
#else
	mode = c;
#endif
    nmode = mode;
#ifdef _VFPF_G
    if (mode == 'g') {
		if (prec == 0) {
			prec = 1;
		}
        p = (0 < prec) ? prec : 6;
    }
	else
#endif
	{
        p = (prec < 0) ? 6 : prec;
    }

#ifdef _VFPF_G
    /* Choose e or f mode from g mode */
    if (mode == 'g') {
        if (!(e < -4) && !((p - 1) < e)) {
            nmode = 'f';
        } else {
            nmode = 'e';
        }
    }
#endif

    /* Decimal places or significant digits */
    m = p;
#ifdef _VFPF_G
    if (!(mode == 'g') || ((nmode == 'f') && (e < 0)))
#endif
	{
        ++m;
    }

    /* Adjust starting exponent, string length for 'f' conversions */
#if defined(_VFPF_F) || defined(_VFPF_G)
    if (nmode == 'f') {
        if (e < 0) {
            u.f = 1.0;
            e = 0;
        }
        if (!(mode == 'g')) {
            m += e;
        }
    }
#endif

    /* Go through the conversion once to get to the rounding step */
    i = 0;
    h = g.f;
    ou = u;
    while (i < m) {
        l = floor(h/u.f);
        d = (int)l;
        h -= l*u.f;
        u.f = u.f/10.0;
        ++i;
    }
    
    /* Remainder >= halfway ? */
    l = u.f*5.0;
    if (h < l) {
        l = 0.0;
    } else {
        /* On tie choose even number */
        if ((h == l) && !(d % 2)) {
            l = 0.0;
        }
    }

    /* Round */
    h = g.f + l;
    /* Has rounding increased the power above 10^0? */
	if (h >= (ou.f*10.0)) {
		e++;
		ou.f *= 10.0;
#if defined(_VFPF_F) || defined(_VFPF_G)
		if (nmode == 'f') {
			// the increase in power will only affect the number of digits in 'f' mode
			m++;
		}
#endif
	}
    
    /* Convert again, after rounding */
    u = ou;
#if defined(_VFPF_G) || defined(_VFPF_E)
    ne = (nmode == 'e') ? 0 : e;
#else
	ne = e;
#endif
    pp = 0;
    t = 0;
    i = 0;
    while ((i < m) && (n < (DBLEN - EXPLEN))) {
        l = floor(h/u.f);
        d = (int)l > 9 ? 9 : (int)l;
#ifdef _VFPF_G
        if (!d && (mode == 'g') && (ne < 0)
#ifdef _VFPF_ALTERN
				&& !(flags & POUND_FLAG)
#endif
				) {
            ++t;
        }
		else
#endif
		{
            if (!pp && (ne < 0)) {
                dbuf[n++] = '.';
                --w;
                pp = 1;
            }
            while (t) {
                dbuf[n++] = '0';
                --w;
                --t;
            }
            dbuf[n++] = (char)((int)'0' + d);
            --w;
        }
        h -= l*u.f;
        u.f = u.f/10.0;
        --ne;
        ++i;
    }
#ifdef _VFPF_ALTERN
    if (!pp && (flags & POUND_FLAG)) {
        dbuf[n++] = '.';
    }
#endif
	i = sizeof(dbuf) - 1;
    dbuf[i] = '\0';

    /* Convert exponent */
#if defined(_VFPF_G) || defined(_VFPF_E)
    if (nmode == 'e') {
        esign = 0;
        if (e < 0) {
            esign = 1;
            e = -e;
        }
        p = 2;
        while (e || (0 < p)) {
            --i;
            dbuf[i] = '0' + (e % 10);
            e = e / 10;
            --p;
            --w;
        }
        --i;
        dbuf[i] = esign ? '-' : '+';
        --w;
        --i;
#ifdef _VFPF_UPCASE
        dbuf[i] = isupper((int)c) ? 'E' : 'e';
#else
		dbuf[i] = 'e';
#endif
        --w;
    }
#endif
	// Attach mantissa to exponent (if any)
	memcpy(&dbuf[i-n], &dbuf[0], (size_t)n);
	n = i-n;
	i = sign == 0 ? 0 : 1;
#ifdef _VFPF_FILL
	// fill with zeros before 0x
	if (flags & ZERO_FLAG) {
		while (w > i && n > i) {
			dbuf[--n] = '0';
			w--;
		}
	}
#endif
	// put on the sign
	if (sign) {
		dbuf[--n] = sign;
	}
	w -= i;

    /* Put out padded string */
    return _COUNT_CAST pad(fp, &dbuf[n], w);
}
#endif

#if defined(_VFPF_O) && !defined(_VFPF_SIMPLE_O)
static _INLINE _COUNT_TYPE otoa(FILE *fp, vfpf_uint_t d)
{
#ifdef _VFPF_ALTERN
	char t;
#endif
    int i, w;
#ifdef _VFPF_PRECISION
	int p;
#else
	bool p = true;
#endif

#if defined(_VFPF_PRECISION)
#if defined(_VFPF_FILL)
    /* Adjust flags, precision, width */
    if (!(prec < 0)) {
        flags &= ~ZERO_FLAG;
    }
#endif
    p = (0 <= prec) ? prec : 1;
#endif
    w = width;

    /* Convert to octal, possibly filling on the left with zeroes */
    i = sizeof(dbuf) - 1;
    dbuf[i] = '\0';
#ifdef _VFPF_ALTERN
    t = 0;
#endif
    while (!(i < 1) && (d != 0
#ifdef _VFPF_PRECISION
				|| (0 < p)
#else
				|| p
#endif
#ifdef _VFPF_FILL
				|| ((0 < w) && (flags & ZERO_FLAG))
#endif
				)) {
        --i;
#ifdef _VFPF_ALTERN
        t = d & 07;
        dbuf[i] = (char)((int)'0' + t);
#else
        dbuf[i] = (char)((int)'0' + (d & 07));
#endif
#ifdef _VFPF_PRECISION
        --p;
#else
		p = false;
#endif
        --w;
        d = d >> 3;
    }

#ifdef _VFPF_ALTERN
    /* Display prefix if required */
    if ((t || prec == 0) && (flags & POUND_FLAG)) {
        --i;
        dbuf[i] = '0';
        --w;
    }
#endif

    /* Put out padded string */
    return _COUNT_CAST pad(fp, &dbuf[i], w);
}
#endif

#if defined(_VFPF_S) && !defined(_VFPF_SIMPLE_S)
static _INLINE _COUNT_TYPE stoa(FILE *fp, char *s)
{
    char *cp;
    int l, p;
#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
	int w;
#endif

    /* Check for null string */
    cp = s;
    if (!cp) {
        cp = "(null)";
    }

    /* Get length, precision, width */
	for (l = 0; cp[l] != '\0'; l++) {
		continue;
	}
#ifdef _VFPF_PRECISION
	if (!(prec < 0) && (prec < l)) {
		l = prec;
	}
#endif
    p = l;

    /* Right justify, pad on left ? */
#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
    w = width;
    if (!(flags & MINUS_FLAG)) {
        while (l < w) {
            fputc(' ', fp);
            ++l;
        }
    }
#endif

    /* Put out string */
    while (p--) {
        fputc(*cp, fp);
        ++cp;
    }

#if defined(_VFPF_LEFT) || defined(_VFPF_WIDTH)
    /* Left justify, pad on right ? */
    if (flags & MINUS_FLAG) {
        while (l < w) {
            fputc(' ', fp);
            ++l;
        }
    }
#endif

#ifdef _VFPF_COUNT_CHARS
    return l;
#endif
}
#endif

#if defined(_VFPF_U) && !defined(_VFPF_SIMPLE_U)
static _INLINE _COUNT_TYPE utoa(FILE *fp, vfpf_uint_t d)
{
    int i, w;
#ifdef _VFPF_PRECISION
	int p;
#else
	bool p = true;
#endif

    /* Adjust flags, precision, width */
#if defined(_VFPF_PRECISION)
#if defined(_VFPF_FILL)
    if (!(prec < 0)) {
        flags &= ~ZERO_FLAG;
    }
#endif
    p = (0 <= prec) ? prec : 1;
#endif
    w = width;

    /* Convert to decimal, possibly filling on the left with zeroes */
    i = sizeof(dbuf) - 1;
    dbuf[i] = '\0';
    while (i && (d != 0
#ifdef _VFPF_PRECISION
				|| (0 < p)
#else
				|| p
#endif
#ifdef _VFPF_FILL
				|| ((0 < w) && (flags & ZERO_FLAG))
#endif
				)) {
        --i;
        dbuf[i] = '0' + (d % 10);
#ifdef _VFPF_PRECISION
        --p;
#else
		p = false;
#endif
        --w;
        d = d / 10;
    }

    /* Put out padded string */
    return _COUNT_CAST pad(fp, &dbuf[i], w);
}
#endif

#if (defined(_VFPF_HEX_CONV) && !defined(_VFPF_SIMPLE_HEX)) || defined(_VFPF_P)
#if defined(_VFPF_X) && (defined(_VFPF_x) || defined(_VFPF_P))
static _INLINE _COUNT_TYPE xtoa(FILE *fp, vfpf_uint_t d, char x)
#define _XTOA(fp,d,x)	xtoa(fp,d,x)
#else
static _INLINE _COUNT_TYPE xtoa(FILE *fp, vfpf_uint_t d)
#define _XTOA(fp,d,x)	xtoa(fp,d)
#endif
{
	char c;
#if (defined(_VFPF_x) || defined(_VFPF_P)) && defined(_VFPF_X)
	char a;
#endif
    int i, w;
#ifdef _VFPF_PRECISION
	int p;
#else
	bool p = true;
#endif
#if _VFPF_ALTERN
	bool d_nonzero = false;
#endif

    /* Adjust, flags, precision, width */
#if defined(_VFPF_PRECISION)
#ifdef _VFPF_FILL
    if (!(prec < 0)) {
        flags &= ~ZERO_FLAG;
    }
#endif
    p = (0 <= prec) ? prec : 1;
#endif
    w = width;
#ifdef _VFPF_ALTERN
	d_nonzero = d != 0;
    if ((flags & POUND_FLAG) && d_nonzero) {
        w -= 2;
    }
#endif

#if (defined(_VFPF_x) || defined(_VFPF_P)) && defined(_VFPF_X)
	a = x == 'x' ? 'a' : 'A';
#endif

    /* Convert to hexadecimal, possibly filling on the left with zeroes */
    i = sizeof(dbuf) - 1;
    dbuf[i] = '\0';
    while (!(i < 2) && (d != 0
#ifdef _VFPF_PRECISION
				|| (0 < p)
#else
				|| p
#endif
#ifdef _VFPF_FILL
				|| ((0 < w) && (flags & ZERO_FLAG))
#endif
				)) {
        --i;
        c = d & 0x0f;
		if (c < 10) {
			c += '0';
		}
		else {
#if (defined(_VFPF_x) || defined(_VFPF_P)) && defined(_VFPF_X)
			c += a - 10;
#elif (defined(_VFPF_x) || defined(_VFPF_P))
			c += 'a' - 10;
#else
			c += 'A' - 10;
#endif
		}
		dbuf[i] = c;
#ifdef _VFPF_PRECISION
        --p;
#else
		p = false;
#endif
        --w;
        d = d >> 4;
    }

#ifdef _VFPF_ALTERN
    /* Display prefix if required */
    if ((flags & POUND_FLAG) && d_nonzero
#ifdef _VFPF_PRECISION
			&& prec != 0
#endif
	   ) {
        --i;
#if defined(_VFPF_X) && (defined(_VFPF_x) || defined(_VFPF_P))
        dbuf[i] = x;
#elif defined(_VFPF_X)
		dbuf[i] = 'X';
#else
		dbuf[i] = 'x';
#endif
        --i;
        dbuf[i] = '0';
    }
#endif

    /* Put out padded string */
    return _COUNT_CAST pad(fp, &dbuf[i], w);
}
#endif

#if defined(_VFPF_WIDTH) || defined(_VFPF_PRECISION)
static int
read_prec_or_width (const char **fmt, va_list *ap) {
    int n = 0;
    if ((*fmt)[0] == '*') {
        ++*fmt;
        n = va_arg(*ap, int);
    } else {
        unsigned char c;
        while ((c = ((unsigned)(*fmt)[0]) - '0') < 10) {
            n = n * 10 + c;
            ++*fmt;
        }
    }
    return n;
}
#endif

/* Consume and convert the next part of the format string */
#ifdef _VFPF_CONVERT
static _INLINE _COUNT_TYPE
vfpfcnvrt(FILE *fp, char *fmt[], va_list ap)
{
    char c, *cp;
    bool done;
#if defined(_VFPF_INT_LENGTH) || defined(_VFPF_FLOAT_CONV)
	union {
#ifdef _VFPF_INT_LENGTH
		vfpf_sint_t	sint;
		vfpf_uint_t	uint;
#endif
		long double f;
	} convarg;
#endif
#if (defined(_VFPF_SIMPLE_S) || defined(_VFPF_SIMPLE_D) || defined(_VFPF_SIMPLE_U) || defined(_VFPF_SIMPLE_O) || defined(_VFPF_SIMPLE_HEX)) && defined(_VFPF_COUNT_CHARS)
	_COUNT_TYPE cnt = 0;
#endif
#if defined(_VFPF_SIMPLE_HEX)
	char x;
#if defined(_VFPF_x) && defined(_VFPF_X)
	char a;
#endif
#endif

    /* Conversion ? */
    if ((*fmt)[0] == '%') {
        ++*fmt;

        flags = width = 0;
        prec = -1;

#ifdef _VFPF_FLAGS
        /* Get flags */
        done = false;
        while (!done) {
            switch ((*fmt)[0]) {
#ifdef _VFPF_LEFT
                case '-' :
                    flags |= MINUS_FLAG;
                    ++*fmt;
                    break;
#endif
#ifdef _VFPF_FILL
                case '0' :
                    flags |= ZERO_FLAG;
                    ++*fmt;
                    break;
#endif
#ifdef _VFPF_MANSIGN
                case '+' :
                    flags |= PLUS_FLAG;
                    ++*fmt;
                    break;
#endif
#ifdef _VFPF_SPCSIGN
                case ' ' :
                    flags |= SPACE_FLAG;
                    ++*fmt;
                    break;
#endif
#ifdef _VFPF_ALTERN
                case '#' :
                    flags |= POUND_FLAG;
                    ++*fmt;
                    break;
#endif
                default:
                    done = true;
                    break;
            }
        }
#if defined(_VFPF_LEFT) && defined(_VFPF_FILL)
        if (flags & MINUS_FLAG) {
            flags &= ~ZERO_FLAG;
        }
#endif
#endif // _VFPF_FLAGS

#ifdef _VFPF_WIDTH
        /* Get field width */
		width = read_prec_or_width(fmt, ap);
		if (width < 0) {
			flags |= MINUS_FLAG;
			width = -width;
		}
#endif

#ifdef _VFPF_PRECISION
        /* Get precision */
        if ((*fmt)[0] == '.') {
            ++*fmt;
			prec = read_prec_or_width(fmt, ap);
		}
#endif

#ifdef _VFPF_C
        /* Character */
        if ((*fmt)[0] == 'c') {
            ++*fmt;
            c = (unsigned char)va_arg(ap, int);
#ifdef _VFPF_SIMPLE_C
			fputc(c, fp);
#ifdef _VFPF_COUNT_CHARS
			return 1;
#else
			return;
#endif
#else
            return _COUNT_CAST ctoa(fp, c);
#endif
        }
#endif

		/* Integers and character count */
#if defined(_VFPF_SIGNED_CONV) || defined(_VFPF_UNSIGNED_CONV) || defined(_VFPF_N)
		cp = *fmt;
#ifdef _VFPF_LENGTH_MOD
		c = *cp;
		switch(c) {
			// determine length modifiers
#if defined(_VFPF_H) || defined(_VFPF_HH)
			case 'h':
				cp++;
#ifdef _VFPF_HH
				if (*cp == 'h') {
					c = 'H';
					cp++;
				}
#endif
				break;
#endif
#if defined(_VFPF_L) || defined(_VFPF_LL)
			case 'l':
				cp++;
#if defined(_VFPF_LL)
				if (*cp == 'l') {
					c = 'L';
					cp++;
				}
#endif
				break;
#endif
#if defined(_VFPF_J) || defined(_VFPF_T) || defined(_VFPF_Z)
#ifdef _VFPF_J
			case 'j':
#endif
#ifdef _VFPF_T
			case 't':
#endif
#ifdef _VFPF_Z
			case 'z':
#endif
				cp++;
				break;
#endif
#ifdef _VFPF_O
			case 'o':
#endif
#ifdef _VFPF_U
			case 'u':
#endif
#ifdef _VFPF_D
			case 'i':
			case 'd':
#endif
#ifdef _VFPF_x
			case 'x':
#endif
#ifdef _VFPF_X
			case 'X':
#endif
#ifdef _VFPF_N
			case 'n':
#endif
				c = 'i';
				break;
			default :
				c = 0;
				break;
		}
		// At this point cp points to the conversion and c specifies the length
#endif // _VFPF_LENGTH_MOD

		/* Signed integers */
#ifdef _VFPF_SIGNED_CONV
		if (*cp == 'd' || *cp == 'i') {
#ifdef _VFPF_LENGTH_MOD
			switch (c) {
				case 'i':
					convarg.sint = (vfpf_sint_t)(int)va_arg(ap, int);
					break;
#ifdef _VFPF_H
				case 'h':
					convarg.sint = (vfpf_sint_t)(short)va_arg(ap, int);
					break;
#endif
#ifdef _VFPF_HH
				case 'H':
					convarg.sint = (vfpf_sint_t)(signed char)va_arg(ap, int);
					break;
#endif
#ifdef _VFPF_L
				case 'l':
					convarg.sint = (vfpf_sint_t)va_arg(ap, long);
					break;
#endif
#ifdef _VFPF_LL
				case 'L':
					convarg.sint = (vfpf_sint_t)va_arg(ap, long long);
					break;
#endif
#ifdef _VFPF_T
				case 't':
					convarg.sint = (vfpf_sint_t)va_arg(ap, ptrdiff_t);
					break;
#endif
#ifdef _VFPF_J
				case 'j':
					convarg.sint = (vfpf_sint_t)va_arg(ap, intmax_t);
					break;
#endif
#ifdef _VFPF_Z
				case 'z':
					convarg.sint = (vfpf_sint_t)va_arg(ap, size_t);
					break;
#endif
			}
#else //!_VFPF_LENGTH_MOD
			convarg.sint = (vfpf_sint_t)(int)va_arg(ap, int);
#endif
			*fmt = cp+1;
#ifdef _VFPF_SIMPLE_D
			c = sizeof(dbuf);
			done = convarg.sint < 0;
			do {
				dbuf[--c] = abs(convarg.sint % 10) + '0';
				convarg.sint /= 10;
#ifdef _VFPF_COUNT_CHARS
				cnt++;
#endif
			} while (convarg.sint != 0 && c != 0);
			if (c != 0 && done) {
				dbuf[--c] = '-';
#ifdef _VFPF_COUNT_CHARS
				cnt++;
#endif
			}
			while (c != sizeof(dbuf)) {
				fputc(dbuf[c++], fp);
			}
#ifdef _VFPF_COUNT_CHARS
			return cnt;
#else
			return;
#endif
#else // !_VFPF_SIMPLE_D
			return _COUNT_CAST dtoa(fp, convarg.sint);
#endif
		}
#endif // _VFPF_SIGNED_CONV

		/* Unsigned integers */
#ifdef _VFPF_UNSIGNED_CONV
		if (0
#ifdef _VFPF_x
				|| *cp == 'x'
#endif
#ifdef _VFPF_X
				|| *cp == 'X'
#endif
#ifdef _VFPF_O
				|| *cp == 'o'
#endif
#ifdef _VFPF_U
				|| *cp == 'u'
#endif
				) {
#ifdef _VFPF_LENGTH_MOD
			switch (c) {
				case 'i':
					convarg.uint = (vfpf_uint_t)(unsigned int)va_arg(ap, unsigned int);
					break;
#ifdef _VFPF_H
				case 'h':
					convarg.uint = (vfpf_uint_t)(unsigned short)va_arg(ap, unsigned int);
					break;
#endif
#ifdef _VFPF_HH
				case 'H':
					convarg.uint = (vfpf_uint_t)(unsigned char)va_arg(ap, unsigned int);
					break;
#endif
#ifdef _VFPF_L
				case 'l':
					convarg.uint = (vfpf_uint_t)va_arg(ap, unsigned long);
					break;
#endif
#ifdef _VFPF_LL
				case 'L':
					convarg.uint = (vfpf_uint_t)va_arg(ap, unsigned long long);
					break;
#endif
#ifdef _VFPF_T
				case 't':
					convarg.uint = (vfpf_uint_t)va_arg(ap, ptrdiff_t);
					break;
#endif
#ifdef _VFPF_J
				case 'j':
					convarg.uint = (vfpf_uint_t)va_arg(ap, uintmax_t);
					break;
#endif
#ifdef _VFPF_Z
				case 'z':
					convarg.uint = (vfpf_uint_t)va_arg(ap, size_t);
					break;
#endif
			}
#else // !_VFPF_LENGTH_MOD
			convarg.uint = (vfpf_uint_t)(unsigned int)va_arg(ap, unsigned int);
#endif
			*fmt = cp+1;
			switch (*cp) {
#ifdef _VFPF_O
				case 'o':
#ifdef _VFPF_SIMPLE_O
					c = sizeof(dbuf);
					do {
						dbuf[--c] = (convarg.uint & 7) + '0';
						convarg.uint >>= 3;
#ifdef _VFPF_COUNT_CHARS
						cnt++;
#endif
					} while (convarg.uint != 0 && c != 0);
					while (c != sizeof(dbuf)) {
						fputc(dbuf[c++], fp);
					}
#ifdef _VFPF_COUNT_CHARS
					return cnt;
#else
					return;
#endif
#else // !_VFPF_SIMPLE_O
					return _COUNT_CAST otoa(fp, convarg.uint);
#endif
#endif // _VFPF_O
#ifdef _VFPF_U
				case 'u':
#ifdef _VFPF_SIMPLE_U
					c = sizeof(dbuf);
					do {
						dbuf[--c] = (convarg.uint % 10) + '0';
						convarg.uint /= 10;
#ifdef _VFPF_COUNT_CHARS
						cnt++;
#endif
					} while (convarg.uint != 0 && c != 0);
					while (c != sizeof(dbuf)) {
						fputc(dbuf[c++], fp);
					}
#ifdef _VFPF_COUNT_CHARS
					return cnt;
#else
					return;
#endif
#else // !_VFPF_SIMPLE_U
					return _COUNT_CAST utoa(fp, convarg.uint);
#endif
#endif // _VFPF_U
#ifdef _VFPF_HEX_CONV
#ifdef _VFPF_X
				case 'X':
#endif
#ifdef _VFPF_x
				case 'x':
#endif
#ifdef _VFPF_SIMPLE_HEX
					c = sizeof(dbuf);
#if defined(_VFPF_x) && defined(_VFPF_X)
					a = *cp == 'x' ? 'a' : 'A';
#endif
					do {
						x = (convarg.uint & 0xf);
						if (x < 10) {
							x += '0';
						}
						else {
#if defined(_VFPF_X) && defined(_VFPF_x)
							x += a - 10;
#elif defined(_VFPF_x)
							x += 'a' - 10;
#else
							x += 'A' - 10;
#endif
						}
						dbuf[--c] = x;
						convarg.uint >>= 4;
#ifdef _VFPF_COUNT_CHARS
						cnt++;
#endif
					} while (convarg.uint != 0 && c != 0);
					while (c != sizeof(dbuf)) {
						fputc(dbuf[c++], fp);
					}
#ifdef _VFPF_COUNT_CHARS
					return cnt;
#else
					return;
#endif
#else // !_VFPF_SIMPLE_HEX
					return _COUNT_CAST _XTOA(fp, convarg.uint, *cp);
#endif
#endif // _VFPF_HEX_CONV
			}
		}
#endif // _VFPF_UNSIGNED_CONV

		/* Character count */
#ifdef _VFPF_N
		if (*cp == 'n') {
#ifdef _VFPF_LENGTH_MOD
			switch (c) {
				case 'i':
					*va_arg(ap, int*) = (int) nout;
					break;
#ifdef _VFPF_H
				case 'h':
					*va_arg(ap, short*) = (short) nout;
					break;
#endif
#ifdef _VFPF_HH
				case 'H':
					*va_arg(ap, signed char*) = (signed char) nout;
					break;
#endif
#ifdef _VFPF_L
				case 'l':
					*va_arg(ap, long*) = (long) nout;
					break;
#endif
#ifdef _VFPF_LL
				case 'L':
					*va_arg(ap, long long*) = (long long) nout;
					break;
#endif
#ifdef _VFPF_T
				case 't':
					*va_arg(ap, ptrdiff_t*) = (ptrdiff_t) nout;
					break;
#endif
#ifdef _VFPF_J
				case 'j':
					*va_arg(ap, intmax_t*) = (intmax_t) nout;
					break;
#endif
#ifdef _VFPF_Z
				case 'z':
					*va_arg(ap, size_t*) = (size_t) nout;
					break;
#endif
			}
#else // !_VFPF_LENGTH_MOD
			*va_arg(ap, int*) = (int) nout;
#endif
			*fmt = cp+1;
			return _COUNT_CAST 0;
		}
#endif // _VFPF_N
#endif // _VFPF_SIGNED_CONV || _VFPF_UNSIGNED_CONV || _VFPF_N

#ifdef _VFPF_P
        /* Pointer */
        if ((*fmt)[0] == 'p') {
            ++*fmt;
            return _COUNT_CAST _XTOA(fp, (vfpf_uint_t)(uintptr_t)va_arg(ap, void *), 'x');
        }
#endif

#ifdef _VFPF_S
		/* String */
		done = false;
		if ((*fmt)[0] == 's') {
			++*fmt;
			done = true;
		}
		else if ((*fmt)[0] == 'l' && (*fmt)[1] == 'l' && (*fmt)[2] == 's') {
			*fmt += 3;
			done = true;
		}
		if (done) {
#ifdef _VFPF_SIMPLE_S
			// Avoid mixing targets in the va_list into those of the character pointer (cp)
			// in the outer block.  Doing so will blow-out the size of the outer cp.
			for (char * cp = va_arg(ap, char *); *cp != '\0'; cp++) {
				fputc(*cp, fp);
#ifdef _VFPF_COUNT_CHARS
				cnt++;
#endif
			}
#ifdef _VFPF_COUNT_CHARS
			return cnt;
#else
			return;
#endif

#else // ! _VFPF_SIMPLE_S
			return _COUNT_CAST stoa(fp, va_arg(ap, char *));
#endif
		}
#endif

		/* Floats */
#ifdef _VFPF_FLOAT_CONV
		cp = *fmt;
		c = *cp;
		switch(c) {
			// length modifiers
			case 'l':
			case 'L':
				cp++;
				break;
#ifdef _VFPF_A
			case 'a':
#ifdef _VFPF_UPCASE
			case 'A':
#endif
#endif
#ifdef _VFPF_E
			case 'e':
#ifdef _VFPF_UPCASE
			case 'E':
#endif
#endif
#ifdef _VFPF_F
			case 'f':
#ifdef _VFPF_UPCASE
			case 'F':
#endif
#endif
#ifdef _VFPF_G
			case 'g':
#ifdef _VFPF_UPCASE
			case 'G':
#endif
#endif
				c = 'l';
				break;
			default:
				c = 0;
				break;
		}
		// at this point cp points to the conversion and c specifies the length
		if (0
#ifdef _VFPF_A
				|| *cp == 'a'
#ifdef _VFPF_UPCASE
				|| *cp == 'A'
#endif
#endif
#ifdef _VFPF_E
				|| *cp == 'e'
#ifdef _VFPF_UPCASE
				|| *cp == 'E'
#endif
#endif
#ifdef _VFPF_F
				|| *cp == 'f'
#ifdef _VFPF_UPCASE
				|| *cp == 'F'
#endif
#endif
#ifdef _VFPF_G
				|| *cp == 'g'
#ifdef _VFPF_UPCASE
				|| *cp == 'G'
#endif
#endif
				) {
			switch (c) {
				case 'l':
					convarg.f = (long double)va_arg(ap, double);
					break;
				case 'L':
					convarg.f = (long double)va_arg(ap, long double);
					break;
			}
			*fmt = cp+1;
			switch (*cp) {
#ifdef _VFPF_A
				case 'a':
#ifdef _VFPF_UPCASE
				case 'A':
#endif
					return _COUNT_CAST _ATOA(fp, convarg.f, *cp);
#endif
#if defined(_VFPF_E) || defined(_VFPF_F) || defined(_VFPF_G)
#ifdef _VFPF_E
				case 'e':
#ifdef _VFPF_UPCASE
				case 'E':
#endif
#endif
#ifdef _VFPF_F
				case 'f':
#ifdef _VFPF_UPCASE
				case 'F':
#endif
#endif
#ifdef _VFPF_G
				case 'g':
#ifdef _VFPF_UPCASE
				case 'G':
#endif
#endif
					return _COUNT_CAST efgtoa(fp, convarg.f, *cp);
#endif
			}
		}
#endif // _VFPF_FLOAT_CONV

#ifdef _VFPF_PERC
        /* 'Escaped' '%' character */
        if ((*fmt)[0] == '%') {
            ++*fmt;
            fputc((int)'%', fp);
            return _COUNT_CAST 1;
        }
#endif

        /* Unrecognized conversion */
        ++*fmt;
        return _COUNT_CAST 0;
    }

    /* No conversion, just intervening text */
    fputc((int)(*fmt)[0], fp);
    ++*fmt;
    return _COUNT_CAST 1;
}
#endif

int vfprintf(FILE *fp, const char *fmt, va_list ap)
{
#ifdef _VFPF_CONVERT
    char *cfmt;

    cfmt = (char *)fmt;
#ifdef _VFPF_COUNT_CHARS
    nout = 0;
#endif
    while (*cfmt) {
#ifdef _VFPF_COUNT_CHARS
        nout +=
#endif
			vfpfcnvrt(fp, &cfmt, ap);
    }
#ifdef _VFPF_RETVALUE
    return nout;
#else
	return 0;
#endif
#else
    return fputs(fmt, fp);
#endif
}
