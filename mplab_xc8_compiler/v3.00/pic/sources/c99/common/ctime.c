#include <time.h>

struct tm *__gmtime_r(const time_t *restrict, struct tm *restrict);

char *ctime(const time_t *t)
{
	struct tm tm;
	if (!__gmtime_r(t, &tm)) {
		return 0;
	}
	return asctime(&tm);
}
