#include "stdio_impl.h"

#ifndef STDIO_NO_FILE_IO
static int dummy(int fd)
{
	return fd;
}

weak_alias(dummy, __aio_close);
#endif

int __stdio_close(FILE *f)
{
#ifndef STDIO_NO_FILE_IO
	return syscall(SYS_close, __aio_close(f->fd));
#else
	return 0;
#endif
}
