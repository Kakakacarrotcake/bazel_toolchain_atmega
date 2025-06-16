#include <string.h>

char *strcat(char *restrict dest, const char *restrict src)
{
	unsigned i = 0;
	while(dest[i] != '\0') {
		i++;
	}
	while ((dest[i] = *src) != '\0') {
		i++;
		src++;
	}
	return dest;
}
