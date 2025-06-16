#include <stdio.h>

#include "stdio_impl.h"

#ifdef STDIO_NO_FILE_IO
/* "No file system" fputc */

int fputc(int c, FILE *fp)
{
    extern void putch(char);

    if ((fp == stdout) || (fp == stderr)) {
        putch((char)c);
    } else {
		if((fp->limit == 0) || (fp->count < fp->limit)) {
#pragma warning push
#pragma warning disable 1498
			fp->buffer[fp->count] = (char)c;
#pragma warning pop
			++fp->count;
		}
    }
    return (unsigned char)c;
}

#endif
