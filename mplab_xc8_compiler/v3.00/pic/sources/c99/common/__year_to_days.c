#include <limits.h>

/* Return days elapsed since epoch. This way, a smaller type can be used,
   and overflow detection is easier.
   Only seconds worth 136 years can be represented with an unsigned
   32-bit value, as 136*86400*365 < pow(2,32) < 137*86400*365.
   MUSL's epoch is 1 Jan 1970, so the representable year range is
   1970 to (1970+136=2106). The YEAR parameter is
   offset from 1900 though, so return UINT_MAX indicating overflow if
   (1970-1900) <= YEAR <= (2106-1900).
   Max number of days is 136 * 365 + leap days, which comfortably fits
   in an unsigned 16-bit int.
   */

unsigned int __year_to_days(int year, int *is_leap)
{
	if (year > 206 || year < 70) {
	  return UINT_MAX;
	}

	/* The below code is the same as time/__year_to_secs, except
       for returning elapsed days instead of multiplying by 86400 and
       returning elapsed seconds. */
	if (year - 2 <= 136) {
		int y = (int)year;
		int leaps = (y-68)>>2;
		if (!((y-68)&3)) {
			leaps--;
			if (is_leap) *is_leap = 1;
		} else if (is_leap) *is_leap = 0;
		return (unsigned int)(365*(y-70) + leaps);
	}

	int cycles, centuries, leaps, rem;

	if (!is_leap) is_leap = &(int){0};
	cycles = (int)(year-100) / 400;
	rem = (year-100) % 400;
	if (rem < 0) {
		cycles--;
		rem += 400;
	}
	if (!rem) {
		*is_leap = 1;
		centuries = 0;
		leaps = 0;
	} else {
		if (rem >= 200) {
			if (rem >= 300) centuries = 3, rem -= 300;
			else centuries = 2, rem -= 200;
		} else {
			if (rem >= 100) centuries = 1, rem -= 100;
			else centuries = 0;
		}
		if (!rem) {
			*is_leap = 0;
			leaps = 0;
		} else {
			leaps = (unsigned int)rem / 4U;
			rem %= 4U;
			*is_leap = !rem;
		}
	}

	leaps += 97*cycles + 24*centuries - *is_leap;

	return (unsigned int)((year-100) * 365 + leaps + 10957 + 1);
}
