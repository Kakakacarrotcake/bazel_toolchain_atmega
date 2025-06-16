#include <string.h>
#include <stdint.h>

char *strstr(const char *h, const char *n)
{
	if (n != NULL) {
		size_t nl = strlen(n);
		while(h && *h) {
			if(strncmp(h, n, nl) == 0)
				return ( char *)h;
			h = strchr(h+1, *n);
		}
	}
	return NULL;
}
