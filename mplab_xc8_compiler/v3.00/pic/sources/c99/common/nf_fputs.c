/* "No file system" fputs */
#include <stdio.h>

#include "stdio_impl.h"

#ifdef STDIO_NO_FILE_IO

int fputs(const char *s, FILE *fp)
{
    char c;
    int i;

    i = 0;
    while ((c = *(s + i))) {
	fputc(c,fp);
        ++i;
    }
    return i;
}

#endif
