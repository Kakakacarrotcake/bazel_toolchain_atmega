#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#include "stdio_impl.h"

#ifdef STDIO_NO_FILE_IO
/* "No file system" snprintf */

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...)
{
	int cnt;
	va_list ap;
	FILE f;

	if (n > INT_MAX) {
		errno = EOVERFLOW;
		return -1;
	}
	va_start(ap, fmt);
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
	va_end(ap);
	return cnt;
}
#endif
