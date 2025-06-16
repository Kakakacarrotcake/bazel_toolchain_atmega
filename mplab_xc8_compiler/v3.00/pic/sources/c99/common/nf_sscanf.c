#include <stdarg.h>
#include <stdio.h>

#include "stdio_impl.h"

#ifdef STDIO_NO_FILE_IO
/* "No file system" sscanf */

int sscanf(const char *restrict s, const char *restrict fmt, ...)
{
	int ret;
	FILE f;

	va_list ap;
	va_start(ap, fmt);
	f.source = s;
	f.count = 0;
	f.ungetcnt = 0;
	ret = vfscanf(&f, fmt, ap);
	va_end(ap);
	return ret;
}

#endif
