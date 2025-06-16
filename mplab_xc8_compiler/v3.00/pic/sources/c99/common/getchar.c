#include <stdio.h>
#include "stdio_impl.h"

#ifdef getchar
#undef getchar
#endif

int getchar(void)
{
	return fgetc(stdin);
}
