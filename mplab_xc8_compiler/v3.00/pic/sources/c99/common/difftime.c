#include <time.h>

double difftime(time_t t1, time_t t0)
{
	return (double)(signed long)(t1-t0);
}
