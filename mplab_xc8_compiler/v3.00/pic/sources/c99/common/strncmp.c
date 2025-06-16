#include <string.h>

int strncmp(const char *l, const char *r, size_t n)
{
	if (!n--) return 0;
	for (; *l && *r && n && *l == *r ; l++, r++, n--);
	return *(unsigned char*)l - *(unsigned char *)r;
}
