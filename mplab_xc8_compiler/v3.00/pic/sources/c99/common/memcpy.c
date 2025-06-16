#include <stdlib.h>

void *
memcpy(void * d1, const void * s1, register size_t n)
{

	register char *		d;
	register const char *	s;
	register unsigned char tmp;

	s = s1;
	d = d1;
	while(n--) {
		tmp = *s++;
		*d++ = tmp;
	}
	return d1;
}

