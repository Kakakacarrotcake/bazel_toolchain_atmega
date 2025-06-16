#include <ctype.h>
#include "libc.h"

#ifdef isalnum
#undef isalnum
#endif

int isalnum(int c)
{
	return isalpha(c) || isdigit(c);
}

int __isalnum_l(int c, locale_t l)
{
	return isalnum(c);
}

weak_alias(__isalnum_l, isalnum_l);
