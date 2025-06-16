#include "stdio_impl.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#ifdef STDIO_NO_FILE_IO

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	int cnt;
	FILE f;

	if (n > INT_MAX) {
		errno = EOVERFLOW;
		return -1;
	}
	f.buffer = s;
	if(n>1) {
		f.limit = (int)n-1;
		f.count = 0;
	}
	else {
		f.limit = 1;
		f.count = 1;
	}
	cnt = vfprintf(&f, fmt, ap);
	if((n>0) && (cnt>=0)) {
		s[MIN(cnt, (int)n-1)] = '\0';
	}
	return cnt;
}
#endif

