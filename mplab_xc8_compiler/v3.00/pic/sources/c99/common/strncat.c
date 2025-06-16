#include <string.h>

char *strncat(char *restrict dest, const char *restrict src, size_t n)
{
	char *	cp = dest;
	while(*cp != '\0') {
		cp++;
	}
	while (n != 0 && (*cp = *src) != '\0') {
		cp++;
		src++;
		n--;
	}
	if (n == 0) {
		*cp = '\0';
	}
	return dest;
}
