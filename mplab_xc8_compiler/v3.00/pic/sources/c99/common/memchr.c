#include <string.h>
#include <stdint.h>
#include <limits.h>

void *memchr(const void *src, int c, size_t n)
{
	const unsigned char *s = src;
	while (n--) {
		if (*s == (unsigned char)c) {
			return (void*)s;
		}
		else {
			++s;
		}
	}
	return 0;
}
