#include <string.h>

char *strncpy(char *restrict d, const char *restrict s, size_t n)
{
	register char *	cp;
	cp = d;
	while(n) {
		n--;
		if(!(*cp++ = *s++))
			break;
	}
	while(n--)
		*cp++ = 0;
	return d;
}
