#include <string.h>

void *memset(void *dest, int c, size_t n)
{
	unsigned char * p = dest;
	while (n--) {
		*p++ = (unsigned char)c;
	}
	return dest;
}
