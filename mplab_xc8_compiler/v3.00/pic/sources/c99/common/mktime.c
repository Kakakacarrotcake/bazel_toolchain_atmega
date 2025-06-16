#include "../time_impl.h"
#include <errno.h>

time_t mktime(struct tm *tm)
{
	struct tm new;
	_Bool overflow;
	unsigned long t = __tm_to_secs(tm, &overflow);

	if (overflow) {
		goto error;
	}
	new.tm_isdst = 0;
	new.__tm_gmtoff = 0;
	new.__tm_zone = "GMT";
	if (__secs_to_tm(t, &new) < 0) {
		goto error;
	}
	*tm = new;
	return t;

error:
	errno = EOVERFLOW;
	return -1ul;
}
