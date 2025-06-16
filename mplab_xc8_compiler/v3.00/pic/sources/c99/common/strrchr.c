#include <string.h>

char *strrchr(const char *s, int c)
{
	size_t n = strlen(s) + 1;
	while (n--) {
		if (s[n]==(unsigned char)c) {
			return (char*)s+n;
		}
	}
	return 0;
}
