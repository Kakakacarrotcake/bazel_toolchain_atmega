#include <stdio.h>
#include "stdio_impl.h"

#ifdef putchar
#undef putchar
#endif

int putchar(int c)
{
	return fputc(c, stdout);
}
