#include <stdio.h>
#include <stdarg.h>
#include "libc.h"
#include "stdio_impl.h"

int scanf(const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vfscanf(stdin, fmt, ap);
	va_end(ap);
	return ret;
}

weak_alias(scanf,__isoc99_scanf);
