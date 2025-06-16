#include "stdio_impl.h"
#include <sys/ioctl.h>

size_t __stdout_write(FILE *f, const unsigned char *buf, size_t len)
{
#ifndef STDIO_NO_FILE_IO
	struct winsize wsz;
	f->write = __stdio_write;
#if 0
	if (!(f->flags & F_SVB) && __syscall(SYS_ioctl, f->fd, TIOCGWINSZ, &wsz))
		f->lbf = -1;
#endif
	return __stdio_write(f, buf, len);
#else
	return 0;
#endif
}
