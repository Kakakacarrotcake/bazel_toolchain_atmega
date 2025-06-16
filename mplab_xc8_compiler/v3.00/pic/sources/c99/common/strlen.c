#include <string.h>
#include <stdint.h>
#include <limits.h>

size_t strlen(const char *s)
{
	const char *a = s;
	while(*s) {
		s++;
	}
	return (size_t)(s-a);
}
