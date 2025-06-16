#include <time.h>

time_t time(time_t *t)
{
	if (t) {
		*t = (time_t)-1;
	}
	return (time_t)-1;
}
