/* vfscanf with configurable support for format conversions */
/* This code is specifically for XC8 */
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inline.h>
#include <stdbool.h>
#include <limits.h>

/* Configuration options */
#ifdef _VFSF_ALL
#define _VFSF_CONVERT
#define _VFSF_FLAGS
#define _VFSF_WIDTH
#define _VFSF_PRECISION

#define _VFSF_HH
#define _VFSF_H
#define _VFSF_L
#define _VFSF_LL
#define _VFSF_J
#define _VFSF_T
#define _VFSF_Z

#define _VFSF_A
#define _VFSF_C
#define _VFSF_D
#define _VFSF_E
#define _VFSF_F
#define _VFSF_G
#define _VFSF_I
#define _VFSF_O
#define _VFSF_N
#define _VFSF_P
#define _VFSF_S
#define _VFSF_U
#define _VFSF_X
#define _VFSF_CHARSEQ
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)	(sizeof(a)/sizeof(a[0]))
#endif
#ifndef CSTRLEN
#define CSTRLEN(s)	(ARRAYSIZE(s)-1)
#endif

/* Assignment suppression, width */
static bool asup;
static int width;

/* Converted argument, matched character counts */
static int ncnv, nmatch;

#ifdef _VFSF_CONVERT
/* Input that would exceed buffer capacity is truncated */
#if defined(_PIC12)
#define DBLEN 16
#else
#if defined(_VFSF_A) || defined(_VFSF_E) || defined(_VFSF_F) || defined(_VFSF_G)
#define DBLEN 80
#else
#define DBLEN 32
#endif
#endif
static char dbuf[DBLEN];

/* Skip over white space */
static void _INLINE skipws(FILE *fp)
{
    int c;

    while ((c = fgetc(fp)) != EOF) {
        if (!isspace(c)) {
            ungetc(c, fp);
            break;
        }
    }
}
#endif

#ifdef _VFSF_C
static _INLINE int atoc(FILE *fp, char *s)
{
    int c, i, w;

    /* Get the characters */
    w = width ? width : 1;
    i = 0;
    while (i < w) {
        c = fgetc(fp);
        if (c == EOF && (width || i==0)) {
            return c;
        }
        if (s) {
            s[i] = (char)c;
        }
        ++i;
        ++nmatch;
    }

    return s != NULL;
}
#endif

#ifdef _VFSF_D
static _INLINE int atod(FILE *fp, long long *pll)
{
    char *ep;
    int c, i, w;

    /* Skip white space */
    skipws(fp);

    /* Get the string */
    w = width ? width : sizeof(dbuf) - 1;
    i = 0;
    while ((i < w) && (i < (sizeof(dbuf) - 1))) {
        c = fgetc(fp);
        if (c == EOF && (width || i==0)) {
            return c;
        }
        if (!i && ((c == '+') || (c == '-'))) {
            dbuf[i] = (char)c;
            ++i;
            ++nmatch;
            continue;
        }
        if (!isdigit(c)) {
			if (c != EOF) {
				ungetc(c, fp);
			}
            break;
        }
        dbuf[i] = (char)c;
        ++i;
        ++nmatch;
    }
    dbuf[i] = '\0';

    /* Convert */
#if __SIZEOF_LONG_LONG__ == 8
    *pll = strtoll(&dbuf[0], &ep, 10);
#else
    *pll = strtol(&dbuf[0], &ep, 10);
#endif

    return (errno || !i) ? EOF - 1 : !asup;
}
#endif

#ifdef _VFSF_I
static _INLINE int _atoi(FILE *fp, long long *pll)
{
	enum {
		SEEN_SIGN = 0x1,
		TEST_FIRST_ZERO = 0x2,
	};
    char *ep, flags = 0, base = 0;
    int c, i, w, temp;


    /* Skip white space */
    skipws(fp);

    /* Get the string */
    w = width ? width : sizeof(dbuf) - 1;
	if (w > (sizeof(dbuf) - 1)) {
		w = sizeof(dbuf) - 1;
	}
    i = 0;

	while (i < w) {
		c = fgetc(fp);
		if (c == EOF && (width || i==0)) {
			return c;
		}
		switch (c) {
			// check for sign
			case '-':
			case '+':
				temp = fgetc(fp);
				ungetc(temp, fp);
				if (!isdigit(temp)) {
					goto _atoi_ungetc_exit;
				}
				flags |= SEEN_SIGN;
				break;
			// is this a loading zero?
			case '0':
				if (((flags & SEEN_SIGN) && i == 1) || i == 0) {
					flags |= TEST_FIRST_ZERO;
				}
				break;
			// might be the beginning of a 0x prefix
			case 'x':
			case 'X':
				if (flags & TEST_FIRST_ZERO) {
					flags &= ~TEST_FIRST_ZERO;
					// look ahead check for hexadecimal characters
					temp = fgetc(fp);
					ungetc(temp, fp);
					if(isxdigit(temp)) {
						base = 16;
					}
					else {
						goto _atoi_ungetc_exit;
					}
				}
				else {
					goto _atoi_ungetc_exit;
				}
				break;
			default:
				if (base == 0) {
					if (flags & TEST_FIRST_ZERO) {
						flags &= ~TEST_FIRST_ZERO;
						base = 8;
					}
					else {
						base = 10;
					}
				}
				switch (base) {
					case 16:
						if (!isxdigit(c)) {
							goto _atoi_ungetc_exit;
						}
						break;
					case 10:
					case 8:
						if (isdigit(c) && ((char)c-'0') < base) {
							break;
						}
						// fall thru
					default:
						goto _atoi_ungetc_exit;
						break;

				} // switch (base)
				break;
		} // switch (c)
		dbuf[i] = (char)c;
		++i;
		++nmatch;
	}
	goto _atoi_exit;
_atoi_ungetc_exit:
	ungetc(c, fp);
_atoi_exit:
    dbuf[i] = '\0';

    /* Convert */
#if __SIZEOF_LONG_LONG__ == 8
    *pll = strtoll(&dbuf[0], &ep, 0);
#else
    *pll = strtol(&dbuf[0], &ep, 0);
#endif

    return (errno || !i) ? EOF - 1 : !asup;
}
#endif

#if defined(_VFSF_A) || defined(_VFSF_E) || defined(_VFSF_F) || defined(_VFSF_G)

/* match inf/nan. returns number of characters matched on success,
   EOF if EOF was reached, or 0 if no match. on error any
   successfully read characters are ungotten. */
static _INLINE int is_inf_nan(FILE *fp, char first, int pos, int n) {
	int c, i, w;
	const char *m[2] = { "inf", "nan" };

	/* early exit if match is impossible in prescribed width. */
	if (n < pos + 3) {
		return 0;
	}
	i = pos;
	w = tolower(first) == 'i' ? 0 : 1;
	dbuf[i++] = first;
	while (i < pos + 3) {
		c = fgetc(fp);
		if (c == EOF) {
			return c;
		}
		if (tolower(c) != m[w][i-pos]) {
			/* match failure with candidate string */
			break;
		}
		dbuf[i++] = (char)c;
	}

	if (i < pos + 3) {
		/* on early exit, undo what we've done */
		ungetc(c, fp);
		while (i > pos) {
			ungetc(dbuf[--i], fp);
		}
		return 0;
	}
	return 3;
}

static _INLINE int atoaefg(FILE *fp, long double *pld)
{
    char *ep;
    int c, e, i, w, s, j, n;
	bool d, hex;

    /* Skip white space */
    skipws(fp);

    /* Get the string */
    w = width ? width : sizeof(dbuf) - 1;
    n = w < (sizeof(dbuf) - 1) ? w : sizeof(dbuf) - 1;
    s = e = i =  0;
	hex = d = s = false;
    while (i < n) {
        c = fgetc(fp);
        if (c == EOF && (width || i==0)) {
            return c;
        }
        if (!s && ((c == '+') || (c == '-'))) {
            /* only allow sign at start or after exponent char */
            if (!(i == 0 || i == e)) {
                ungetc(c, fp);
                break;
            }
            s = 1;
            dbuf[i] = (char)c;
            ++i;
            ++nmatch;
            continue;
        }
        if (!d && (c == '.')) {
            /* no decimal places in exponents! */
            if (e) {
                ungetc(c, fp);
                break;
            }
            d = true;
            dbuf[i] = (char)c;
            ++i;
            ++nmatch;
            continue;
        }
        if (!e && (hex ? (tolower(c) == 'p') : (tolower(c) == 'e'))) {
            e = i + 1; /* set so that we may match sign on exponent */
            s = 0;
            dbuf[i] = (char)c;
            ++i;
            ++nmatch;
            continue;
        }
        if (!e && i == s + 1 && tolower(c) == 'x' && dbuf[i-1] == '0') {
            /* check for 0x prefix immediately following sign. */
            hex = true;
            dbuf[i] = (char)c;
            ++i;
            ++nmatch;
            continue;
        }
        if (!(hex ? isxdigit(c) : isdigit(c))) {
            /* check for inf/nan if immediately following sign char.
               is_inf_nan will return the number of chars written to dbuf,
               or EOF if one was encountered */
			j = tolower(c);
			if (s == i && (j == 'i' || j == 'n')) {
				j = is_inf_nan(fp, (char)c, i, n);
				if (j <= 0) {
					break;
				}
                i += j;
                nmatch += j;
			}
			else {
				ungetc(c, fp);
			}
			break;
        }
        dbuf[i] = (char)c;
        ++i;
        ++nmatch;
    }
    dbuf[i] = '\0';

    /* Convert */
    //printf("%s\n", dbuf);
	e = errno;
	errno = 0;
    *pld = strtold(&dbuf[0], &ep);
	// numbers outside the representable range are still completely scanned, but are reported as [-/+]HUGE_VAL
	d = errno != 0 && errno != ERANGE;
	errno = e;

    return (d || !i || ep == &dbuf[0]) ? EOF - 1 : !asup;
}
#endif

#ifdef _VFSF_O
static _INLINE int atoo(FILE *fp, unsigned long long *pll)
{
    char *ep;
    int c, i, w;

    /* Skip white space */
    skipws(fp);

    /* Get the string */
    w = width ? width : sizeof(dbuf) - 1;
    i = 0;
    while ((i < w) && (i < (sizeof(dbuf) - 1))) {
        c = fgetc(fp);
        if (c == EOF && (width || i==0)) {
            return c;
        }
        if ((c < '0') || ('7' < c)) {
			if (c != EOF) {
				ungetc(c, fp);
			}
            break;
        }
        dbuf[i] = (char)c;
        ++i;
        ++nmatch;
    }
    dbuf[i] = '\0';

    /* Convert */
#if __SIZEOF_LONG_LONG__ == 8
    *pll = strtoull(&dbuf[0], &ep, 8);
#else
    *pll = strtoul(&dbuf[0], &ep, 8);
#endif

    return (errno || !i) ? EOF - 1 : !asup;
}
#endif

#ifdef _VFSF_S
static _INLINE int atos(FILE *fp, char *s)
{
    int c, i, w;

    /* Skip white space */
    skipws(fp);

    /* Get the string */
    w = width;
    i = 0;
    while (!w || (i < w)) {
        c = fgetc(fp);
        if (c == EOF) {
			if (width || i==0) {
				return c;
			}
			break;
        }
        if (isspace(c)) {
            ungetc(c, fp);
            break;
        }
        if (s) {
            s[i] = (char)c;
        }
        ++i;
        ++nmatch;
    }
    if (s) {
        s[i] = '\0';
    }

    return !asup;
}
#endif

#ifdef _VFSF_U
static _INLINE int atou(FILE *fp, unsigned long long *pll)
{
    char *ep;
    int c, i, w;

    /* Skip white space */
    skipws(fp);

    /* Get the string */
    w = width ? width : sizeof(dbuf) - 1;
    i = 0;
    while ((i < w) && (i < (sizeof(dbuf) - 1))) {
        c = fgetc(fp);
        if (c == EOF && (width || i==0)) {
            return c;
        }
        if (!i && ((c == '+') || (c == '-'))) {
            dbuf[i] = (char)c;
            ++i;
            ++nmatch;
            continue;
        }
        if (!isdigit(c)) {
			if (c != EOF) {
				ungetc(c, fp);
			}
            break;
        }
        dbuf[i] = (char)c;
        ++i;
        ++nmatch;
    }
    dbuf[i] = '\0';

    /* Convert */
#if __SIZEOF_LONG_LONG__ == 8
    *pll = strtoull(&dbuf[0], &ep, 10);
#else
    *pll = strtoul(&dbuf[0], &ep, 10);
#endif

    return (errno || !i) ? EOF - 1 : !asup;
}
#endif

#if defined(_VFSF_P) || defined(_VFSF_X)
static _INLINE int atox(FILE *fp, unsigned long long *pll)
{
    char *ep;
    int c, i, w, seen_0x = 0;

    /* Skip white space */
    skipws(fp);

    /* Get the string */
    w = width ? width : sizeof(dbuf) - 1;
    i = 0;
    while ((i < w) && (i < (sizeof(dbuf) - 1))) {
        c = fgetc(fp);
        if (c == EOF && (width || i==0)) {
            return c;
        }
		if (c == '-' || c == '+') {
			if (i > 0) {
				ungetc(c, fp);
				break;
			}
		}
		else if (c == '0') {
			dbuf[i] = (char)c;
			++i;
			++nmatch;
			c = fgetc(fp);
			if (c == 'X' || c == 'x') {
				if (seen_0x) {
					ungetc(c,fp);
					break;
				}
				seen_0x = i;
			}
			else {
				if (c != EOF) {
					ungetc(c, fp);
				}
				continue;
			}
		}
		else if (!isxdigit(c)) {
			if (c != EOF) {
				ungetc(c, fp);
			}
			if (seen_0x && seen_0x == (i-1)) {
				i--;
				nmatch--;
			}
            break;
        }
        dbuf[i] = (char)c;
        ++i;
        ++nmatch;
    }
    dbuf[i] = '\0';

    /* Convert */
#if __SIZEOF_LONG_LONG__ == 8
    *pll = strtoull(&dbuf[0], &ep, 16);
#else
    *pll = strtoul(&dbuf[0], &ep, 16);
#endif

    return (errno || !i || ep == &dbuf[0]) ? EOF - 1 : !asup;
}
#endif

#ifdef _VFSF_CHARSEQ
static _INLINE int atocs(FILE *fp, char *fmt[], va_list ap)
{
	bool invert;
	int start, end, w, i, c, j, k;
	char * s;
	// *fmt points to the first character in the set after the inital '['
	// find the start of the character set
	if ((*fmt)[0] == '^') {
		invert = true;
		start = 1;
	}
	else {
		start = 0;
		invert = false;
	}
	// find the end of the character set
	for (end = start; (*fmt)[end]; end++) {
		if ((*fmt)[end] == ']' && end != start) {
			break;
		}
	}
	// compare the input against the character set
	w = width ? width : INT_MAX; // max input characters we can match
	i = 0; // characters matched
	s = asup ? NULL : va_arg(ap, char*);
	for (;i != w; i++) {
		c = fgetc(fp);
		if (c == EOF) {
			return EOF;
		}
		for (j = start; j != end; j++) {
			if ((*fmt)[j] == '-' && j != start && j != (end-1)) {
				// we have a character range and the first character has been done
				for (k = ((*fmt)[j-1]+1); k < (*fmt)[j+1]; k++) {
					if (k == c) {
						goto atocs_stop_char_set_scan;
					}
				}
			}
			if ((*fmt)[j] == c) {
				goto atocs_stop_char_set_scan;
			}
		}
atocs_stop_char_set_scan:
		if (invert) {
			if (j == end) {
				if (s != NULL) {
					s[i] = (char)c;
					s[i+1] = '\0';
				}
			}
			else {
				ungetc(c, fp);
				break;
			}
		}
		else if (j == end) {
			ungetc(c, fp);
			break;
		}
		else if (s != NULL) {
			s[i] = (char)c;
			s[i+1] = '\0';
		}
	}
	*fmt += end;
	if (**fmt != '\0') {
		(*fmt)++;
	}
	nmatch += i;
	return i ? !asup : 0;
}
#endif

/* Consume and convert the next part of the format string */
static _INLINE int vfsfcnvrt(FILE *fp, char *fmt[], va_list ap)
{
    char *cp, ct[3];
    int c, i;
    long long ll;
    unsigned long long llu;
    long double f;
    void *vp;

    /* Conversion ? */
    if ((*fmt)[0] == '%') {
        ++*fmt;

        asup = width = 0;

#ifdef _VFSF_FLAGS
        /* Get asignment suppression flag */
        if (*fmt[0] == '*') {
            ++*fmt;
            asup = 1;
        }
#endif

#ifdef _VFSF_WIDTH
        /* Get field width */
        width = atoi(*fmt);
        while (isdigit((*fmt)[0])) {
            ++*fmt;
        }
#endif

#if defined(_VFSF_A) || defined(_VFSF_E) || defined(_VFSF_F) || defined(_VFSF_G)
        /* Case-folded conversion types */
        ct[0] = (char)tolower((int)(*fmt)[0]);
        if (ct[0]) {
            ct[1] = (char)tolower((int)(*fmt)[1]);
            if (ct[1]) {
                ct[2] = (char)tolower((int)(*fmt)[2]);
            }
        }
#endif

#ifdef _VFSF_C
        /* Character */
        if (*fmt[0] == 'c') {
#ifdef _PIC14
            cp = "";
#endif
            ++*fmt;
            if (!asup) {
                cp = va_arg(ap, char *);
                return atoc(fp, cp);
            }
            return atoc(fp, NULL);
        }
#endif

#ifdef _VFSF_D
#ifdef _VFSF_HH
        /* Character decimal integer */
        if (!strncmp(*fmt, "hhd", CSTRLEN("hhd"))) {

            *fmt += CSTRLEN("hhd");
            i = atod(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, char *);
                *(char *)vp = (char)ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_H
        /* Short decimal integer */
        if (!strncmp(*fmt, "hd", CSTRLEN("hd"))) {

            *fmt += CSTRLEN("hd");
            i = atod(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, short *);
                *(short *)vp = (short)ll;
            }
            return i;
        }
#endif

        /* Decimal integer */
        if (*fmt[0] == 'd') {

            ++*fmt;
            i = atod(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, int *);
                *(int *)vp = (int)ll;
            }
            return i;
        }

#ifdef _VFSF_L
        /* Long decimal integer */
        if (!strncmp(*fmt, "ld", CSTRLEN("ld"))) {

            *fmt += CSTRLEN("ld");
            i = atod(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, long *);
                *(long *)vp = (long)ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_LL
        /* Long long decimal integer */
        if (!strncmp(*fmt, "lld", CSTRLEN("lld"))) {

            *fmt += CSTRLEN("lld");
            i = atod(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, long long *);
                *(long long *)vp = ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_J
        /* intmax_t decimal integer */
        if (!strncmp(*fmt, "jd", CSTRLEN("jd"))) {

            *fmt += CSTRLEN("jd");
            i = atod(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, intmax_t *);
                *(intmax_t *)vp = (intmax_t)ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_T
        /* ptrdiff_t decimal integer */
        if (!strncmp(*fmt, "td", CSTRLEN("td"))) {

            *fmt += CSTRLEN("td");
            i = atod(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, ptrdiff_t *);
                *(ptrdiff_t *)vp = (ptrdiff_t)ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_Z
        /* size_t decimal integer */
        if (!strncmp(*fmt, "zd", CSTRLEN("zd"))) {

            *fmt += CSTRLEN("zd");
            i = atod(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, size_t *);
                *(size_t *)vp = (size_t)ll;
            }
            return i;
        }
#endif
#endif

#ifdef _VFSF_I
#ifdef _VFSF_HH
        /* Character integer */
        if (!strncmp(*fmt, "hhi", CSTRLEN("hhi"))) {

            *fmt += CSTRLEN("hhi");
            i = _atoi(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, char *);
                *(char *)vp = (char)ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_H
        /* Short integer */
        if (!strncmp(*fmt, "hi", CSTRLEN("hi"))) {

            *fmt += CSTRLEN("hi");
            i = _atoi(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, short *);
                *(short *)vp = (short)ll;
            }
            return i;
        }
#endif
        /* integer */
        if (*fmt[0] == 'i') {

            ++*fmt;
            i = _atoi(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, int *);
                *(int *)vp = (int)ll;
            }
            return i;
        }

#ifdef _VFSF_L
        /* Long integer */
        if (!strncmp(*fmt, "li", CSTRLEN("li"))) {

            *fmt += CSTRLEN("li");
            i = _atoi(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, long *);
                *(long *)vp = (long)ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_LL
        /* Long long integer */
        if (!strncmp(*fmt, "lli", CSTRLEN("lli"))) {

            *fmt += CSTRLEN("lli");
            i = _atoi(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, long long *);
                *(long long *)vp = ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_J
        /* intmax_t integer */
        if (!strncmp(*fmt, "ji", CSTRLEN("ji"))) {

            *fmt += CSTRLEN("jd");
            i = _atoi(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, intmax_t *);
                *(intmax_t *)vp = (intmax_t)ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_T
        /* ptrdiff_t integer */
        if (!strncmp(*fmt, "ti", CSTRLEN("ti"))) {

            *fmt += CSTRLEN("ti");
            i = _atoi(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, ptrdiff_t *);
                *(ptrdiff_t *)vp = (ptrdiff_t)ll;
            }
            return i;
        }
#endif

#ifdef _VFSF_Z
        /* size_t integer */
        if (!strncmp(*fmt, "zi", CSTRLEN("zi"))) {

            *fmt += CSTRLEN("zi");
            i = _atoi(fp, &ll);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, size_t *);
                *(size_t *)vp = (size_t)ll;
            }
            return i;
        }
#endif
#endif


#if defined(_VFSF_A) || defined(_VFSF_E) || defined(_VFSF_F) || defined(_VFSF_G)
        /* 'a', 'e', 'f', or 'g' style floating point */
        if ((ct[0] == 'a') || (ct[0] == 'e') || \
            (ct[0] == 'f') || (ct[0] == 'g')) { 

            ++*fmt;
            i = atoaefg(fp, &f);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, float *);
                *(float *)vp = (float)f;
            }
            return i;
        }
        if (!strncmp(ct, "la", CSTRLEN("le")) || \
            !strncmp(ct, "le", CSTRLEN("lf")) || \
            !strncmp(ct, "lf", CSTRLEN("lf")) || \
            !strncmp(ct, "lg", CSTRLEN("lg"))) {

            i = atoaefg(fp, &f);
            if (!(i < 1)) {
                if (isupper((int)(*fmt)[0])) {
                    vp = (void *)va_arg(ap, long double *);
                    *(long double *)vp = f;
                } else {
                    vp = (void *)va_arg(ap, double *);
                    *(double *)vp = (double)f;
                }
            }
            *fmt += CSTRLEN("la");
            return i;
        }
#endif

#ifdef _VFSF_O
#ifdef _VFSF_HH
        /* Character octal integer */
        if (!strncmp(*fmt, "hho", CSTRLEN("hho"))) {

            *fmt += CSTRLEN("hho");
            i = atoo(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned char *);
                *(unsigned char *)vp = (unsigned char)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_H
        /* Short octal integer */
        if (!strncmp(*fmt, "ho", CSTRLEN("ho"))) {

            *fmt += CSTRLEN("ho");
            i = atoo(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned short *);
                *(unsigned short *)vp = (unsigned short)llu;
            }
            return i;
        }
#endif

        /* Octal integer */
        if (*fmt[0] == 'o') {

            ++*fmt;
            i = atoo(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned int *);
                *(unsigned int *)vp = (unsigned int)llu;
            }
            return i;
        }

#ifdef _VFSF_L
        /* Long octal integer */
        if (!strncmp(*fmt, "lo", CSTRLEN("lo"))) {

            *fmt += CSTRLEN("lo");
            i = atoo(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned long *);
                *(unsigned long *)vp = (unsigned long)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_LL
        /* Long long octal integer */
        if (!strncmp(*fmt, "llo", CSTRLEN("llo"))) {

            *fmt += CSTRLEN("llo");
            i = atoo(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned long long *);
                *(unsigned long long *)vp = llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_J
        /* uintmax_t octal integer */
        if (!strncmp(*fmt, "jo", CSTRLEN("jo"))) {

            *fmt += CSTRLEN("jo");
            i = atoo(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, uintmax_t *);
                *(uintmax_t *)vp = (uintmax_t)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_T
        /* ptrdiff_t octal integer */
        if (!strncmp(*fmt, "to", CSTRLEN("to"))) {

            *fmt += CSTRLEN("to");
            i = atoo(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, ptrdiff_t *);
                *(ptrdiff_t *)vp = (ptrdiff_t)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_Z
        /* size_t octal integer */
        if (!strncmp(*fmt, "zo", CSTRLEN("zo"))) {

            *fmt += CSTRLEN("zo");
            i = atoo(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, size_t *);
                *(size_t *)vp = (size_t)llu;
            }
            return i;
        }
#endif
#endif

        /* Character count */
#ifdef _VFSF_N

#ifdef _VFSF_HH
        if (!strncmp(*fmt, "hhn", CSTRLEN("hhn"))) {

            *fmt += CSTRLEN("hhn");
            vp = (void *)va_arg(ap, char *);
            *(char *)vp = (char)nmatch;
            return 0;
        }
#endif

#ifdef _VFSF_H
        if (!strncmp(*fmt, "hn", CSTRLEN("hn"))) {

            *fmt += CSTRLEN("hn");
            vp = (void *)va_arg(ap, short *);
            *(short *)vp = (short)nmatch;
            return 0;
        }
#endif

        if (*fmt[0] == 'n') {
            ++*fmt;
            vp = (void *)va_arg(ap, int *);
            *(int *)vp = nmatch;
            return 0;
        }

#ifdef _VFSF_L
        if (!strncmp(*fmt, "ln", CSTRLEN("ln"))) {

            *fmt += CSTRLEN("ln");
            vp = (void *)va_arg(ap, long *);
            *(long *)vp = (long)nmatch;
            return 0;
        }
#endif

#ifdef _VFSF_LL
        if (!strncmp(*fmt, "lln", CSTRLEN("lln"))) {

            *fmt += CSTRLEN("lln");
            vp = (void *)va_arg(ap, long long *);
            *(long long *)vp = (long long)nmatch;
            return 0;
        }
#endif

#ifdef _VFSF_J
        if (!strncmp(*fmt, "jn", CSTRLEN("jn"))) {

            *fmt += CSTRLEN("jn");
            vp = (void *)va_arg(ap, uintmax_t *);
            *(uintmax_t *)vp = (uintmax_t)nmatch;
            return 0;
        }
#endif

#ifdef _VFSF_T
        if (!strncmp(*fmt, "tn", CSTRLEN("tn"))) {

            *fmt += CSTRLEN("tn");
            vp = (void *)va_arg(ap, ptrdiff_t *);
            *(ptrdiff_t *)vp = (ptrdiff_t)nmatch;
            return 0;
        }
#endif

#ifdef _VFSF_Z
        if (!strncmp(*fmt, "zn", CSTRLEN("zn"))) {

            *fmt += CSTRLEN("zn");
            vp = (void *)va_arg(ap, size_t *);
            *(size_t *)vp = (size_t)nmatch;
            return 0;
        }
#endif

#endif

#ifdef _VFSF_P
        /* Pointer */
        if (*fmt[0] == 'p') {
            ++*fmt;
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, void **);
                *(void **)vp = (void *)(size_t)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_S
        /* String */
        if (*fmt[0] == 's' || !strncmp(*fmt, "lls", CSTRLEN("lls"))) {


			*fmt += *fmt[0] == 's' ? 1 : CSTRLEN("lls");
            if (!asup) {
                cp = va_arg(ap, char *);
                i = atos(fp, cp);
            } else {
                i = atos(fp, NULL);
            }
            return i;
        }
#endif

#ifdef _VFSF_U
#ifdef _VFSF_HH
        /* Unsigned character decimal integer */
        if (!strncmp(*fmt, "hhu", CSTRLEN("hhu"))) {

            *fmt += CSTRLEN("hhu");
            i = atou(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned char *);
                *(unsigned char *)vp = (unsigned char)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_H
        /* Unsigned short decimal integer */
        if (!strncmp(*fmt, "hu", CSTRLEN("hu"))) {

            *fmt += CSTRLEN("hu");
            i = atou(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned short *);
                *(unsigned short *)vp = (unsigned short)llu;
            }
            return i;
        }
#endif

        /* Unsigned decimal integer */
        if (*fmt[0] == 'u') {

            ++*fmt;
            i = atou(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned int *);
                *(unsigned int *)vp = (unsigned int)llu;
            }
            return i;
        }

#ifdef _VFSF_L
        /* Unsigned long decimal integer */
        if (!strncmp(*fmt, "lu", CSTRLEN("lu"))) {

            *fmt += CSTRLEN("lu");
            i = atou(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned long *);
                *(unsigned long *)vp = (unsigned long)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_LL
        /* Unsigned long long decimal integer */
        if (!strncmp(*fmt, "llu", CSTRLEN("llu"))) {

            *fmt += CSTRLEN("llu");
            i = atou(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned long long *);
                *(unsigned long long *)vp = llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_J
        /* uintmax_t decimal integer */
        if (!strncmp(*fmt, "ju", CSTRLEN("ju"))) {

            *fmt += CSTRLEN("ju");
            i = atou(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, uintmax_t *);
                *(uintmax_t *)vp = (uintmax_t)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_T
        /* ptrdiff_t decimal integer */
        if (!strncmp(*fmt, "tu", CSTRLEN("tu"))) {

            *fmt += CSTRLEN("tu");
            i = atou(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, ptrdiff_t *);
                *(ptrdiff_t *)vp = (ptrdiff_t)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_Z
        /* size_t decimal integer */
        if (!strncmp(*fmt, "zu", CSTRLEN("zu"))) {

            *fmt += CSTRLEN("zu");
            i = atou(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, size_t *);
                *(size_t *)vp = (size_t)llu;
            }
            return i;
        }
#endif
#endif

#ifdef _VFSF_X
#ifdef _VFSF_HH
        /* Character hexadecimal integer */
        if (!strncmp(*fmt, "hhx", CSTRLEN("hhx")) || \
            !strncmp(*fmt, "hhX", CSTRLEN("hhX"))) {

            *fmt += CSTRLEN("hhx");
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned char *);
                *(unsigned char *)vp = (unsigned char)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_H
        /* Short hexadecimal integer */
        if (!strncmp(*fmt, "hx", CSTRLEN("hx")) || \
            !strncmp(*fmt, "hX", CSTRLEN("hX"))) {

            *fmt += CSTRLEN("hx");
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned short *);
                *(unsigned short *)vp = (unsigned short)llu;
            }
            return i;
        }
#endif

        /* Hexadecimal integer */
        if ((*fmt[0] == 'x') || (*fmt[0] == 'X')) {
            ++*fmt;
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned int *);
                *(unsigned int *)vp = (unsigned int)llu;
            }
            return i;
        }

#ifdef _VFSF_L
        /* Long hexadecimal integer */
        if (!strncmp(*fmt, "lx", CSTRLEN("lx")) || \
            !strncmp(*fmt, "lX", CSTRLEN("lX"))) {

            *fmt += CSTRLEN("lx");
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned long *);
                *(unsigned long *)vp = (unsigned long)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_LL
        /* Long long hexadecimal integer */
        if (!strncmp(*fmt, "llx", CSTRLEN("llx")) || \
            !strncmp(*fmt, "llX", CSTRLEN("llX"))) {

            *fmt += CSTRLEN("llx");
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, unsigned long long *);
                *(unsigned long long *)vp = llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_J
        /* uintmax_t hexadecimal integer */
        if (!strncmp(*fmt, "jx", CSTRLEN("jx")) || \
            !strncmp(*fmt, "jX", CSTRLEN("jX"))) {

            *fmt += CSTRLEN("jx");
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, uintmax_t *);
                *(uintmax_t *)vp = (uintmax_t)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_T
        /* ptrdiff_t hexadecimal integer */
        if (!strncmp(*fmt, "tx", CSTRLEN("tx")) || \
            !strncmp(*fmt, "tX", CSTRLEN("tX"))) {

            *fmt += CSTRLEN("tx");
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, ptrdiff_t *);
                *(ptrdiff_t *)vp = (ptrdiff_t)llu;
            }
            return i;
        }
#endif

#ifdef _VFSF_Z
        /* size_t hexadecimal integer */
        if (!strncmp(*fmt, "zx", CSTRLEN("zx")) || \
            !strncmp(*fmt, "zX", CSTRLEN("zX"))) {

            *fmt += CSTRLEN("zx");
            i = atox(fp, &llu);
            if (!(i < 1)) {
                vp = (void *)va_arg(ap, size_t *);
                *(size_t *)vp = (size_t)llu;
            }
            return i;
        }
#endif
#endif

#ifdef _VFSF_CHARSEQ
		/* character sets */
		if (**fmt == '[') {
			(*fmt)++;
			return atocs(fp, fmt, ap);
		}
#endif

        /* 'Escaped' '%' character */
        if ((*fmt)[0] == '%') {
			skipws(fp);
			c = fgetc(fp);
			if (c == '%') {
				++*fmt;
				++nmatch;
				return 0;
			}
			else if (c != EOF) {
				ungetc(c, fp);
			}
			return EOF - 1;
        }

        /* Unrecognized conversion */
        ++*fmt;
        return 0;
    }

    /* No conversion, just intervening text */
    c = fgetc(fp);
	if (isspace(*fmt[0])) {
		// skip past any directive whitespace
		do {
			++*fmt;
		} while(isspace(*fmt[0]));
		// skip past any input whitespace
		while (isspace(c)) {
			++nmatch;
			c = fgetc(fp);
		}
		if (c != EOF) {
			ungetc(c, fp);
		}
	}
	else if (c == *fmt[0]) {
        ++*fmt;
        ++nmatch;
    } else {
		if (c == EOF) {
			return EOF;
		}
        ungetc(c, fp);
        return EOF - 1;
    }

    return 0;
}

int vfscanf(FILE *fp, const char *fmt, va_list ap)
{
    char *cfmt;
    int n, saved_errno;

    cfmt = (char *)fmt;
    ncnv = nmatch = 0;
	saved_errno = errno;
    while (*cfmt) {
		errno = 0;
        n = vfsfcnvrt(fp, &cfmt, ap);
        if (n < 0) {
            break;
        }
        ncnv += n;
    }
	errno = saved_errno;
    if (n == EOF && ncnv == 0) {
        return n;
    }

    return ncnv;
}
