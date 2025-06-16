#include "stdio_impl.h"
#include <limits.h>
#include <string.h>

#ifdef STDIO_NO_FILE_IO
char *gets(char *s)
{
	int c;
	char *s1 = s;

	for(;;) {
		c = _NF_GETC();
		if(c < 0) {
			if(s1 != s) {
				*s1 = '\0';
			}
			return NULL;
		}
		if(c == '\n') {
			*s1 = '\0';
			break;
		}
		*s1++ = (char)c;
	}
	return s;
}
#endif

