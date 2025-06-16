#include "stdio_impl.h"

int puts(const char *s)
{
#ifdef STDIO_NO_FILE_IO
	while (*s != '\0') {
		putch(*s);
		s++;
	}
	putch('\n');
	return 0;
#else
	int r;
	FLOCK(stdout);
	r = -(fputs(s, stdout) < 0 || putc_unlocked('\n', stdout) < 0);
	FUNLOCK(stdout);
	return r;
#endif
}
