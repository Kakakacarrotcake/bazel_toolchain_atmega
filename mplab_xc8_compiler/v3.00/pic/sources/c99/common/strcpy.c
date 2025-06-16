#include <string.h>

char *strcpy(char *restrict dest, const char *restrict src)
{
	char *d = dest;
	while ((*d++ = *src++));
	return dest;
}
