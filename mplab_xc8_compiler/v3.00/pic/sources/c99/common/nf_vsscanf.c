#include "stdio_impl.h"
#include "libc.h"

#ifdef STDIO_NO_FILE_IO
int vsscanf(const char *restrict s, const char *restrict fmt, va_list ap)
{
	FILE f;

	f.source = s;
	f.count = 0;
	f.ungetcnt = 0;
	return vfscanf(&f, fmt, ap);
}
#endif

