#include "stdio_impl.h"
#include "libc.h"

#ifdef STDIO_NO_FILE_IO

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	int cnt;
	FILE f;

	f.buffer = s;
	f.limit = INT_MAX-1;
	f.count = 0;
	cnt = vfprintf(&f, fmt, ap);
	if(cnt>=0) {
		s[MIN(cnt, INT_MAX-1)] = '\0';
	}
	return cnt;
}
#endif
