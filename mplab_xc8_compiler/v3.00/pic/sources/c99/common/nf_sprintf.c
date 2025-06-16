#include <stdarg.h>
#include <stdio.h>

#include "stdio_impl.h"

#ifdef STDIO_NO_FILE_IO
/* "No file system" sprintf */

int sprintf(char *restrict s, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	FILE f;

	va_start(ap, fmt);
	f.buffer = s;
	f.count = 0;
	f.limit = 0;
	ret = vfprintf(&f, fmt, ap);
	s[f.count] = '\0';
	va_end(ap);
	return ret;
}

#endif
