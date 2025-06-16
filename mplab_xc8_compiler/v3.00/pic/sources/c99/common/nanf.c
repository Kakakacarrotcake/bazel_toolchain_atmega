#include <math.h>
#include <stdint.h>

float nanf(const char *s)
{
	union {float f; uint32_t i;} u;
	u.i = 0x7FFFFFFF;
	return u.f;
}
