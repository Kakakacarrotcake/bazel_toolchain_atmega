#include <string.h>

char *strpbrk(const char *s, const char *b)
{
	while(*s) {
		if(strchr(b, *s)) {
			return (char *)s;
		}
		s++;
	}
	return NULL;
}
