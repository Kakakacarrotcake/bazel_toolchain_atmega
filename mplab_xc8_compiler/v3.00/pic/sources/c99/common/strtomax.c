#include <inttypes.h>
#include <stdlib.h>

intmax_t 
strtoimax(const char *restrict s, char **restrict p, int base)
{
#if __SIZEOF_LONG_LONG__ == 8
	return strtoll(s, p, base);
#else
	return strtol(s, p, base);
#endif
}

uintmax_t 
strtoumax(const char *restrict s, char **restrict p, int base)
{
#if __SIZEOF_LONG_LONG__ == 8
	return strtoull(s, p, base);
#else
	return strtoul(s, p, base);
#endif
}

