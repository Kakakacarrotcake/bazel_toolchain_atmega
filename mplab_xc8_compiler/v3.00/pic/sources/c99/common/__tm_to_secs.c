#include "../time_impl.h"
#include "limits.h"

unsigned long __tm_to_secs(const struct tm *tm, _Bool *overflow)
{
	int is_leap;
	int year = tm->tm_year;
	int month = tm->tm_mon;
	if (month >= 12 || month < 0) {
		int adj = month / 12;
		month %= 12;
		if (month < 0) {
			adj--;
			month += 12;
		}
		year += adj;
	}

	long days = __year_to_days(year, &is_leap);
    if (days == UINT_MAX) {
	    *overflow = 1;
		return 0;
	}
	days += __month_to_days(month, is_leap);
	days += (tm->tm_mday-1);

	/* Max value of days at this point is
       INT_MAX + 345 + (136 * 365) = 82752 at this point */
	/* This computation cannot overflow a long as
       (3600 * INT_MAX + 60 * INT_MAX + INT_MAX) < LONG_MAX */
    long secs = 3600L * tm->tm_hour + 60L * tm->tm_min + tm->tm_sec;
	long secdays = (secs + (86400L - 1)) / 86400L;
	/* LONG_MAX/86400 + days cannot exceed LONG_MAX, so no overflow can occur */
	long totaldays = days + secdays;

    /* floor(ULONG_MAX / (24 * 60 * 60)) = 49710
	   so 0 <= days <= 49710 is max representable range.
       Overflow if outside range. */
    if (totaldays < 0 || totaldays > 49710) {
	    *overflow = 1;
		return 0;
	}
	*overflow = 0;
    return (unsigned long)((days * 86400) + secs);
}
